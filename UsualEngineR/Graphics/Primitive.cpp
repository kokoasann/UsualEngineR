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
		m_vertexBuffer.Init(vertexNum* stride, stride);
		if(vertexData)
			m_vertexBuffer.Copy((void*)vertexData);

		if (indexNum || 1)
		{
			m_indexBuffer.Init(indexNum* indexType, indexType);
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

	void Primitive::Draw(RenderContext& rc) const
	{
		rc.SetIndexBuffer(m_indexBuffer);
		rc.SetVertexBuffer(m_vertexBuffer);
		rc.SetPrimitiveTopology(m_topology);
		rc.DrawIndexed(m_indexBuffer.GetCount());
	}
	void Primitive::Draw(RenderContext& rc, int vertexnum) const
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