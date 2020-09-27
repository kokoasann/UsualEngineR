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

	void Model::Init(const ModelInitData& initData)
	{
		//std::lock_guard<std::mutex> lg(debugMute);
		
		Stopwatch ts;
		ts.Start();

		m_upAxis = initData.m_upAxis;

		//内部のシェーダーをロードする処理が求めているのが
		//wchar_t型の文字列なので、ここで変換しておく。
		wchar_t wfxFilePath[256];
		if (initData.m_fxFilePath == nullptr) {
			MessageBox(nullptr, L"fxファイルパスが指定されていません", L"エラー", MB_OK);
			std::abort();
		}
		mbstowcs(wfxFilePath, initData.m_fxFilePath, 256);
	

		
		Stopwatch sw;
		sw.Start();

		m_tkmFile = TkmFile::GetManager().Load(initData.m_tkmFilePath);
		//m_tkmFile.Load(initData.m_tkmFilePath);

		tkmtime.push_back(sw.Stop());

		sw.Start();
		if (initData.m_tksFilePath)
		{
			m_skeleton.Init(initData.m_tksFilePath);
			m_skeleton.BuildBoneMatrices();
			BindSkeleton(m_skeleton);
		}
		tkstime.push_back(sw.Stop());

		sw.Start();
		m_meshParts.InitFromTkmFile(
			*m_tkmFile, 
			wfxFilePath, 
			initData.m_vsEntryPointFunc,
			initData.m_psEntryPointFunc,
			initData.m_expandConstantBuffer,
			initData.m_expandConstantBufferSize,
			initData.m_expandShaderResoruceView
		);
		meshtime.push_back(sw.Stop());
		

		UpdateWorldMatrix(g_vec3Zero, g_quatIdentity, g_vec3One);


		totaltime.push_back(ts.Stop());
		

	}
	
	void Model::UpdateWorldMatrix(Vector3 pos, Quaternion rot, Vector3 scale)
	{
		Matrix mBias;
		if (m_upAxis == enUpAxisZ) {
			//Z-up
			mBias.MakeRotationX(Math::PI * -0.5f);
		}
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
