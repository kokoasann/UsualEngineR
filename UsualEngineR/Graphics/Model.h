#pragma once

#include "tkFile/TkmFile.h"
#include "MeshParts.h"
#include "Skeleton.h"

#include "IK/IK.h"


namespace UER
{
	
	class IShaderResource;
	class raytracing::Instance;

	enum EUpAxis
	{
		enUpAxisY,
		enUpAxisZ,
	};
	
	/// <summary>
	/// モデルの初期化データ
	/// </summary>
	struct ModelInitData {
		const char* m_tkmFilePath = nullptr;		//tkmファイルパス。
		const char* m_vsEntryPointFunc = "VSMain";	//頂点シェーダーのエントリーポイント。
		const char* m_psEntryPointFunc = "PSMain";	//ピクセルシェーダーのエントリーポイント。
		const char* m_vsfxFilePath = "Assets/shader/NoAnimModel.fx";			//.fxファイルのファイルパス。
		//const char* m_psfxFilePath = "Assets/shader/ModelPS.fx";
		const char* m_psfxFilePath = "Assets/shader/GBufferModelPS.fx";
		const char* m_tksFilePath = nullptr;		//tksファイルパス。
		void* m_expandConstantBuffer = nullptr;		//ユーザー拡張の定数バッファ。
		int m_expandConstantBufferSize = 0;			//ユーザー拡張の定数バッファのサイズ。
		IShaderResource* m_expandShaderResoruceView = nullptr;	//ユーザー拡張のシェーダーリソース。
		EUpAxis m_upAxis = enUpAxisZ;
		const wchar_t* m_emmissionMap = nullptr;		//エミッションマップ
	};

	
	/// <summary>
	/// モデルクラス。
	/// </summary>
	class Model {
	
	public:

		virtual ~Model();
		/// <summary>
		/// 
		/// </summary>
		void Release();
	
		/// <summary>
		/// tkmファイルから初期化。
		/// </summary>
		/// <param name="initData">初期化データ</param>
		void Init( const ModelInitData& initData );
		/// <summary>
		/// ワールド行列の更新。
		/// </summary>
		/// <param name="pos">座標</param>
		/// <param name="rot">回転</param>
		/// <param name="scale">拡大率</param>
		void UpdateWorldMatrix(const Vector3& pos, const Quaternion& rot, const Vector3& scale);

		void UpdateSkeleton();

		void UpdateIKTarget();
		void UpdateIK();
	
		/// <summary>
		/// スケルトンを関連付ける。
		/// </summary>
		/// <param name="skeleton">スケルトン</param>
		void BindSkeleton(Skeleton& skeleton)
		{
			m_meshParts.BindSkeleton(skeleton);
		}
		/// <summary>
		/// 描画
		/// </summary>
		/// <param name="renderContext">レンダリングコンテキスト</param>
		void Draw(RenderContext& renderContext, const Vector4& mulcolor);

		void DrawInstanced(RenderContext& renderContext, int instanceNum, const Vector4& mulcolor);

		void DrawShadow(RenderContext& rc);
		/// <summary>
		/// ワールド行列を取得。
		/// </summary>
		/// <returns></returns>
		const Matrix& GetWorldMatrix() const
		{
			return m_world;
		}
		/// <summary>
		/// メッシュに対して問い合わせを行う。
		/// </summary>
		/// <param name="queryFunc">問い合わせ関数</param>
		void QueryMeshs(std::function<void(const SMesh& mesh)> queryFunc)
		{
			m_meshParts.QueryMeshs(queryFunc);
		}

		/// <summary>
		/// tkmFileからメッシュに対して問い合わせを行う。
		/// </summary>
		/// <param name="queryFunc">問い合わせ関数</param>
		void QueryMeshsFromTkm(std::function<void(const TkmFile::SMesh& mesh)> queryFunc) const
		{
			m_tkmFile->QueryMeshParts(queryFunc);
		}

		/// <summary>
		/// 
		/// </summary>
		/// <param name="queryFunc"></param>
		void QueryMeshAndDescriptorHeap(std::function<void(const SMesh& mesh, const DescriptorHeap& ds)> queryFunc)
		{
			m_meshParts.QueryMeshAndDescriptorHeap(queryFunc);
		}

		Skeleton* GetSkelton()
		{
			return &m_skeleton;
		}

		EUpAxis GetUpAxis() const
		{
			return m_upAxis;
		}

		void AddRaytraceInstance(raytracing::Instance* instance)
		{
			m_raytracingInstanceList.push_back(instance);
		}

		/// <summary>
		/// IKを作成する
		/// 
		/// </summary>
		/// <param name="startBone"></param>
		/// <param name="endBone"></param>
		/// <param name="radius"></param>
		/// <returns></returns>
		IK* CreateIK(Bone* startBone, Bone* endBone, float radius);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="startBone"></param>
		/// <param name="len"></param>
		/// <param name="radius"></param>
		/// <returns></returns>
		IK* CreateIK(Bone* startBone, int len, float radius)
		{
			Bone* endBone = m_skeleton.GetBone(startBone->GetParentBoneNo());
			for (int i = 0; i < len-1; i++)
			{
				endBone = m_skeleton.GetBone(endBone->GetParentBoneNo());
			}
			return CreateIK(startBone, endBone, radius);
		}

	private:
	
		Matrix m_world;			//ワールド行列。
		TkmFile* m_tkmFile;		//tkmファイル。
		Skeleton m_skeleton;	//スケルトン。
		MeshParts m_meshParts;	//メッシュパーツ。

		EUpAxis m_upAxis = enUpAxisZ;
		std::vector<raytracing::Instance*> m_raytracingInstanceList;
		
		std::vector<IK*> m_ikList;
	};


}
