/*!
* @brief	キャラクタのコリジョンコントロール。
*/

#include "PreCompile.h"
#include "character/CharacterController.h"
#include "Physics/CollisionAttr.h"



namespace UER
{
	const float RAD_GROUND = 0.42f;
	const float RAD_WALL = 0.4;
	//衝突したときに呼ばれる関数オブジェクト(地面用)
	struct SweepResultGround : public btCollisionWorld::ConvexResultCallback
	{
		bool isHit = false;									//衝突フラグ。
		Vector3 hitPos = Vector3(0.0f, -FLT_MAX, 0.0f);	//衝突点。
		Vector3 startPos = g_vec3Zero;					//レイの始点。
		Vector3 hitNormal = g_vec3Zero;				//衝突点の法線。
		btCollisionObject* me = nullptr;					//自分自身。自分自身との衝突を除外するためのメンバ。
		float dist = FLT_MAX;								//衝突点までの距離。一番近い衝突点を求めるため。FLT_MAXは単精度の浮動小数点が取りうる最大の値。

		bool isHitWall = false;
		Vector3 wallNormal = g_vec3Zero;
		Vector3 wallHitPos = g_vec3One;
		float wallDist = FLT_MAX;

		int nonHitCollisionAttr = 0;
		
															//衝突したときに呼ばれるコールバック関数。
		virtual	btScalar	addSingleResult(btCollisionWorld::LocalConvexResult& convexResult, bool normalInWorldSpace)
		{
			if (convexResult.m_hitCollisionObject == me
				|| convexResult.m_hitCollisionObject->getUserIndex() & enCollisionAttr_Character
				|| convexResult.m_hitCollisionObject->getUserIndex() & enCollisionAttr_NonHit
				|| convexResult.m_hitCollisionObject->getUserIndex() & nonHitCollisionAttr
				) {
				//自分に衝突した。or キャラクタ属性のコリジョンと衝突した。
				return 0.0f;
			}
			//衝突点の法線を引っ張ってくる。
			Vector3 hitNormalTmp = *(Vector3*)&convexResult.m_hitNormalLocal;
			//上方向と法線のなす角度を求める。
			float angle = hitNormalTmp.Dot(g_vec3Up);
			angle = fabsf(acosf(angle));
			//if (angle <= Math::PI * RAD_GROUND && 0 	//地面の傾斜が54度より小さいので地面とみなす。
			if (angle < Math::PI * RAD_GROUND	//地面の傾斜が54度より小さいので地面とみなす。
				|| convexResult.m_hitCollisionObject->getUserIndex() & enCollisionAttr_Ground //もしくはコリジョン属性が地面と指定されている。
				) 
			{
				//衝突している。
				isHit = true;
				Vector3 hitPosTmp = *(Vector3*)&convexResult.m_hitPointLocal;
				//衝突点の距離を求める。。
				Vector3 vDist;
				vDist.Subtract(hitPosTmp, startPos);
				float distTmp = vDist.Length();
				if (dist > distTmp)
				{
					//この衝突点の方が近いので、最近傍の衝突点を更新する。
					hitPos = hitPosTmp;
					hitNormal = *(Vector3*)&convexResult.m_hitNormalLocal;
					dist = distTmp;
				}
			}
			if(angle >= Math::PI * 0.4
				//|| convexResult.m_hitCollisionObject->getUserIndex() & enCollisionAttr_Ground	//もしくはコリジョン属性が地面と指定されている。
				|| convexResult.m_hitCollisionObject->getUserIndex() & enCollisionAttr_Character	//もしくはコリジョン属性がキャラクタなので壁とみなす。
				)
			{
				//return 0.f;
				isHitWall = true;
				Vector3 hitPosTmp = *(Vector3*)&convexResult.m_hitPointLocal;
				//衝突点の距離を求める。。
				Vector3 vDist;
				vDist.Subtract(hitPosTmp, startPos);
				float distTmp = vDist.Length();
				if (wallDist > distTmp)
				{
					wallHitPos = hitPosTmp;
					wallNormal = *(Vector3*)&convexResult.m_hitNormalLocal;
					wallDist = distTmp;
				}
			}
			return 0.0f;
		}
	};
	//衝突したときに呼ばれる関数オブジェクト(壁用)
	struct SweepResultWall : public btCollisionWorld::ConvexResultCallback
	{
		bool isHit = false;						//衝突フラグ。
		Vector3 hitPos = g_vec3Zero;		//衝突点。
		Vector3 startPos = g_vec3Zero;		//レイの始点。
		float dist = FLT_MAX;					//衝突点までの距離。一番近い衝突点を求めるため。FLT_MAXは単精度の浮動小数点が取りうる最大の値。
		Vector3 hitNormal = g_vec3Zero;	//衝突点の法線。
		btCollisionObject* me = nullptr;		//自分自身。自分自身との衝突を除外するためのメンバ。

