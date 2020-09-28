#pragma once
#include "Physics/ICollider.h"




namespace UER
{
	class SkinModel;
	/*!
	 * @brief	メッシュコライダー。
	 */
	class MeshCollider : public ICollider
	{
	public:
		/*!
		 * @brief	コンストラクタ。
		 */
		MeshCollider();
		/*!
		 * @brief	デストラクタ。
		 */
		~MeshCollider();

		/// <summary>
		/// リリース
		/// </summary>
		void Release();
		/*!
		 * @brief	CSkinModelからメッシュコライダーを生成。
		 *@param[in]	model		スキンモデル。
		 */
		void CreateFromSkinModel(const Model& model, const Matrix* offsetMatrix = nullptr);
		btCollisionShape* GetBody() const override
		{
			return m_meshShape.get();
		}
	private:
		typedef std::vector<Vector3>					VertexBuffer;		//頂点バッファ。
		typedef std::vector<unsigned int>				IndexBuffer;		//インデックスバッファ。
		typedef std::unique_ptr<VertexBuffer>			VertexBufferPtr;
		typedef std::unique_ptr<IndexBuffer>			IndexBufferPtr;
		std::vector<VertexBufferPtr>					m_vertexBufferArray;		//頂点バッファの配列。
		std::vector<IndexBufferPtr>						m_indexBufferArray;		//インデックスバッファの配列。
		std::unique_ptr<btBvhTriangleMeshShape>			m_meshShape;				//メッシュ形状。
		std::unique_ptr<btTriangleIndexVertexArray>		m_stridingMeshInterface;
	};

}