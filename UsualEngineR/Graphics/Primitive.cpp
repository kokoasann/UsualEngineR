#include "PreCompile.h"
#include "Primitive.h"


namespace UER
{
	void Primitive::Release()
	{
		m_indexBuffer.Release();
		m_vertexBuffer.Release();
	}
	bool Primitive::Cteate(D3D_PRIMITIVE_TOPOLOGY topo, int vertexNum, int stride, const void* vertexData, int indexNum, EIndexType indexType, const void* indexData)
	{
		m_vertexBuffer.Init(vertexNum, stride);
		m_vertexBuffer.Copy((void*)vertexData);

		m_indexBuffer.Init(indexNum, indexType);
		m_indexBuffer.Copy((void*)indexData);

		m_topology = topo;
		return true;
	}

	int Primitive::SendData(RenderContext& rc)
	{
		rc.SetVertexBuffer(m_vertexBuffer);
		rc.SetIndexBuffer(m_indexBuffer);
		rc.SetPrimitiveTopology(m_topology);

		return m_indexBuffer.GetCount();
	}

	void Primitive::Draw()
	{
		/*ID3D11DeviceContext* dc = usualEngine()->GetGraphicsEngine()->GetD3DDeviceContext();
		UINT offset = 0;
		UINT stride = m_vertexBuffer.GetStride();
		dc->IASetVertexBuffers(0, 1, &m_vertexBuffer.GetBody(), &stride, &offset);

		dc->IASetIndexBuffer
		(
			m_indexBuffer.GetBody(), 
			m_indexBuffer.GetIndexType() == IndexBuffer::e16bit ? DXGI_FORMAT_R16_UINT: DXGI_FORMAT_R32_UINT,
			0
		);

		dc->IASetPrimitiveTopology(m_topology);

		dc->DrawIndexed(m_indexBuffer.GetIndexNum(), 0, 0);*/
	}
	void Primitive::Draw(RenderContext* dc, int vertexnum)
	{
		/*UINT ofs = 0;
		UINT stride = m_vertexBuffer.GetStride();
		dc->IASetVertexBuffers(0, 1, &m_vertexBuffer.GetBody(), &stride, &ofs);
		dc->IASetPrimitiveTopology(m_topology);
		dc->Draw(vertexnum, 0);*/
	}
}