#include "PreCompile.h"
#include "Model.h"


namespace UER
{
	static std::vector<double> tkmtime;
	static std::vector<double> tkstime;
	static std::vector<double> meshtime;
	static std::vector<double> totaltime;
	static int c=0;
	static std::mutex debugMute;

	void Model::Release()
	{
		/*for (auto ik : m_ikList)
		{
			delete ik;
		}*/
	}

	void Model::Init(const ModelInitData& initData)
	{
		//std::lock_guard<std::mutex> lg(debugMute);
		

		m_upAxis = initData.m_upAxis;

		//内部のシェーダーをロードする処理が求めているのが
		//wchar_t型の文字列なので、ここで変換しておく。
		wchar_t wfxFilePath[256];
		wchar_t wpsfxFilePath[256];
		if (initData.m_vsfxFilePath == nullptr || initData.m_psfxFilePath == nullptr) {
			MessageBox(nullptr, L"fxファイルパスが指定されていません", L"エラー", MB_OK);
			std::abort();
		}
		mbstowcs(wfxFilePath, initData.m_vsfxFilePath, 256);
		mbstowcs(wpsfxFilePath, initData.m_psfxFilePath, 256);
	
		m_tkmFile = TkmFile::GetManager().Load(initData.m_tkmFilePath);
		//m_tkmFile.Load(initData.m_tkmFilePath);

		
		if (initData.m_tksFilePath)
		{
			m_skeleton.Init(initData.m_tksFilePath);
			m_skeleton.BuildBoneMatrices();
			//m_skeleton.CalcBoneMatrixInRootBoneSpace();
			BindSkeleton(m_skeleton);
		}
		
		m_meshParts.InitFromTkmFile(
			*m_tkmFile, 
			wfxFilePath,
			wpsfxFilePath,
			initData.m_vsEntryPointFunc,
			initData.m_psEntryPointFunc,
			initData.m_expandConstantBuffer,
			initData.m_expandConstantBufferSize,
			initData.m_expandShaderResoruceView,
			initData.m_emmissionMap
		);

		UpdateWorldMatrix(g_vec3Zero, g_quatIdentity, g_vec3One);


	}
	
	void Model::UpdateWorldMatrix(const Vector3& pos, const Quaternion& rot, const Vector3& scale)
	{
		Matrix mBias;
		if (m_upAxis == enUpAxisZ) {
			//Z-up
			mBias.MakeRotationX(Math::PI * -0.5f);
		}
		/*Matrix mTrans, mRot, mScale;
		mTrans.MakeTranslation(pos);
		mRot.MakeRotationFromQuaternion(rot);
		mScale.MakeScaling(scale);
		m_world = mBias * mScale * mRot * mTrans;*/

		Matrix mW(pos, scale, rot);
		m_world = mBias * mW;
	}
	void Model::UpdateSkeleton()
	{
		m_skeleton.Update(m_world);
	}
	void Model::UpdateIKTarget()
	{
		for (auto ik : m_ikList)
		{
			ik->UpdateTarget(m_world);
		}
	}
	void Model::UpdateIK()
	{
		for (auto ik : m_ikList)
		{
			ik->UpdateTarget(m_world);
			ik->Update(m_world);
		}
	}
	void Model::Draw(RenderContext& rc, const Vector4& mulcolor)
	{
		m_meshParts.Draw(
			rc,
			m_world,
			g_camera3D->GetViewMatrix(),
			g_camera3D->GetProjectionMatrix(),
			mulcolor
		);
		
	}

	void Model::DrawShadow(RenderContext& rc)
	{
		m_meshParts.DrawShadow(rc, m_world);
	}

	IK* Model::CreateIK(Bone* startBone, Bone* endBone, float radius)
	{
		IK* ik = new IK(&m_skeleton, startBone, endBone, radius);
		m_ikList.push_back(ik);
		return ik;
	}


}
