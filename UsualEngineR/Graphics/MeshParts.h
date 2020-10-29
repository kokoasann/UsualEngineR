/// <summary>
/// メッシュパーツクラス。
/// </summary>

#pragma once

#include "tkFile/TkmFile.h"
#include "StructuredBuffer.h"


namespace UER
{
	
	class RenderContext;
	class Skeleton;
	class Material;
	class IShaderResource;
	class Camera;
	
	/// <summary>
	/// メッシュ
	/// </summary>
	struct SMesh {
		VertexBuffer m_vertexBuffer;						//頂点バッファ。
		std::vector< IndexBuffer* >		m_indexBufferArray;	//インデックスバッファ。
		std::vector< Material* >		m_materials;			//マテリアル。
		std::vector<int>				skinFlags;				//スキンを持っているかどうかのフラグ。
	};
	
	/// <summary>
	/// メッシュパーツ。
	/// </summary>
	class MeshParts {
	public:
		/// <summary>
		/// デストラクタ。
		/// </summary>
		~MeshParts();
		/// <summary>
		/// tkmファイルから初期化
		/// </summary>
		/// <param name="tkmFile">tkmファイル。</param>
		/// /// <param name="fxFilePath">fxファイルのファイルパス</param>
		/// <param name="vsEntryPointFunc">頂点シェーダーのエントリーポイントの関数名</param>
		/// <param name="psEntryPointFunc">ピクセルシェーダーのエントリーポイントの関数名</param>
		void InitFromTkmFile(
			const TkmFile& tkmFile,
			const wchar_t* vsfxFilePath,
			const wchar_t* psfxFilePath,
			const char* vsEntryPointFunc,
			const char* psEntryPointFunc,
			void* expandData,
			int expandDataSize,
			IShaderResource* expandShaderResourceView
		) ;
		/// <summary>
		/// 描画。
		/// </summary>
		/// <param name="rc">レンダリングコンテキスト</param>
		/// <param name="mWorld">ワールド行列</param>
		/// <param name="mView">ビュー行列</param>
		/// <param name="mProj">プロジェクション行列</param>
		/// <param name="light">ライト</param>
		void Draw(RenderContext& rc, const Matrix& mWorld, const Matrix& mView, const Matrix& mProj, const Vector4& mulcolor);
		/// <summary>
		/// スケルトンを関連付ける。
		/// </summary>
		/// <param name="skeleton">スケルトン</param>
		void BindSkeleton(Skeleton& skeleton) ;
		/// <summary>
		/// メッシュに対して問い合わせを行う。
		/// </summary>
		/// <param name="queryFunc">クエリ関数</param>
		void QueryMeshs(std::function<void(const SMesh& mesh)> queryFunc)
		{
			for (const auto& mesh : m_meshs) {
				queryFunc(*mesh);
			}
		}

		void QueryMeshAndDescriptorHeap(std::function<void(const SMesh& mesh, const DescriptorHeap& ds)> queryFunc)
		{
			for (int i = 0; i < m_meshs.size(); i++) {
				queryFunc(*m_meshs[i], m_descriptorHeap[i]);
			}
		}
	private:
		/// <summary>
		/// tkmメッシュからメッシュを作成。
		/// </summary>
		/// <param name="mesh">メッシュ</param>
		/// <param name="meshNo">メッシュ番号</param>
		/// <param name="fxFilePath">fxファイルのファイルパス</param>
		/// <param name="vsEntryPointFunc">頂点シェーダーのエントリーポイントの関数名</param>
		/// <param name="psEntryPointFunc">ピクセルシェーダーのエントリーポイントの関数名</param>
		void CreateMeshFromTkmMesh(
			const TkmFile::SMesh& mesh, 
			int meshNo,
			const wchar_t* vsfxFilePath,
			const wchar_t* psfxFilePath,
			const char* vsEntryPointFunc,
			const char* psEntryPointFunc );
	
		/// <summary>
		/// ディスクリプタヒープを作成。
		/// </summary>
		void CreateDescriptorHeaps();
	private:
		//拡張SRVが設定されるレジスタの開始番号。
		const static int EXPAND_SRV_REG__START_NO = 10;
		/// <summary>
		/// 定数バッファ。
		/// </summary>
		/// <remarks>
		/// この構造体を変更したら、SimpleModel.fxのCBも変更するように。
		/// </remarks>
		struct SConstantBuffer {
			Matrix mWorld;		//ワールド行列。
			Vector4 mulcolor;
			//Matrix mView;		//ビュー行列。
			//Matrix mProj;		//プロジェクション行列。
		};
		
		ConstantBuffer m_commonConstantBuffer;					//メッシュ共通の定数バッファ。
		//ConstantBuffer m_cameraConstantBuffer;
		//ConstantBuffer m_lightConstantBuffer;
		ConstantBuffer m_expandConstantBuffer;					//ユーザー拡張用の定数バッファ
		IShaderResource* m_expandShaderResourceView = nullptr;	//ユーザー拡張シェーダーリソースビュー。
		StructuredBuffer m_boneMatricesStructureBuffer;			//ボーン行列の構造化バッファ。
		std::vector< SMesh* > m_meshs;							//メッシュ。
		std::vector< DescriptorHeap > m_descriptorHeap;			//ディスクリプタヒープ。
		Skeleton* m_skeleton = nullptr;							//スケルトン。
		void* m_expandData = nullptr;							//ユーザー拡張データ。
	};

}
