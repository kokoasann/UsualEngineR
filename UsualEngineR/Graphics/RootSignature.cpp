#include "PreCompile.h"
#include "RootSignature.h"


namespace UER
{
	
	enum {
		enDescriptorHeap_CB,
		enDescriptorHeap_SRV,
		enDescriptorHeap_UAV,
		enNumDescriptorHeap
	};
	
	bool RootSignature::Init(
		D3D12_FILTER samplerFilter,
		D3D12_TEXTURE_ADDRESS_MODE textureAdressModeU,
		D3D12_TEXTURE_ADDRESS_MODE textureAdressModeV,
		D3D12_TEXTURE_ADDRESS_MODE textureAdressModeW,
		UINT maxCbvDescriptor ,
		UINT maxSrvDescriptor ,
		UINT maxUavDescritor
	)
	{
		auto d3dDevice = g_graphicsEngine->GetD3DDevice();
	
		D3D12_STATIC_SAMPLER_DESC sampler = {};
		sampler.Filter = samplerFilter;
		sampler.AddressU = textureAdressModeU;
		sampler.AddressV = textureAdressModeV;
		sampler.AddressW = textureAdressModeW;
		sampler.MipLODBias = 0;
		sampler.MaxAnisotropy = 0;
		sampler.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
		sampler.BorderColor = D3D12_STATIC_BORDER_COLOR_OPAQUE_BLACK;
		sampler.MinLOD = 0.0f;
		sampler.MaxLOD = D3D12_FLOAT32_MAX;
		sampler.ShaderRegister = 0;
		sampler.RegisterSpace = 0;
		sampler.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
	
		CD3DX12_DESCRIPTOR_RANGE1 ranges[enNumDescriptorHeap];
		CD3DX12_ROOT_PARAMETER1 rootParameters[enNumDescriptorHeap];
	
		ranges[enDescriptorHeap_CB].Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, maxCbvDescriptor, 0, 0, D3D12_DESCRIPTOR_RANGE_FLAG_DATA_STATIC);
		rootParameters[enDescriptorHeap_CB].InitAsDescriptorTable(1, &ranges[enDescriptorHeap_CB]);
	