		bool isHitFloor = false;
		Vector3 floorNormal = g_vec3Zero;
		Vector3 floorHitPos = g_vec3One;
		float floorDist = FLT_MAX;
		int nonHitCollisionAttr = 0;
												//衝突したときに呼ばれるコールバック関数。
		virtual	btScalar	addSingleResult(btCollisionWorld::LocalConvexResult& convexResult, bool normalInWorldSpace)
		{
			if (convexResult.m_hitCollisionObject == me
				|| convexResult.m_hitCollisionObject->getUserIndex() & enCollisionAttr_NonHit
				|| convexResult.m_hitCollisionObject->getUserIndex() & nonHitCollisionAttr
				)
			{
				//自分に衝突した。or 地面に衝突した。
				return 0.0f;
			}
			//衝突点の法線を引っ張ってくる。
			Vector3 hitNormalTmp;
			hitNormalTmp.Set(convexResult.m_hitNormalLocal);
			//上方向と衝突点の法線のなす角度を求める。
			float cta = hitNormalTmp.Dot(g_vec3Up);
			float angle = fabsf(acosf(cta));

			//if (angle >= Math::PI * RAD_WALL && 1 		//地面の傾斜が54度以上なので壁とみなす。
			if (angle >= Math::PI * RAD_WALL 		//地面の傾斜が54度以上なので壁とみなす。
				|| convexResult.m_hitCollisionObject->getUserIndex() & enCollisionAttr_Character	//もしくはコリジョン属性がキャラクタなので壁とみなす。
				|| convexResult.m_hitCollisionObject->getUserIndex() & enCollisionAttr_Wall
				//|| convexResult.m_hitCollisionObject->getUserIndex() & enCollisionAttr_Ground
				)
			{
				isHit = true;
				Vector3 hitPosTmp;
				hitPosTmp.Set(convexResult.m_hitPointLocal);
				//交点との距離を調べる。
				Vector3 vDist;
				vDist.Subtract(hitPosTmp, startPos);
				vDist.y = 0.0f;
				float distTmp = vDist.Length();
				if (distTmp < dist)
				{
					//この衝突点の方が近いので、最近傍の衝突点を更新する。
					hitPos = hitPosTmp;
					dist = distTmp;
					hitNormal = hitNormalTmp;
				}
			}
			if(angle <= Math::PI * RAD_GROUND
				|| convexResult.m_hitCollisionObject->getUserIndex() & enCollisionAttr_Ground
				)
			{
				isHitFloor = true;

				Vector3 hitPosTmp = *(Vector3*)&convexResult.m_hitPointLocal;
				//衝突点の距離を求める。。
				Vector3 vDist;
				vDist.Subtract(hitPosTmp, startPos);
				vDist.y = 0.0f;
				float distTmp = vDist.Length();
				if (floorDist > distTmp)
				{
					floorHitPos = hitPosTmp;
					floorNormal = hitNormalTmp;
					floorDist = distTmp;
				}
			}
			return 0.0f;
		}
	};

	struct ContactTestResult :public btCollisionWorld::ContactResultCallback
	{
		bool isHit = false;
		Vector3 hitNormal = Vector3::Zero;
		Vector3 hitPos = Vector3::Zero;
		float dist = 0;

		int nonHitCollisionAttr = 0;

		// ContactResultCallback を介して継承されました
		virtual btScalar addSingleResult(btManifoldPoint& cp,
			const btCollisionObjectWrapper* colObj0Wrap, int partId0, int index0,
			const btCollisionObjectWrapper* colObj1Wrap, int partId1, int index1) override
		{
			int ind = colObj1Wrap->getCollisionObject()->getUserIndex();
			if (ind & enCollisionAttr_NonHit
				|| ind & nonHitCollisionAttr
				)
				return 0.f;

			
			if (cp.getDistance() < dist)
			{
				isHit = true;
				hitNormal = cp.m_normalWorldOnB;
				hitPos = cp.getPositionWorldOnB();
				dist = cp.getDistance();
			}


			return 0.f;
		}
	};


