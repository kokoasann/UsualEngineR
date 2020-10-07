#include "PreCompile.h"
#include "Bloom.h"

#include "PostEffect.h"

namespace UER
{
	void Bloom::Release()
	{
	}

	void Bloom::Init(PostEffect& pe)
	{
		float clearColor[4] = { 0,0,0,0 };
		m_combineRT.Create(FRAME_BUFFER_W, FRAME_BUFFER_H, 0, 1, DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_FORMAT_D16_UNORM, clearColor);
		m_luminanceRT.Create(FRAME_BUFFER_W, FRAME_BUFFER_H, 0, 1, DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_FORMAT_D16_UNORM, clearColor);
		auto* currentRT = &m_luminanceRT;
		for (int i = 0; i < MAX_GAUSS; i++)
		{
			m_blur[i].Init(FRAME_BUFFER_W, FRAME_BUFFER_H, currentRT, currentRT->GetWidth(), currentRT->GetHeight(), currentRT);
		}

		m_rootSign.Init(
			D3D12_FILTER_MIN_MAG_MIP_LINEAR,
			D3D12_TEXTURE_ADDRESS_MODE_WRAP,
			D3D12_TEXTURE_ADDRESS_MODE_WRAP,
			D3D12_TEXTURE_ADDRESS_MODE_WRAP
		);

		m_vsBloom.LoadVS(L"Assets/shader/Bloom.fx", "VSMain");
		m_psLuminance.LoadPS(L"Assets/shader/Bloom.fx", "PSMain_SamplingLuminance");
		m_psCombine.LoadPS(L"Assets/shader/Bloom.fx", "PSMain_Combine");
		{
			D3D12_INPUT_ELEMENT_DESC inputElementDescs[] =
			{
				{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
				{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
			};

			//パイプラインステートを作成。
			D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = { 0 };
			psoDesc.InputLayout = { inputElementDescs, _countof(inputElementDescs) };
			psoDesc.pRootSignature = m_rootSign.Get();
			psoDesc.VS = CD3DX12_SHADER_BYTECODE(m_vsBloom.GetCompiledBlob());
			psoDesc.PS = CD3DX12_SHADER_BYTECODE(m_psLuminance.GetCompiledBlob());
			psoDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
			psoDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
			psoDesc.DepthStencilState.DepthEnable = FALSE;
			psoDesc.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
			psoDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;
			psoDesc.DepthStencilState.StencilEnable = FALSE;
			psoDesc.SampleMask = UINT_MAX;
			psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
			psoDesc.NumRenderTargets = 3;
			psoDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;		//アルベドカラー出力用。
			m_pipeLuminance.Init(psoDesc);

			psoDesc.PS = CD3DX12_SHADER_BYTECODE(m_psCombine.GetCompiledBlob());
			m_pipeCombine.Init(psoDesc);
		}

		m_descLuminance.RegistShaderResource(0, *pe.GetCurrentBackBufferTexture());
		D3D12_SAMPLER_DESC samplerDesc = {};
		samplerDesc.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;
		samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		samplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		samplerDesc.MipLODBias = 0;
		samplerDesc.MaxAnisotropy = 1;
		samplerDesc.ComparisonFunc = D3D12_COMPARISON_FUNC_ALWAYS;
		samplerDesc.BorderColor[0] = 1.0f;
		samplerDesc.BorderColor[1] = 1.0f;
		samplerDesc.BorderColor[2] = 1.0f;
		samplerDesc.BorderColor[3] = 1.0f;
		samplerDesc.MinLOD = 0.0f;
		samplerDesc.MaxLOD = D3D12_FLOAT32_MAX;
		m_descLuminance.RegistSamplerDesc(0,samplerDesc);
		
		m_descLuminance.Commit();

		for (int i = 0; i < MAX_GAUSS; i++)
		{
			m_descCombine.RegistShaderResource(i, *m_blur[i].GetOutputTexture());
		}
		m_descCombine.Commit();
	}
	void Bloom::Render()
	{
	}
}