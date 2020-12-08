#include "PreCompile.h"
#include "ShadowMap.h"


namespace UER
{
	ShadowMap::ShadowMap()
	{
	}
	ShadowMap::~ShadowMap()
	{
	}
	void ShadowMap::Init(float w, float h)
	{
		DXGI_SAMPLE_DESC multiSampleDesc;
		multiSampleDesc.Count = 1;
		multiSampleDesc.Quality = 0;
		int iw = w;
		int ih = h;
		
		for (int i = 0; i < MAX_SHADOW_MAP; i++)
		{
			float cc[4] = { 1,1,1,1 };
			m_shadowMapRT[i].Create(iw, ih, 1, 1, DXGI_FORMAT_R32_FLOAT, DXGI_FORMAT_D32_FLOAT, cc);
			

			m_shadowCBEntity.pixSize[i].x = 1.f / iw;
			m_shadowCBEntity.pixSize[i].y = 1.f / ih;
			iw >>= 1;
			ih >>= 1;
		}
		//m_shadowCB.Create(&m_shadowCBEntity, sizeof(m_shadowCBEntity));
		m_shadowCB.Init(sizeof(m_shadowCBEntity));
		
		m_shadowCBEntity.depthoffset.x = 0.0001f;
		m_shadowCBEntity.depthoffset.y = 0.0001f;
		m_shadowCBEntity.depthoffset.z = 0.0001f;

		m_lightHeight = 500.f;

		m_lightDirection = { -0.3f,-1,-0.2f };
		m_lightDirection.Normalize();

		for (int i = 0; i < MAX_SHADOW_MAP; i++)
		{
			m_constBufferLight[i].Init(sizeof(SConstBufferLight));
		}
	}
	void ShadowMap::Update()
	{
		auto dirlight = g_graphicsEngine->GetLightManager().GetMainLightDirection();
		if (dirlight != nullptr)
			m_lightDirection = dirlight->GetDir();
		else
			m_lightDirection = Vector3::Up;
		m_lightDirection.Normalize();

		Camera& MainCamera = *g_camera3D;
		//m_lightHeight = MainCamera.GetPosition().y + 500.f;
		//シーンをレンダリング使用としているカメラを使って、ライトカメラの回転を求める。
		Vector3 cameraDirXZ = MainCamera.GetForward();
		//cameraDirXZ.y = 0.f;
		cameraDirXZ.Normalize();
		if (fabs(cameraDirXZ.x) < FLT_EPSILON && fabsf(cameraDirXZ.z) < FLT_EPSILON) {
			//ほぼ真上をむいている。
			return;
		}
		/*cameraDirXZ.y = 0.0f;
		cameraDirXZ.Normalize();*/
		//ライトビュー行列の回転成分をを計算する。
		Vector3 lightViewForward = m_lightDirection;
		Vector3 lightViewUp;
		if (fabsf(lightViewForward.y) > 0.999f) {
			//ほぼ真上。
			lightViewUp.Cross(lightViewForward, Vector3::Right);
		}
		else {
			lightViewUp.Cross(lightViewForward, Vector3::Up);
		}
		lightViewUp.Normalize();
		Vector3 lgihtViewRight;
		lgihtViewRight.Cross(lightViewUp, lightViewForward);
		lgihtViewRight.Normalize();

		Matrix lightViewRot;
		//ライトビューの横を設定する。
		lightViewRot.m[0][0] = lgihtViewRight.x;
		lightViewRot.m[0][1] = lgihtViewRight.y;
		lightViewRot.m[0][2] = lgihtViewRight.z;
		lightViewRot.m[0][3] = 0.0f;
		//ライトビューの上を設定する。
		lightViewRot.m[1][0] = lightViewUp.x;
		lightViewRot.m[1][1] = lightViewUp.y;
		lightViewRot.m[1][2] = lightViewUp.z;
		lightViewRot.m[1][3] = 0.0f;
		//ライトビューの前を設定する。
		lightViewRot.m[2][0] = lightViewForward.x;
		lightViewRot.m[2][1] = lightViewForward.y;
		lightViewRot.m[2][2] = lightViewForward.z;
		lightViewRot.m[2][3] = 0.0f;

		//ライトビューのターゲットを計算。
		Vector3 lightTarget;
		lightTarget = MainCamera.GetPosition();
		lightTarget.y = MainCamera.GetTarget().y;
		//lightTarget += cameraDirXZ;				//?
		Vector3 lightPos = lightTarget + m_lightDirection * -m_lightHeight;

		float lightHeight = MainCamera.GetPosition().y + m_lightHeight;

		float nearPlaneZ = 0.0f;
		float farPlaneZ;
		Vector3 cameraUp;
		cameraUp.Cross(MainCamera.GetRight(), MainCamera.GetForward());

		//float shadowAriaTable[3] = { 1.f,10.0f,20.5f };
		float shadowAriaTable[3] = { 0.1f,0.5f,1.5f };
		//float shadowAriaTable[3] = { 0.4f,0.8f,1.6f };
		//float shadowPosTable[3] = { 1,0.5f,0.25f };
		//float offsetLen[3] = { 0.5,1,1.5 };

		//視推台を分割するようにライトビュープロジェクション行列を計算する。
		for (int i = 0; i < MAX_SHADOW_MAP; i++) {
			//Vector3 lightPos = lightTarget + m_lightDirection * -m_lightHeight;
			//farPlaneZ = nearPlaneZ + m_lightHeight * shadowPosTable[i];
			farPlaneZ = nearPlaneZ + lightHeight * shadowAriaTable[i];
			Matrix mLightView = Matrix::Identity;
			float halfViewAngle = MainCamera.GetViewAngle() * 0.5f;
			//視推台の8頂点をライト空間に変換してAABBを求めて、正射影の幅と高さを求める。
			float w, h;
			float fFar, fNear;
			Vector3 v[8];
			{
				//cameraDirXZ.y *= 0.f;

				float t = tan(halfViewAngle);
				Vector3 toUpperNear, toUpperFar;
				toUpperNear = cameraUp * t * nearPlaneZ;
				toUpperFar = cameraUp * t * farPlaneZ;
				t *= MainCamera.GetAspect();
				//近平面の中央座標を計算。
				Vector3 nearWK = MainCamera.GetPosition() + cameraDirXZ * nearPlaneZ;
				v[0] = nearWK + MainCamera.GetRight() * t * nearPlaneZ + toUpperNear;
				v[1] = v[0] - toUpperNear * 2.0f;

				v[2] = nearWK + MainCamera.GetRight() * -t * nearPlaneZ + toUpperNear;
				v[3] = v[2] - toUpperNear * 2.0f;

				//遠平面の中央座標を計算。
				Vector3 farWK = MainCamera.GetPosition() + cameraDirXZ * farPlaneZ;
				v[4] = farWK + MainCamera.GetRight() * t * farPlaneZ + toUpperFar;
				v[5] = v[4] - toUpperFar * 2.0f;
				v[6] = farWK + MainCamera.GetRight() * -t * farPlaneZ + toUpperFar;
				v[7] = v[6] - toUpperFar * 2.0f;

				//ライト行列を作成。
				
				//lightPos *= 0.5f;
				/*
				auto ligXZ = m_lightDirection;
				ligXZ.y = 0;
				ligXZ.Normalize();
				float cta = ligXZ.Dot({ 0,-1,0 });
				float camY = MainCamera.GetPosition().y;*/
				//lightPos += m_lightDirection * -m_lightHeight + ligXZ * -camY * cta;

				Vector3 half = (nearWK + farWK) * 0.5f;
				float alpha = (lightHeight - half.y) / m_lightDirection.y;
				Vector3 lightPos = half + m_lightDirection*alpha;

				mLightView = lightViewRot;
				mLightView.m[3][0] = lightPos.x;
				mLightView.m[3][1] = lightPos.y;
				mLightView.m[3][2] = lightPos.z;
				mLightView.m[3][3] = 1.0f;
				mLightView.Inverse(mLightView);	//ライトビュー完成。
				//視推台を構成する8頂点が計算できたので、ライト空間に座標変換して、AABBを求める。
				Vector3 vMax = { -FLT_MAX, -FLT_MAX, -FLT_MAX };
				Vector3 vMin = { FLT_MAX, FLT_MAX, FLT_MAX };
				for (auto& vInLight : v) {
					mLightView.Apply(vInLight);
					vMax.Max(vInLight);
					vMin.Min(vInLight);

				}
				w = vMax.x - vMin.x;
				h = vMax.y - vMin.y;
			}
			Matrix proj;
			proj.MakeOrthoProjectionMatrix(
				w,	//ちょい太らせる。
				h,
				m_near,
				m_far
			);
			m_mLVP[i].Multiply(mLightView, proj);
			m_shadowCBEntity.mLVP[i] = m_mLVP[i];

			//m_shadowCBEntity.ligFar[i] = m_far;
			//m_shadowCBEntity.ligNear[i] = m_near;

			nearPlaneZ = farPlaneZ;
		}
		m_shadowCB.CopyToVRAM(m_shadowCBEntity);
	}
	void ShadowMap::Send2GPU()
	{
		/*ID3D11DeviceContext* dc = usualEngine()->GetGraphicsEngine()->GetD3DDeviceContext();
		dc->UpdateSubresource(m_shadowCB.GetBody(), 0, NULL, (const SShadowCB*)& m_shadowCBEntity, 0, 0);
		dc->PSSetConstantBuffers(enSkinModelCBReg_Shadow, 1, &m_shadowCB.GetBody());
		for (int i = 0; i < MAX_SHADOW_MAP; i++)
		{
			dc->PSSetShaderResources(enSkinModelSRVReg_ShadowMap_1 + i, 1, &m_shadowMapRT[i].GetSRV());
		}*/
	}
	void ShadowMap::Render(RenderContext& rc)
	{
		/*ID3D11DeviceContext* dc = usualEngine()->GetGraphicsEngine()->GetD3DDeviceContext();
		for (int i = 0; i < MAX_SHADOW_MAP; i++)
		{
			ID3D11ShaderResourceView* none[] = { NULL };
			dc->PSSetShaderResources(enSkinModelSRVReg_ShadowMap_1 + i, 1, none);
		}*/

		RenderTarget* oldRenderTargets[8];
		int numRenderTargetViews;
		//rc.OMGetRenderTargets(numRenderTargetViews, oldRenderTargets);

		
		for (int i = 0; i < MAX_SHADOW_MAP; i++)
		{
			m_renderingShadowMapNum = i;

			rc.WaitUntilToPossibleSetRenderTarget(m_shadowMapRT[i]);
			m_constBufferLight[i].CopyToVRAM(m_mLVP[i]);
			

			RenderTarget* rts[MAX_SHADOW_MAP] = { &m_shadowMapRT[i] };
			rc.SetRenderTargets(1, rts);

			m_shadowMapRT[i].Clear(rc);

			D3D12_VIEWPORT vp = { 0.f, 0.f, m_shadowMapRT[i].GetWidth(), m_shadowMapRT[i].GetHeight(), D3D12_MIN_DEPTH, D3D12_MAX_DEPTH };
			rc.SetViewport(vp);

			D3D12_RECT rect;
			rect.left = 0;
			rect.top = 0;
			rect.right = m_shadowMapRT[i].GetWidth();
			rect.bottom = m_shadowMapRT[i].GetHeight();
			rc.SetScissorRect(rect);

			for (auto cast : m_shadowCaster)
			{
				cast->DrawShadow(rc);
			}

			rc.WaitUntilFinishDrawingToRenderTarget(m_shadowMapRT[i]);
			//g_graphicsEngine->ExecuteCommandList();
			//g_graphicsEngine->ResetRender();
		}
		m_shadowCaster.clear();

		//usualEngine()->GetGraphicsEngine()->OMSetRenderTarget(numRenderTargetViews, oldRenderTargets);
		m_viewPort.Width = FRAME_BUFFER_W;
		m_viewPort.Height = FRAME_BUFFER_H;
		m_viewPort.TopLeftX = 0.f;
		m_viewPort.TopLeftY = 0.f;
		m_viewPort.MaxDepth = 1.f;
		m_viewPort.MinDepth = 0.f;
		rc.SetViewport(m_viewPort);

		D3D12_RECT rect;
		rect.left = 0;
		rect.top = 0;
		rect.right = FRAME_BUFFER_W;
		rect.bottom = FRAME_BUFFER_H;
		rc.SetScissorRect(rect);
	}
}