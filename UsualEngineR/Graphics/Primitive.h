#pragma once

#include "VertexBuffer.h"
#include "IndexBuffer.h"


namespace UER
{
	class Primitive
	{
	public:
		enum EIndexType
		{
			it_2byte = 2,
			it_4byte = 4,
		};
		Primitive() {}
		~Primitive() { Release(); }

		void Release();

		bool Cteate
		(
			D3D_PRIMITIVE_TOPOLOGY topo,
			int vertexNum,
			int stride,
			const void* vertexData,
			int indexNum,
			EIndexType indexType,
			const void* indexData
		);

		VertexBuffer& GetVertexBuffer()
		{
			return m_vertexBuffer;
		}
		IndexBuffer& GetIndexBuffer()
		{
			return m_indexBuffer;
		}

		int GetIndexCount()
		{
			return m_indexBuffer.GetCount();
		}

		/// <summary>
		/// vertexBuffer��indexBuffer
		/// Topology�̏��𑗂�
		/// </summary>
		/// <param name="rc"></param>
		/// <returns>indexBuffer�̐�</returns>
		int SendData(RenderContext& rc);
		void Draw(RenderContext& rc, int vertexnum);
		void Draw(RenderContext& rc);
	private:
		
		//void Draw(RenderContext* dc, int vertexnum);
	private:
		VertexBuffer m_vertexBuffer;		//���_�o�b�t�@
		IndexBuffer m_indexBuffer;		//�C���f�b�N�X�o�b�t�@

		D3D_PRIMITIVE_TOPOLOGY m_topology; //topology
	};
}