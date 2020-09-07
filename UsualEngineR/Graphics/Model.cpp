#include "PreCompile.h"
#include "Model.h"


namespace UER
{
	
	
	void Model::Init(const ModelInitData& initData)
	{
		//内部のシェーダーをロードする処理が求めているのが
		//wchar_t型の文字列なので、ここで変換しておく。
		wchar_t wfxFilePath[256];
		if (initData.m_fxFilePath == nullptr) {
			MessageBox(nullptr, L"fxファイルパスが指定されていません", L"エラー", MB_OK);
			std::abort();
		}
		mbstowcs(wfxFilePath, initData.m_fxFilePath, 256);
	
		m_tkmFile.Load(initData.m_tkmFilePath);

		if (initData.m_tksFilePath)
		{
			m_skeleton.Init(initData.m_tksFilePath);
			m_skeleton.BuildBoneMatrices();
			BindSkeleton(m_skeleton);
		}

		m_meshParts.InitFromTkmFile(
			m_tkmFile, 
			wfxFilePath, 
			initData.m_vsEntryPointFunc,
			initData.m_psEntryPointFunc,
			initData.m_expandConstantBuffer,
			initData.m_expandConstantBufferSize,
			initData.m_expandShaderResoruceView
		);
	
		

		UpdateWorldMatrix(g_vec3Zero, g_quatIdentity, g_vec3One);
	}
	
	void Model::UpdateWorldMatrix(Vector3 pos, Quaternion rot, Vector3 scale)
	{
		Matrix mBias;
	//todo	if (enUpdateAxis == enFbxUpAxisZ) {
			//Z-up
			mBias.MakeRotationX(Math::PI * -0.5f);
	//	}
		Matrix mTrans, mRot, mScale;
		mTrans.MakeTranslation(pos);
		mRot.MakeRotationFromQuaternion(rot);
		mScale.MakeScaling(scale);
		m_world = mBias * mScale * mRot * mTrans;
	}
	void Model::Draw(RenderContext& rc)
	{
		m_meshParts.Draw(
			rc, 
			m_world, 
			g_camera3D->GetViewMatrix(), 
			g_camera3D->GetProjectionMatrix()
		);
	}


}
