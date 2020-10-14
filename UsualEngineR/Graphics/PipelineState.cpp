#include "PreCompile.h"
#include "PipelineState.h"


namespace UER
{
	
	void PipelineState::Init(const D3D12_GRAPHICS_PIPELINE_STATE_DESC& desc)
	{
		auto d3dDevice = g_graphicsEngine->GetD3DDevice();
		auto hr = d3dDevice->CreateGraphicsPipelineState(&desc, IID_PPV_ARGS(&m_pipelineState));

		if (FAILED(hr)) {
			MessageBoxA(nullptr, "PipelineState create failed パイプラインステートの作成に失敗しました。\n", "エラー", MB_OK);
			std::abort();
		}
	}
	void PipelineState::Init(const D3D12_COMPUTE_PIPELINE_STATE_DESC& desc)
	{
		auto d3dDevice = g_graphicsEngine->GetD3DDevice();
		auto hr = d3dDevice->CreateComputePipelineState(&desc, IID_PPV_ARGS(&m_pipelineState));
		if (FAILED(hr)) {
			MessageBoxA(nullptr, "PipelineState create failed パイプラインステートの作成に失敗しました。\n", "エラー", MB_OK);
			std::abort();
		}
	}


}
