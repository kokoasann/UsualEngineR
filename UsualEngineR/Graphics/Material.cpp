#include "PreCompile.h"
#include "Material.h"

#include "PreRender/GBuffer.h"

namespace UER
{
	
	//ルートシグネチャとパイプラインステート周りはカリカリカリ
	enum {
		enDescriptorHeap_CB,
		enDescriptorHeap_SRV,
		enNumDescriptorHeap
	};
		
	void Material::InitTexture(const TkmFile::SMaterial& tkmMat)
	{
		if (tkmMat.albedoMap != nullptr) {
			m_albedoMap.InitFromMemory(tkmMat.albedoMap.get(), tkmMat.albedoMapSize,tkmMat.albedoMapFileName);
		}
		if (tkmMat.normalMap != nullptr) {
			m_normalMap.InitFromMemory(tkmMat.normalMap.get(), tkmMat.normalMapSize,tkmMat.normalMapFileName);
		}
		if (tkmMat.specularMap != nullptr) {
			m_specularMap.InitFromMemory(tkmMat.specularMap.get(), tkmMat.specularMapSize,tkmMat.specularMapFileName);
		}
	}
	void Material::InitFromTkmMaterila(
		const TkmFile::SMaterial& tkmMat,
		const wchar_t* fxFilePath,
		const wchar_t* psfxFilePath,
		const char* vsEntryPointFunc,
		const char* psEntryPointFunc)
	{
		//テクスチャをロード。
		InitTexture(tkmMat);
		
		//定数バッファを作成。
		SMaterialParam matParam;
		matParam.hasNormalMap = m_normalMap.IsValid() ? 1 : 0;
		matParam.hasSpecMap = m_specularMap.IsValid() ? 1 : 0;
		m_constantBuffer.Init(sizeof(SMaterialParam), &matParam);
	
		//ルートシグネチャを初期化。
		m_rootSignature.Init(
			D3D12_FILTER_MIN_MAG_MIP_LINEAR,
			D3D12_TEXTURE_ADDRESS_MODE_WRAP,
			D3D12_TEXTURE_ADDRESS_MODE_WRAP,
			D3D12_TEXTURE_ADDRESS_MODE_WRAP);

		/*RootSignature::TableInitData tid[3] = {0};
		tid[0].maxCBV = 1;
		tid[0].maxSRV = 10;
		tid[0].maxUAV = 0;
		tid[0].baseRegisterCBV = 0;
		tid[0].baseRegisterSRV = 0;
		tid[0].baseRegisterUAV = 0;
		tid[1].maxCBV = 1;
		tid[1].maxSRV = 0;
		tid[1].maxUAV = 0;
		tid[1].baseRegisterCBV = 1;
		tid[1].baseRegisterSRV = 0;
		tid[1].baseRegisterUAV = 0;
		tid[2].maxCBV = 1;
		tid[2].maxSRV = 0;
		tid[2].maxUAV = 0;
		tid[2].baseRegisterCBV = 2;
		tid[2].baseRegisterSRV = 0;
		tid[2].baseRegisterUAV = 0;

		m_rootSignature.Init(
			D3D12_FILTER_MIN_MAG_MIP_LINEAR,
			D3D12_TEXTURE_ADDRESS_MODE_WRAP,
			D3D12_TEXTURE_ADDRESS_MODE_WRAP,
			D3D12_TEXTURE_ADDRESS_MODE_WRAP,
			3, tid
		);*/
	
		//シェーダーを初期化。
		InitShaders(fxFilePath, psfxFilePath, vsEntryPointFunc, psEntryPointFunc);
	
		//パイプラインステートを初期化。
		InitPipelineState();
	
	}
	void Material::InitEmmission(const wchar_t* emsPath)
	{
		m_emmissionMap.InitFromDDSFile(emsPath);
		m_isLoadEmmission = true;
	}
	void Material::InitPipelineState()
	{
		// 頂点レイアウトを定義する。
		D3D12_INPUT_ELEMENT_DESC inputElementDescs[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
			{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
			{ "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 24, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
			{ "BINORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 36, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 48, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
			{ "BLENDINDICES", 0, DXGI_FORMAT_R32G32B32A32_UINT, 0, 56, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
			{ "BLENDWEIGHT", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 72, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		};
	
		//パイプラインステートを作成。
		D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = { 0 };
		psoDesc.InputLayout = { inputElementDescs, _countof(inputElementDescs) };
		psoDesc.pRootSignature = m_rootSignature.Get();
		psoDesc.VS = CD3DX12_SHADER_BYTECODE(m_vsSkinModel.GetCompiledBlob());
		psoDesc.PS = CD3DX12_SHADER_BYTECODE(m_psModel.GetCompiledBlob());
		psoDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
		psoDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
		psoDesc.DepthStencilState.DepthEnable = TRUE;
		psoDesc.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
		psoDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;
		psoDesc.DepthStencilState.StencilEnable = FALSE;
		psoDesc.SampleMask = UINT_MAX;
		psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
		psoDesc.NumRenderTargets = TO_INT(EGBufferKind::NumKind);
		psoDesc.RTVFormats[TO_INT(EGBufferKind::Diffuse)] = DXGI_FORMAT_R32G32B32A32_FLOAT;		//アルベドカラー出力用。
		psoDesc.RTVFormats[TO_INT(EGBufferKind::Normal)] = DXGI_FORMAT_R32G32B32A32_FLOAT;			//法線出力用。	
		psoDesc.RTVFormats[TO_INT(EGBufferKind::Depth)] = DXGI_FORMAT_R32_FLOAT;	//Z値。
		psoDesc.RTVFormats[TO_INT(EGBufferKind::Specular)] = DXGI_FORMAT_R32_FLOAT;			//法線出力用。	
		psoDesc.RTVFormats[TO_INT(EGBufferKind::Tangent)] = DXGI_FORMAT_R32G32B32A32_FLOAT;	//Z値。
		psoDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT;
		psoDesc.SampleDesc.Count = 1;
	
		m_skinModelPipelineState.Init(psoDesc);


		//続いてスキンなしモデル用を作成。
		psoDesc.VS = CD3DX12_SHADER_BYTECODE(m_vsNonSkinModel.GetCompiledBlob());
		m_nonSkinModelPipelineState.Init(psoDesc);



		//SHADOW NON SKIN MODEL
		psoDesc.NumRenderTargets = 1;
		psoDesc.RTVFormats[0] = DXGI_FORMAT_R32_FLOAT;									//light view depth。
		psoDesc.RTVFormats[TO_INT(EGBufferKind::Normal)] = DXGI_FORMAT_UNKNOWN;			//法線出力用。	
		psoDesc.RTVFormats[TO_INT(EGBufferKind::Depth)] = DXGI_FORMAT_UNKNOWN;			//Z値。
		psoDesc.RTVFormats[TO_INT(EGBufferKind::Specular)] = DXGI_FORMAT_UNKNOWN;		//法線出力用。	
		psoDesc.RTVFormats[TO_INT(EGBufferKind::Tangent)] = DXGI_FORMAT_UNKNOWN;		//Z値。
		psoDesc.PS = CD3DX12_SHADER_BYTECODE(m_psShadow.GetCompiledBlob());
		psoDesc.VS = CD3DX12_SHADER_BYTECODE(m_vsNonSkinShadow.GetCompiledBlob());
		m_shadowNonSkinModelPipelineState.Init(psoDesc);

		// SHADOW SKIN MODEL
		psoDesc.VS = CD3DX12_SHADER_BYTECODE(m_vsSkinShadow.GetCompiledBlob());
		m_shadowSkinModelPipelineState.Init(psoDesc);
	

		////続いて半透明マテリアル用。
		//psoDesc.VS = CD3DX12_SHADER_BYTECODE(m_vsSkinModel.GetCompiledBlob());
		//psoDesc.BlendState.IndependentBlendEnable = TRUE;
		//psoDesc.BlendState.RenderTarget[0].BlendEnable = TRUE;
		//psoDesc.BlendState.RenderTarget[0].SrcBlend = D3D12_BLEND_SRC_ALPHA;
		//psoDesc.BlendState.RenderTarget[0].DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
		//psoDesc.BlendState.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
	
		//
		//m_transSkinModelPipelineState.Init(psoDesc);
	
		//psoDesc.VS = CD3DX12_SHADER_BYTECODE(m_vsNonSkinModel.GetCompiledBlob());
		//m_transNonSkinModelPipelineState.Init(psoDesc);
	
	}
	void Material::InitShaders(
		const wchar_t* fxFilePath,
		const wchar_t* psfxFilePath,
		const char* vsEntryPointFunc,
		const char* psEntryPointFunc
	)
	{
		m_vsNonSkinModel.LoadVS(fxFilePath, vsEntryPointFunc);
		m_vsSkinModel.LoadVS(fxFilePath, vsEntryPointFunc);
		m_psModel.LoadPS(psfxFilePath, psEntryPointFunc);

		m_vsSkinShadow.LoadVS(L"Assets/shader/Shadow.fx", "VSMain_Skin");
		m_vsNonSkinShadow.LoadVS(L"Assets/shader/Shadow.fx", "VSMain_NonSkin");
		m_psShadow.LoadPS(L"Assets/shader/Shadow.fx", "PSMain");
	}
	void Material::BeginRender(RenderContext& rc, int hasSkin)
	{
		rc.SetRootSignature(m_rootSignature);
		
		if (hasSkin) {
			rc.SetPipelineState(m_skinModelPipelineState);
			//rc.SetPipelineState(m_transSkinModelPipelineState);
		}
		else {
			rc.SetPipelineState(m_nonSkinModelPipelineState);
			//rc.SetPipelineState(m_transNonSkinModelPipelineState);
		}
	}

	void Material::BeginRenderShadow(RenderContext& rc, int hasSkin)
	{
		rc.SetRootSignature(m_rootSignature);

		if (hasSkin) {
			rc.SetPipelineState(m_shadowSkinModelPipelineState);
		}
		else {
			rc.SetPipelineState(m_shadowNonSkinModelPipelineState);
		}
	}


}
