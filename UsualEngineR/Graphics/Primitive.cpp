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
		if(vertexData)
			m_vertexBuffer.Copy((void*)vertexData);

		if (indexNum || 1)
		{
			m_indexBuffer.Init(indexNum, indexType);
			m_indexBuffer.Copy((void*)indexData);
		}

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

	void Primitive::Draw(RenderContext& rc)
	{

		rc.SetPrimitiveTopology(m_topology);
		rc.SetIndexBuffer(m_indexBuffer);
		rc.SetVertexBuffer(m_vertexBuffer);
		rc.DrawIndexed(m_indexBuffer.GetCount());
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
	void Primitive::Draw(RenderContext& rc, int vertexnum)
	{
		//UINT ofs = 0;
		//UINT stride = m_vertexBuffer.GetStride();
		//rc->IASetVertexBuffers(0, 1, &m_vertexBuffer.GetBody(), &stride, &ofs);
		//rc->IASetPrimitiveTopology(m_topology);
		//rc->Draw(vertexnum, 0);

		rc.SetPrimitiveTopology(m_topology);
		rc.SetIndexBuffer(m_indexBuffer);
		rc.SetVertexBuffer(m_vertexBuffer);
		rc.DrawIndexed(vertexnum);
		
	}
}