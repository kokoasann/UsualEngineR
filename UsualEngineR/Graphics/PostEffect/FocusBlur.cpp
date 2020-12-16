#include "PreCompile.h"
#include "FocusBlur.h"


namespace UER
{
	void FocusBlur::Init()
	{
		m_constBuff.Init(sizeof(BlurData));
		float cc[] = { 1,1,1,0 };
		m_rt.Create(FRAME_BUFFER_W, FRAME_BUFFER_H, 1, 1, DXGI_FORMAT_R32G32B32_FLOAT, DXGI_FORMAT_UNKNOWN, cc);

		m_rootSign.Init(
			D3D12_FILTER_MIN_MAG_MIP_LINEAR,
			D3D12_TEXTURE_ADDRESS_MODE_CLAMP,
			D3D12_TEXTURE_ADDRESS_MODE_CLAMP,
			D3D12_TEXTURE_ADDRESS_MODE_CLAMP
		);

		m_descHeap.RegistConstantBuffer(0, m_constBuff);
		m_descHeap.RegistShaderResource(0, g_graphicsEngine->GetCurrentRenderTarget()->GetRenderTargetTexture());
		m_descHeap.Commit();

		m_vs.LoadVS(L"Assets/shader/FocusBlur.fx", "VSMain");
		m_ps.LoadPS(L"Assets/shader/FocusBlur.fx", "PSMain");
	}
}