	void CharacterController::Init(float radius, float height, const Vector3& position,bool isUseRigidBody)
	{
		m_position = position;
		//コリジョン作成。
		m_radius = radius;
		m_height = height;
		m_collider.Create(radius, height);
		//m_collider.Create(radius);
		//isUseRigidBody = true;
		if (isUseRigidBody)
		{
			//剛体を初期化。
			RigidBodyInfo rbInfo;
			rbInfo.collider = &m_collider;
			rbInfo.mass = 0.0f;
			m_rigidBody.Create(rbInfo);
			btTransform& trans = m_rigidBody.GetBody()->getWorldTransform();
			//剛体の位置を更新。
			trans.setOrigin(btVector3(position.x, position.y, position.z));
			//todo: 未対応。trans.setRotation(btQuaternion(rotation.x, rotation.y, rotation.z));
			m_rigidBody.GetBody()->setUserIndex(enCollisionAttr_Character);
			m_rigidBody.GetBody()->setCollisionFlags(btCollisionObject::CF_CHARACTER_OBJECT);
			Physics().AddRigidBody(m_rigidBody);
		}
		m_isUseRigidBody = isUseRigidBody;
	}
	static bool isDebugPrinted = true;
	const Vector3& CharacterController::Execute(float deltaTime, const Vector3& moveSpeed)
	{
		isDebugPrinted = false;
		if (std::isnan(moveSpeed.x) || std::isnan(moveSpeed.y) || std::isnan(moveSpeed.z) || std::isinf(moveSpeed.x))
		{
			DebugPrintLineConsole(TO_INT(EDebugConsoleKind::master), "nextPos is Nan !!!!");
		}

		if (moveSpeed.y > 0.0f) {
			//吹っ飛び中にする。
			m_isJump = true;
			m_isOnGround = false;
		}
		//次の移動先となる座標を計算する。
		Vector3 nextPosition = m_position;
		//速度からこのフレームでの移動量を求める。オイラー積分。
		Vector3 addPos = moveSpeed;
		addPos *= deltaTime;
		nextPosition += addPos;

		Vector3 addV = addPos;
		addV.Normalize();

		//const float OFFSETXZ = Vector3(addPos.x, 0, addPos.z).Length()*0.5f*0+0.1;
		//const float OFFSETY = fabsf(addPos.y)*0.5f*0 + 0.1;

		m_offsetXZ = Vector3(addPos.x, 0, addPos.z).Length() * 0.1 + m_radius;
		//m_offsetY = m_radius;

		Vector3 oldnextPos = m_position;

		float Ypos = 0.f;

		Vector3 originalXZDir = addPos;
		originalXZDir.y = 0.0f;
		originalXZDir.Normalize();
		//XZ平面での衝突検出と衝突解決を行う。
#if 0
		{
			bool isHitFloor = false;
			Vector3 floorPos;
			Vector3 floorNor;


			int loopCount = 0;
			while (true) {
				//現在の座標から次の移動先へ向かうベクトルを求める。
				Vector3 addPos;
				addPos.Subtract(nextPosition, m_position);
				Vector3 addPosXZ = addPos;
				addPosXZ.y = 0.0f;
				if (addPosXZ.Length() < FLT_EPSILON) {
					//XZ平面で動きがないので調べる必要なし。
					//FLT_EPSILONは1より大きい、最小の値との差分を表す定数。
					//とても小さい値のことです。
					break;
				}
				//カプセルコライダーの中心座標 + 高さ*0.1の座標をposTmpに求める。
				Vector3 posTmp = m_position;
				if (!isHitFloor)
					posTmp.y += m_height * 0.5f + m_radius;// +0.1f;
				else
					posTmp.y = Ypos + m_height * 0.5f + m_radius;// +0.1f;

				//posTmp.y += m_height + m_radius + m_height * 0.1f;
				//レイを作成。
				btTransform start, end;
				start.setIdentity();
				end.setIdentity();
				//始点はカプセルコライダーの中心座標 + 0.2の座標をposTmpに求める。
				start.setOrigin(btVector3(posTmp.x, posTmp.y, posTmp.z));
				//終点は次の移動先。XZ平面での衝突を調べるので、yはposTmp.yを設定する。
				end.setOrigin(btVector3(nextPosition.x, posTmp.y, nextPosition.z));

				SweepResultWall callback;
				if(m_isUseRigidBody)
					callback.me = m_rigidBody.GetBody();
				callback.startPos = posTmp;
				//衝突検出。
				Physics().ConvexSweepTest((const btConvexShape*)m_collider.GetBody(), start, end, callback);

				if (callback.isHitFloor)
				{
					isHitFloor = true;
					floorPos = callback.floorHitPos;
					floorNor = callback.floorNormal;

					Vector3 vT0, vT1;
					//XZ平面上での移動後の座標をvT0に、交点の座標をvT1に設定する。
					vT0.Set(nextPosition.x, 0.0f, nextPosition.z);
					vT1.Set(floorPos.x, 0.0f, floorPos.z);
					//めり込みが発生している移動ベクトルを求める。
					Vector3 vMerikomi;
					vMerikomi = vT0 - vT1;

					Vector3 vMeriNorm = vMerikomi;
					vMeriNorm.Normalize();

					Vector3 Z, S;
					Z.Cross(vMeriNorm, floorNor);
					S.Cross(floorNor, Z);

					float t = vMerikomi.Dot(S);
					Vector3 Yup = (S * t - vMerikomi) * vMerikomi.Length() / t;
					//Yup = Yup - vMerikomi;

					if (Yup.y > 0.f)
					{
						//nextPosition.y += Yup.y;
						//Ypos = nextPosition.y;
						Ypos = Yup.y;
						DebugPrintLineConsole(TO_INT(EDebugConsoleKind::master), "HIT FLOOR IN  WALL");
						//DebugPrintVector3(EDebugConsoleKind::master, floorNor);
						DebugPrintValue(EDebugConsoleKind::master, "Ypos", Ypos);
					}
				}

				if (callback.isHit) {
					//当たった。
					//壁。
					Vector3 vT0, vT1;
					//XZ平面上での移動後の座標をvT0に、交点の座標をvT1に設定する。
					vT0.Set(nextPosition.x, 0.0f, nextPosition.z);
					vT1.Set(callback.hitPos.x, 0.0f, callback.hitPos.z);
					//めり込みが発生している移動ベクトルを求める。
					Vector3 vMerikomi;
					vMerikomi = vT0 - vT1;
					//XZ平面での衝突した壁の法線を求める。。
					Vector3 hitNormalXZ = callback.hitNormal;
					hitNormalXZ.y = 0.0f;
					hitNormalXZ.Normalize();
					//めり込みベクトルを壁の法線に射影する。
					float fT0 = hitNormalXZ.Dot(vMerikomi);
					//押し戻し返すベクトルを求める。
					//押し返すベクトルは壁の法線に射影されためり込みベクトル+半径。
					Vector3 vOffset;
					vOffset = hitNormalXZ;
					vOffset *= -fT0 + m_radius + OFFSETXZ;
					nextPosition += vOffset;

					//Yの値は入ってないよ!
					/*if (vOffset.y > 0.f || vOffset.y < 0.f)
					{
						printf("BIG Y");
					}*/

					Vector3 currentDir;
					currentDir = nextPosition - m_position;
					currentDir.y = 0.0f;
					currentDir.Normalize();

					DebugPrintLineConsole(TO_INT(EDebugConsoleKind::master), "HIT WALL");
					DebugPrintVector3(EDebugConsoleKind::master, nextPosition);
					if (currentDir.Dot(originalXZDir) < 0.0f) {
						//角に入った時のキャラクタの振動を防止するために、
						//移動先が逆向きになったら移動をキャンセルする。
						nextPosition.x = m_position.x;
						nextPosition.z = m_position.z;

						break;
					}
					/*char st[255] = { 0 };
					sprintf_s(st, "nor: x:%.4f y:%.4f z:%.4f\n", callback.hitNormal.x, callback.hitNormal.y, callback.hitNormal.z);
					OutputDebugString(st);
					sprintf_s(st, "hip: x:%.4f y:%.4f z:%.4f\n", callback.hitPos.x, callback.hitPos.y, callback.hitPos.z);
					OutputDebugString(st);
					sprintf_s(st, "pos: x:%.4f y:%.4f z:%.4f\n", nextPosition.x, nextPosition.y, nextPosition.z);
					OutputDebugString(st);*/
				}
				else {
					//どことも当たらないので終わり。
					break;
				}
				/*float len = (oldnextPos - nextPosition).Length();
				if (len < 0.0001)
					break;
				oldnextPos = nextPosition;*/

				loopCount++;
				if (loopCount >= 10) {
					
					break;
				}
			}
		}
#endif
		ExecuteWall(m_position, nextPosition, originalXZDir, Ypos);

		//XZの移動は確定。
		m_position.x = nextPosition.x;
		m_position.z = nextPosition.z;


		//下方向を調べる。
#if 0
		//if(false)
		{
			Vector3 addPos;
			addPos.Subtract(nextPosition, m_position);

			//移動の仮確定。
			//m_position.x = nextPosition.x;	
			//m_position.z = nextPosition.z;

										//レイを作成する。
			btTransform start, end;
			start.setIdentity();
			end.setIdentity();
			//始点はカプセルコライダーの中心。
			start.setOrigin(btVector3(m_position.x, (m_position.y + m_height * 0.5f + m_radius)+ Ypos, m_position.z));
			//start.setOrigin(btVector3(m_position.x, m_position.y + m_height * 0.5f + m_radius + 0.1f, m_position.z));


			bool isFall = false;
			//終点は地面上にいない場合は1m下を見る。
			//地面上にいなくてジャンプで上昇中の場合は上昇量の0.01倍下を見る。
			//地面上にいなくて降下中の場合はそのまま落下先を調べる。
			Vector3 endPos;
			//endPos.Set(start.getOrigin());
			endPos.Set(nextPosition);
			if (m_isOnGround == false) {
				if (addPos.y > 0.0f) {
					//ジャンプ中とかで上昇中。
					//上昇中でもXZに移動した結果めり込んでいる可能性があるので下を調べる。
					endPos.y -= addPos.y * 0.01f;
				}
				else {
					//落下している場合はそのまま下を調べる。
					if (addPos.y < 0.f)
					{
						isFall = true;
						endPos.y += addPos.y;
					}
				}
			}
			else {
				//地面上にいない場合は1m下を見る。
				//endPos.y -= 1.0f;
				endPos.y += addPos.y;
			}
			end.setOrigin(btVector3(endPos.x, endPos.y, endPos.z));
			SweepResultGround callback;
			if (m_isUseRigidBody)
				callback.me = m_rigidBody.GetBody();
			callback.startPos.Set(start.getOrigin());
			//衝突検出。
			if(fabsf(endPos.y - callback.startPos.y) > FLT_EPSILON){
				Physics().ConvexSweepTest((const btConvexShape*)m_collider.GetBody(), start, end, callback);
				if (callback.isHit) {
					//当たった。
					//moveSpeed.y = 0.0f;
					m_isJump = false;
					m_isOnGround = true;
					nextPosition.y = callback.hitPos.y + OFFSETY;// + m_height * 0.5f + m_radius;//+m_height * 0.1f;
					//nextPosition = callback.hitPos;
					DebugPrintLineConsole(TO_INT(EDebugConsoleKind::master), "HIT FLOOR");
					DebugPrintVector3(EDebugConsoleKind::master, nextPosition);

					//printf("HIT FLOOR\n");
				}
				if (callback.isHitWall && isFall)	
				{
					Vector3 vT0, vT1;
					//XZ平面上での移動後の座標をvT0に、交点の座標をvT1に設定する。
					vT0.Set(nextPosition.x, 0.0f, nextPosition.z);
					vT1.Set(callback.wallHitPos.x, 0.0f, callback.wallHitPos.z);
					//めり込みが発生している移動ベクトルを求める。
					Vector3 vMerikomi;
					vMerikomi = vT0 - vT1;
					//XZ平面での衝突した壁の法線を求める。。
					Vector3 hitNormalXZ = callback.wallNormal;
					hitNormalXZ.y = 0.0f;
					hitNormalXZ.Normalize();
					//めり込みベクトルを壁の法線に射影する。
					float fT0 = hitNormalXZ.Dot(vMerikomi);
					//押し戻し返すベクトルを求める。
					//押し返すベクトルは壁の法線に射影されためり込みベクトル+半径。
					Vector3 vOffset;
					vOffset = hitNormalXZ;
					vOffset *= -fT0 + m_radius + OFFSETXZ;
					nextPosition += hitNormalXZ * (m_radius + OFFSETXZ);
					nextPosition.y = callback.wallHitPos.y;
					
					DebugPrintLineConsole(TO_INT(EDebugConsoleKind::master), "HIT WALL IN FLOOR");
				}
				if(!(callback.isHit || callback.isHitWall))
				{
					//地面上にいない。
					m_isOnGround = false;

				}
			}
		}
#endif
		{
			btRigidBody* btBody = m_rigidBody.GetBody();
			//剛体を動かす。
			btBody->setActivationState(DISABLE_DEACTIVATION);
			btTransform& trans = btBody->getWorldTransform();
			//剛体の位置を更新。
			trans.setOrigin(btVector3(m_position.x, m_position.y + m_height * 0.5f + m_radius, m_position.z));
			ExebuteContactTest(addV);
		}

		ExecuteFloor(m_position, nextPosition, originalXZDir, Ypos);

		if (std::isnan(nextPosition.x) || std::isnan(nextPosition.y) || std::isnan(nextPosition.z) || std::isinf(nextPosition.x))
		{
			DebugPrintLineConsole(TO_INT(EDebugConsoleKind::master), "nextPos is Nan !!!!");
		}

#if DEBUG_FUNC
		//auto v = m_position - nextPosition;
		//if (isDebugPrinted)
			//DebugPrintLineConsole(TO_INT(EDebugConsoleKind::master), "END EXECUTE\n");

#endif

		//移動確定。
		m_position = nextPosition;
		//m_position.y += 0.1f;
		if (m_isUseRigidBody)
		{
			btRigidBody* btBody = m_rigidBody.GetBody();
			//剛体を動かす。
			btBody->setActivationState(DISABLE_DEACTIVATION);
			btTransform& trans = btBody->getWorldTransform();
			//剛体の位置を更新。
			trans.setOrigin(btVector3(m_position.x, m_position.y + m_height * 0.5f + m_radius, m_position.z));
			//@todo 未対応。 trans.setRotation(btQuaternion(rotation.x, rotation.y, rotation.z));
		}

		ExebuteContactTest(addV);

		return m_position;
	}
	void CharacterController::ExecuteWall(Vector3& nowPos, Vector3& nextPos, const Vector3& originalXZDir, float& Ypos)
	{
		//const float OFFSETXZ = m_radius;

		bool isHitFloor = false;
		Vector3 floorPos;
		Vector3 floorNor;

		int loopCount = 0;
		while (true) {
			//現在の座標から次の移動先へ向かうベクトルを求める。
			Vector3 addPos;
			addPos.Subtract(nextPos, nowPos);
			Vector3 addPosXZ = addPos;
			addPosXZ.y = 0.0f;
			if (addPosXZ.Length() < FLT_EPSILON) {
				//XZ平面で動きがないので調べる必要なし。
				//FLT_EPSILONは1より大きい、最小の値との差分を表す定数。
				//とても小さい値のことです。
				break;
			}
			//カプセルコライダーの中心座標 + 高さ*0.1の座標をposTmpに求める。
			Vector3 posTmp = nowPos;
			if (!isHitFloor)
				posTmp.y += m_height * 0.5f + m_radius;// +0.1f;
			else
				posTmp.y += m_height * 0.5f + m_radius;// +0.1f;

			//posTmp.y += m_height + m_radius + m_height * 0.1f;
			//レイを作成。
			btTransform start, end;
			start.setIdentity();
			end.setIdentity();
			//始点はカプセルコライダーの中心座標 + 0.2の座標をposTmpに求める。
			start.setOrigin(btVector3(posTmp.x, posTmp.y, posTmp.z));
			//終点は次の移動先。XZ平面での衝突を調べるので、yはposTmp.yを設定する。
			end.setOrigin(btVector3(nextPos.x, posTmp.y, nextPos.z));

			SweepResultWall callback;
			if (m_isUseRigidBody)
				callback.me = m_rigidBody.GetBody();
			callback.startPos = posTmp;
			callback.nonHitCollisionAttr = m_nonHitCollisionAttr;
			//衝突検出。
			Physics().ConvexSweepTest((const btConvexShape*)m_collider.GetBody(), start, end, callback);

			float f2wR = acosf(callback.floorNormal.Dot(callback.hitNormal));

			bool isNearHitWall = callback.dist <= callback.floorDist;
			//isNearHitWall |= f2wR < Math::PI * 0.05f;
			//isNearHitWall = true;
			isNearHitWall = callback.isHit;
			//if (!isNearHitWall && callback.isHitFloor && 1)
			if (!isNearHitWall && callback.isHitFloor)
			{
#if 0
				isHitFloor = true;
				floorPos = callback.floorHitPos;
				floorNor = callback.floorNormal;

				Vector3 vT0, vT1;
				//XZ平面上での移動後の座標をvT0に、交点の座標をvT1に設定する。
				vT0.Set(nextPos.x, 0.0f, nextPos.z);
				vT1.Set(floorPos.x, 0.0f, floorPos.z);
				//めり込みが発生している移動ベクトルを求める。
				Vector3 vMerikomi;
				vMerikomi = vT0 - vT1;

				Vector3 vMeriNorm = vMerikomi;
				vMeriNorm.Normalize();

				Vector3 Z, S;
				Z.Cross(vMeriNorm, floorNor);
				Z.Normalize();
				S.Cross(floorNor, Z);

				if (S.y < 0.f)
				{
					S *= -1.f;
				}
				S.Normalize();

				float t = vMerikomi.Dot(S);
				Vector3 Yup = (S * t - vMerikomi);// *vMerikomi.Length() / t;
				//Yup = Yup - vMerikomi;

				if (Yup.y > 0.f)
				{
					//nextPos.y += Yup.y;
					//Ypos = nextPosition.y;
					Ypos = Yup.Length()+m_offsetY;
					DebugPrintLineConsole(TO_INT(EDebugConsoleKind::master), "HIT FLOOR IN WALL");
					DebugPrintVector3(EDebugConsoleKind::master, vMerikomi);
					DebugPrintVector3(EDebugConsoleKind::master, Z);
					DebugPrintVector3(EDebugConsoleKind::master, S);
					DebugPrintVector3(EDebugConsoleKind::master, Yup);
					DebugPrintValue(EDebugConsoleKind::master, "Ypos", Ypos);
					DebugPrintValue(EDebugConsoleKind::master, "t", t);
				}
				else
				{
					DebugPrintLineConsole(TO_INT(EDebugConsoleKind::master), "HIT FLOOR IN  WALL BAD NORMAL");
					DebugPrintVector3(EDebugConsoleKind::master, vMerikomi);
					DebugPrintVector3(EDebugConsoleKind::master, Z);
					DebugPrintVector3(EDebugConsoleKind::master, S);
				}
#elif 0
				
				Ypos = callback.floorHitPos.y;
				if (0.f < callback.floorHitPos.y - nowPos.y)
				{
					//nowPos.y = Ypos;
					isHitFloor = true;
					//DebugPrintLineConsole(TO_INT(EDebugConsoleKind::master), "HIT FLOOR IN WALL !!!!");
					//DebugPrintValue(TO_INT(EDebugConsoleKind::master), "Ypos", Ypos);
					//DebugPrintLineConsole(TO_INT(EDebugConsoleKind::master), "");
					isDebugPrinted = true;
				}
				//DebugPrintVector3(EDebugConsoleKind::master, callback.hitNormal);
				//DebugPrintVector3(EDebugConsoleKind::master, callback.floorNormal);

				if (std::isnan(nextPos.x) || std::isnan(nextPos.y) || std::isnan(nextPos.z) || std::isinf(nextPos.x))
				{
					//DebugPrintLineConsole(TO_INT(EDebugConsoleKind::master), "nextPos is Nan !!!!");
				}
				
				/*
				DebugPrintValue(EDebugConsoleKind::master, "fT0", fT0);
				DebugPrintValue(EDebugConsoleKind::master, "m_radius", m_radius);
				DebugPrintValue(EDebugConsoleKind::master,"-fT0 + m_radius", -fT0 + m_radius);*/
#endif
				Vector3 vT0, vT1;
				//XZ平面上での移動後の座標をvT0に、交点の座標をvT1に設定する。
				//vT0.Set(nextPos.x, 0.0f, nextPos.z);
				//vT1.Set(callback.floorHitPos.x, 0.0f, callback.floorHitPos.z);
				vT0 = nextPos;
				vT0.y = nowPos.y+ m_height * 0.5f + m_radius;
				vT1 = callback.floorHitPos;
				//めり込みが発生している移動ベクトルを求める。
				Vector3 vMerikomi;
				vMerikomi = vT0 - vT1;
				//XZ平面での衝突した壁の法線を求める。。
				Vector3 hitNormalXZ = callback.floorNormal;
				//hitNormalXZ.y = 0.0f;
				//hitNormalXZ.Normalize();
				//めり込みベクトルを壁の法線に射影する。
				float fT0 = hitNormalXZ.Dot(vMerikomi);
				//押し戻し返すベクトルを求める。
				//押し返すベクトルは壁の法線に射影されためり込みベクトル+半径。
				Vector3 vOffset;
				vOffset = hitNormalXZ;
				vOffset *= -fT0 + m_radius;
				nextPos += vOffset;
			}

			if (isNearHitWall && callback.isHit)
			//if (callback.isHit)
			{
#if 1
				//当たった。
				//壁。
				Vector3 vT0, vT1;
				//XZ平面上での移動後の座標をvT0に、交点の座標をvT1に設定する。
				vT0.Set(nextPos.x, 0.0f, nextPos.z);
				vT1.Set(callback.hitPos.x, 0.0f, callback.hitPos.z);
				//めり込みが発生している移動ベクトルを求める。
				Vector3 vMerikomi;
				vMerikomi = vT0 - vT1;
				//XZ平面での衝突した壁の法線を求める。。
				Vector3 hitNormalXZ = callback.hitNormal;
				hitNormalXZ.y = 0.0f;
				hitNormalXZ.Normalize();
				//めり込みベクトルを壁の法線に射影する。
				float fT0 = hitNormalXZ.Dot(vMerikomi);
				//押し戻し返すベクトルを求める。
				//押し返すベクトルは壁の法線に射影されためり込みベクトル+半径。
				Vector3 vOffset;
				vOffset = hitNormalXZ;
				vOffset *= -fT0 + m_radius + m_offsetXZ;
				nextPos += vOffset;

				//Yの値は入ってないよ!
				/*if (vOffset.y > 0.f || vOffset.y < 0.f)
				{
					printf("BIG Y");
				}*/
#endif

				Vector3 currentDir;
				currentDir = nextPos - nowPos;
				currentDir.y = 0.0f;
				currentDir.Normalize();

				if (std::isnan(nextPos.x) || std::isnan(nextPos.y) || std::isnan(nextPos.z) || std::isinf(nextPos.x))
				{
					//DebugPrintLineConsole(TO_INT(EDebugConsoleKind::master), "nextPos is Nan !!!!");
				}

				//DebugPrintLineConsole(TO_INT(EDebugConsoleKind::master), "HIT WALL");
				isDebugPrinted = true;
				//DebugPrintVector3(EDebugConsoleKind::master, nextPos);
				if (currentDir.Dot(originalXZDir) < 0.0f) {
					//角に入った時のキャラクタの振動を防止するために、
					//移動先が逆向きになったら移動をキャンセルする。
					nextPos.x = nowPos.x;
					nextPos.z = nowPos.z;
					//nextPos.y = nowPos.y;
					DebugPrintLineConsole(TO_INT(EDebugConsoleKind::master), "KADOKAWA");
					//if (isHitFloor)
						//nowPos.y = Ypos;

					break;
				}
			}
#if 0
			if (!callback.isHit && callback.isHitFloor && 1)
			{
				float dt = 1. - (max(-1., min(1., Dot(g_vec3Up, callback.floorNormal))) + 1.f) * 0.5f;
				dt = 1. - min(0.14, dt) * 5.;

				nextPos = nowPos + addPosXZ* dt;
				DebugPrintLineConsole(TO_INT(EDebugConsoleKind::master), "HIT WALL IN CONTROL FLOOR");
				DebugPrintValue(TO_INT(EDebugConsoleKind::master),"FLOOR DT", dt);
				DebugPrintVector3(TO_INT(EDebugConsoleKind::master), callback.floorNormal);
				DebugPrintLineConsole(TO_INT(EDebugConsoleKind::master), "");
				loopCount++;
				if (loopCount >= 5) 
				{
					break;
				}
				continue;
			}
#endif
			//if(!(callback.isHit || callback.isHitFloor))
			//if(!callback.isHit)
			//{
			//	//どことも当たらないので終わり。
			//	break;
			//}
			/*float len = (oldnextPos - nextPosition).Length();
			if (len < 0.0001)
				break;
			oldnextPos = nextPosition;*/

			loopCount++;
			if (loopCount >= 5 || !callback.isHit) 
			{
				//if (isHitFloor)
					//nowPos.y = Ypos;
				break;
			}
		}
	}
	void CharacterController::ExecuteFloor(Vector3& nowPos, Vector3& nextPos, const Vector3& originalXZDir, float& Ypos)
	{
		
		//const float OFFSETXZ = m_radius;
		//const float OFFSETY = m_radius;

		//Vector3 addPos;
		//addPos.Subtract(nextPos, nowPos);
		float addPosY = nextPos.y - nowPos.y;

		//移動の仮確定。
		//m_position.x = nextPosition.x;	
		//m_position.z = nextPosition.z;

									//レイを作成する。
		btTransform start, end;
		start.setIdentity();
		end.setIdentity();
		//始点はカプセルコライダーの中心。
		//start.setOrigin(btVector3(nowPos.x, (nowPos.y), nowPos.z));
		start.setOrigin(btVector3(nowPos.x, (nowPos.y + m_height * 0.5f + m_radius)/* + Ypos*/, nowPos.z));
		//start.setOrigin(btVector3(m_position.x, m_position.y + m_height * 0.5f + m_radius + 0.1f, m_position.z));


		bool isFall = false;
		//終点は地面上にいない場合は1m下を見る。
		//地面上にいなくてジャンプで上昇中の場合は上昇量の0.01倍下を見る。
		//地面上にいなくて降下中の場合はそのまま落下先を調べる。
		Vector3 endPos;
		//endPos.Set(start.getOrigin());
		endPos.Set(nextPos);
		if (m_isOnGround == false) {
			if (addPosY > 0.0f) {
				//ジャンプ中とかで上昇中。
				//上昇中でもXZに移動した結果めり込んでいる可能性があるので下を調べる。
				//endPos.y -= addPos.y * 0.005f;
				//endPos.y -= 0.01f;
				endPos.y = start.getOrigin().y() - 0.001;
			}
			else if (addPosY < 0.f)
			{
				//落下している場合はそのまま下を調べる。
				isFall = true;
				endPos.y += addPosY;
			}
			else
			{
				endPos.y -= 0.01f;
			}
		}
		else {
			//地面上にいない場合は1m下を見る。
			//endPos.y -= 1.f;
			endPos.y -= (m_radius + m_height * 0.5f)+0.05f;
			//endPos.y += addPos.y;
		}
		//end.setOrigin(btVector3(endPos.x, endPos.y/*+ Ypos*/, endPos.z));
		end.setOrigin(btVector3(endPos.x, endPos.y + m_radius + m_height * 0.5f/* + Ypos*/, endPos.z));
		SweepResultGround callback;
		if (m_isUseRigidBody)
			callback.me = m_rigidBody.GetBody();
		callback.startPos.Set(start.getOrigin());
		callback.nonHitCollisionAttr = m_nonHitCollisionAttr;
		//衝突検出。
		//if (fabsf(endPos.y - callback.startPos.y) > FLT_EPSILON) {
		if (fabsf(end.getOrigin().y() - start.getOrigin().y()) > FLT_EPSILON) {
			Physics().ConvexSweepTest((const btConvexShape*)m_collider.GetBody(), start, end, callback);
			bool isNearFloor = callback.dist < callback.wallDist;
			isNearFloor = true;
			//if (isNearFloor && callback.isHit) 
			if (callback.isHit)
			{
				//当たった。
				//moveSpeed.y = 0.0f;
				m_isJump = false;
				m_isOnGround = true;
				nextPos.y = callback.hitPos.y + m_offsetY;// + m_height * 0.5f + m_radius;//+m_height * 0.1f;
				//nextPos = callback.hitPos;
				//DebugPrintLineConsole(TO_INT(EDebugConsoleKind::master), "HIT FLOOR");
				//DebugPrintVector3(EDebugConsoleKind::master, nextPos-callback.hitPos);
				//DebugPrintLineConsole(TO_INT(EDebugConsoleKind::master), "");
				//DebugPrintVector3(EDebugConsoleKind::master, nextPos);
				isDebugPrinted = true;

				if (std::isnan(nextPos.x) || std::isnan(nextPos.y) || std::isnan(nextPos.z) || std::isinf(nextPos.x))
				{
					//DebugPrintLineConsole(TO_INT(EDebugConsoleKind::master), "nextPos is Nan !!!!");
				}
			}
			//if (!isNearFloor && callback.isHitWall && isFall && 1)
			if (callback.isHitWall /*&& isFall*/ && 0)
			{
				m_isJump = false;
				m_isOnGround = true;

				Vector3 vT0, vT1;
				//XZ平面上での移動後の座標をvT0に、交点の座標をvT1に設定する。
				vT0.Set(nextPos.x, 0.0f, nextPos.z);
				vT1.Set(callback.wallHitPos.x, 0.0f, callback.wallHitPos.z);
				//めり込みが発生している移動ベクトルを求める。
				Vector3 vMerikomi;
				vMerikomi = vT0 - vT1;
				//XZ平面での衝突した壁の法線を求める。。
				Vector3 hitNormalXZ = callback.wallNormal;
				hitNormalXZ.y = 0.0f;
				hitNormalXZ.Normalize();
				//めり込みベクトルを壁の法線に射影する。
				float fT0 = hitNormalXZ.Dot(vMerikomi);
				//押し戻し返すベクトルを求める。
				//押し返すベクトルは壁の法線に射影されためり込みベクトル+半径。
				Vector3 vOffset;
				vOffset = hitNormalXZ;
				vOffset *= -fT0 + m_radius + m_offsetXZ;
				//nextPos += hitNormalXZ * (m_radius + m_offsetXZ);
				nextPos += vOffset;
				//nextPos.y = callback.wallHitPos.y;

				//DebugPrintLineConsole(TO_INT(EDebugConsoleKind::master), "HIT WALL IN FLOOR");
				isDebugPrinted = true;

				if (std::isnan(nextPos.x) || std::isnan(nextPos.y) || std::isnan(nextPos.z) || std::isinf(nextPos.x))
				{
					//DebugPrintLineConsole(TO_INT(EDebugConsoleKind::master), "nextPos is Nan !!!!");
				}

				//m_position = nextPosition;
				//Vector3 newpos(nowPos.x, nextPos.y, nowPos.z);
				//ExecuteWall(newpos, nextPos, originalXZDir, Ypos);
			}
			//nextPos.y += m_height * 0.5f + m_radius;
			//if (!(callback.isHit || callback.isHitWall))
			if (!callback.isHit)
			{
				//地面上にいない。
				m_isOnGround = false;

			}
		}
		else
		{
			/*DebugPrintLineConsole(TO_INT(EDebugConsoleKind::master), "WTF!!!??? KONNA NO ARIE NAI");
			DebugPrintValue(TO_INT(EDebugConsoleKind::master), "ADD Y", addPosY);
			DebugPrintValue(TO_INT(EDebugConsoleKind::master), "DIFF s2e", fabsf(end.getOrigin().y() - start.getOrigin().y()));
			
			DebugPrintLineConsole(TO_INT(EDebugConsoleKind::master), "");*/
			isDebugPrinted = true;
		}
	}
	void CharacterController::ExebuteContactTest(const Vector3& addV)
	{
		if (m_isUseRigidBody)
		{
			
			btRigidBody* btBody = m_rigidBody.GetBody();
			//剛体を動かす。
			btBody->setActivationState(DISABLE_DEACTIVATION);
			btTransform& trans = btBody->getWorldTransform();
			
			for (int i=0;i<5;i++)
			{
				ContactTestResult res;
				res.nonHitCollisionAttr = m_nonHitCollisionAttr;
				Physics().ContactTest(m_rigidBody.GetBody(), res);
				if (res.isHit)
				{
					float t = addV.Dot(res.hitNormal);
					if (t >= 0)
					{
						res.hitNormal *= -1.f;
					}
					//Vector3 v = res.hitPos - m_position;
					
					m_position += res.hitNormal * -res.dist;
					//剛体の位置を更新。
					trans.setOrigin(btVector3(m_position.x, m_position.y + m_height * 0.5f + m_radius, m_position.z));

					/*DebugPrintLineConsole(TO_INT(EDebugConsoleKind::master), "Contact Test");
					DebugPrintVector3(TO_INT(EDebugConsoleKind::master), res.hitNormal);
					DebugPrintValue(EDebugConsoleKind::master, "DIST", res.dist);
					DebugPrintValue(EDebugConsoleKind::master, "T", t);*/
				}
				else
					return;
			}
			//DebugPrintLineConsole(TO_INT(EDebugConsoleKind::master), "Contact Test END");
		}
	}
	/*!
	* @brief	死亡したことを通知。
	*/
	void CharacterController::RemoveRigidBoby()
	{
		if (m_isRemoveRigidBody)
			return;
		Physics().RemoveRigidBody(m_rigidBody);
		m_isRemoveRigidBody = true;
	}
}