		ranges[enDescriptorHeap_SRV].Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, maxSrvDescriptor, 0, 0, D3D12_DESCRIPTOR_RANGE_FLAG_DATA_STATIC);
		rootParameters[enDescriptorHeap_SRV].InitAsDescriptorTable(1, &ranges[enDescriptorHeap_SRV]);
	
		ranges[enDescriptorHeap_UAV].Init(D3D12_DESCRIPTOR_RANGE_TYPE_UAV, maxUavDescritor, 0, 0, D3D12_DESCRIPTOR_RANGE_FLAG_DATA_STATIC);
		rootParameters[enDescriptorHeap_UAV].InitAsDescriptorTable(1, &ranges[enDescriptorHeap_UAV]);
	
		// Allow input layout and deny uneccessary access to certain pipeline stages.
		D3D12_ROOT_SIGNATURE_FLAGS rootSignatureFlags =
			D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT |
			D3D12_ROOT_SIGNATURE_FLAG_DENY_HULL_SHADER_ROOT_ACCESS |
			D3D12_ROOT_SIGNATURE_FLAG_DENY_DOMAIN_SHADER_ROOT_ACCESS |
			D3D12_ROOT_SIGNATURE_FLAG_DENY_GEOMETRY_SHADER_ROOT_ACCESS;
	
		CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC rootSignatureDesc;
		rootSignatureDesc.Init_1_1(_countof(rootParameters), rootParameters, 1, &sampler, rootSignatureFlags);
		Microsoft::WRL::ComPtr<ID3DBlob> signature;
		Microsoft::WRL::ComPtr<ID3DBlob> error;
		D3DX12SerializeVersionedRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1, &signature, &error);
		auto hr = d3dDevice->CreateRootSignature(0, signature->GetBufferPointer(), signature->GetBufferSize(), IID_PPV_ARGS(&m_rootSignature));
		if (FAILED(hr) ){
			//ルートシグネチャの作成に失敗した。
			return false;
		}
		return true;
	}
	
	bool RootSignature::Init(Shader& shader)
	{
		//シェーダーからルートシグネチャ情報を取得
		ID3DBlob* sig = nullptr;
		auto shaderBlob = shader.GetCompiledBlob();
	
		auto hr = D3DGetBlobPart(shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(),
			D3D_BLOB_ROOT_SIGNATURE, 0, &sig);
		//ルートシグネチャの生成
		auto d3dDevice = g_graphicsEngine->GetD3DDevice();
		hr = d3dDevice->CreateRootSignature(0, sig->GetBufferPointer(), sig->GetBufferSize(),
			IID_PPV_ARGS(&m_rootSignature));
		return hr == S_OK;
	}


	bool RootSignature::Init(
		D3D12_FILTER samplerFilter,
		D3D12_TEXTURE_ADDRESS_MODE textureAdressModeU,
		D3D12_TEXTURE_ADDRESS_MODE textureAdressModeV,D3D12_TEXTURE_ADDRESS_MODE textureAdressModeW,
		int numTables, TableInitData* tableInitDatas
	)
	{
		auto d3dDevice = g_graphicsEngine->GetD3DDevice();

		D3D12_STATIC_SAMPLER_DESC sampler = {};
		sampler.Filter = samplerFilter;
		sampler.AddressU = textureAdressModeU;
		sampler.AddressV = textureAdressModeV;
		sampler.AddressW = textureAdressModeW;
		sampler.MipLODBias = 0;
		sampler.MaxAnisotropy = 0;
		sampler.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
		sampler.BorderColor = D3D12_STATIC_BORDER_COLOR_OPAQUE_BLACK;
		sampler.MinLOD = 0.0f;
		sampler.MaxLOD = D3D12_FLOAT32_MAX;
		sampler.ShaderRegister = 0;
		sampler.RegisterSpace = 0;
		sampler.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;

		//DescTableData* dtd = new DescTableData[numTables];
		CD3DX12_DESCRIPTOR_RANGE1* ranges = new CD3DX12_DESCRIPTOR_RANGE1[numTables * enNumDescriptorHeap];
		CD3DX12_ROOT_PARAMETER1* rootParameters = new CD3DX12_ROOT_PARAMETER1[numTables];
		for (int i = 0; i < numTables; i++)
		{
			auto& tid = tableInitDatas[i];
			//auto& DT = dtd[i];
			int c = int(tid.maxCBV != 0) + int(tid.maxSRV != 0) + int(tid.maxUAV != 0);
			//DT.ranges = new CD3DX12_DESCRIPTOR_RANGE1[c];

			int j = i * enNumDescriptorHeap;
			if (tid.maxCBV)
			{
				ranges[j].Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, tid.maxCBV, 0, 0, D3D12_DESCRIPTOR_RANGE_FLAG_DATA_STATIC);
				j++;
			}
			if (tid.maxSRV)
			{
				ranges[j].Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, tid.maxSRV, 0, 0, D3D12_DESCRIPTOR_RANGE_FLAG_DATA_STATIC);
				j++;
			}
			if (tid.maxUAV)
			{
				ranges[j].Init(D3D12_DESCRIPTOR_RANGE_TYPE_UAV, tid.maxUAV, 0, 0, D3D12_DESCRIPTOR_RANGE_FLAG_DATA_STATIC);
			}

			rootParameters[i].InitAsDescriptorTable(c, &ranges[i * enNumDescriptorHeap]);
			rootParameters[i].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
			rootParameters[i].DescriptorTable.NumDescriptorRanges = c;
			rootParameters[i].DescriptorTable.pDescriptorRanges = &ranges[i* enNumDescriptorHeap];
			rootParameters[i].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
		}

		// Allow input layout and deny uneccessary access to certain pipeline stages.
		D3D12_ROOT_SIGNATURE_FLAGS rootSignatureFlags =
			D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT |
			D3D12_ROOT_SIGNATURE_FLAG_DENY_HULL_SHADER_ROOT_ACCESS |
			D3D12_ROOT_SIGNATURE_FLAG_DENY_DOMAIN_SHADER_ROOT_ACCESS |
			D3D12_ROOT_SIGNATURE_FLAG_DENY_GEOMETRY_SHADER_ROOT_ACCESS;

		CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC rootSignatureDesc;
		rootSignatureDesc.Init_1_1(numTables, rootParameters, 1, &sampler, rootSignatureFlags);
		Microsoft::WRL::ComPtr<ID3DBlob> signature;
		Microsoft::WRL::ComPtr<ID3DBlob> error;
		D3DX12SerializeVersionedRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1, &signature, &error);
		auto hr = d3dDevice->CreateRootSignature(0, signature->GetBufferPointer(), signature->GetBufferSize(), IID_PPV_ARGS(&m_rootSignature));

		delete[] ranges;
		delete[] rootParameters;
		return true;
	}
}
