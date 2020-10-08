#include "PreCompile.h"
#include "TextureCopy.h"



namespace UER
{
	void TextureCopy::Release()
	{
	}
	void TextureCopy::Init(Texture& srcTex, BlendMethod bm)
	{
		m_rootSign.Init(
			D3D12_FILTER_MIN_MAG_MIP_LINEAR,
			D3D12_TEXTURE_ADDRESS_MODE_WRAP,
			D3D12_TEXTURE_ADDRESS_MODE_WRAP,
			D3D12_TEXTURE_ADDRESS_MODE_WRAP
		);
		m_descHeap.RegistShaderResource(0, srcTex);
		m_descHeap.Commit();
	}
	void TextureCopy::Render(RenderContext& rc,RenderTarget& rt, const Primitive& prim)
	{
		rc.SetRenderTarget(rt.GetRTVCpuDescriptorHandle(), rt.GetDSVCpuDescriptorHandle());
	}
}