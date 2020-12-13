#include "PreCompile.h"
#include "IK.h"


namespace UER
{
	//IK用の当たり判定のやつ
	struct SweepResultIK : public btCollisionWorld::ConvexResultCallback
	{
		bool isHit = false;
		Vector3 hitPos = Vector3::Zero;
		Vector3 startPos = Vector3::Zero;
		Vector3 hitNormal = Vector3::Zero;
		float dist = FLT_MAX;
		btCollisionObject* me = nullptr;
		int chit = 0;
		btCollisionObject* hitCollision = nullptr;

		btScalar	addSingleResult(btCollisionWorld::LocalConvexResult& convexResult, bool normalInWorldSpace)
		{
			if (convexResult.m_hitCollisionObject == me
				|| convexResult.m_hitCollisionObject->getUserIndex() & enCollisionAttr_Character
				|| convexResult.m_hitCollisionObject->getUserIndex() & enCollisionAttr_NonHitIK)
			{
				chit += 1;
				return 0.0f;
			}
			isHit = true;
			Vector3 hitp = *(Vector3*)&convexResult.m_hitPointLocal;
			Vector3 div = startPos - hitp;
			float dis = div.Length();
			if (dis < dist)
			{
				hitNormal = *(Vector3*)&convexResult.m_hitNormalLocal;
				hitNormal.Normalize();
				hitPos = *(Vector3*)&convexResult.m_hitPointLocal;
				dist = dis;
				hitCollision = (btCollisionObject*)convexResult.m_hitCollisionObject;
			}
			return 0.0f;
		}
	};

	//IK用の当たり判定のやつ
	struct SweepResultIK_Floor : public btCollisionWorld::ConvexResultCallback
	{
		bool isHit = false;
		Vector3 hitPos = Vector3::Zero;
		Vector3 startPos = Vector3::Zero;
		Vector3 hitNormal = Vector3::Zero;
		float dist = FLT_MAX;
		btCollisionObject* me = nullptr;
		int chit = 0;
		btCollisionObject* hitCollision = nullptr;

		btScalar	addSingleResult(btCollisionWorld::LocalConvexResult& convexResult, bool normalInWorldSpace)
		{
			if (convexResult.m_hitCollisionObject == me 
				|| convexResult.m_hitCollisionObject->getUserIndex() & enCollisionAttr_Character
				|| convexResult.m_hitCollisionObject->getUserIndex() & enCollisionAttr_NonHitIK)
			{
				chit += 1;
				return 0.0f;
			}
			Vector3 norm;
			norm.Set(convexResult.m_hitNormalLocal);
			norm.Normalize();
			auto rad = fabsf(acosf(norm.Dot(Vector3::Up)));
			if (rad < Math::PI * 0.4f)
			{
				isHit = true;
				Vector3 hitp = *(Vector3*)& convexResult.m_hitPointLocal;
				Vector3 div = startPos - hitp;
				float dis = div.Length();
				if (dis < dist)
				{
					hitNormal = *(Vector3*)& convexResult.m_hitNormalLocal;
					hitNormal.Normalize();
					hitPos = *(Vector3*)& convexResult.m_hitPointLocal;
					dist = dis;

					hitCollision = (btCollisionObject*)convexResult.m_hitCollisionObject;
				}
			}
			return 0.0f;
		}
	};
	//IK用の当たり判定のやつ(壁用)
	struct SweepResultIK_Wall : public btCollisionWorld::ConvexResultCallback
	{
		bool isHit = false;
		Vector3 hitPos = Vector3::Zero;
		Vector3 startPos = Vector3::Zero;
		Vector3 hitNormal = Vector3::Zero;
		float dist = FLT_MAX;
		btCollisionObject* me = nullptr;
		int chit = 0;
		btCollisionObject* hitCollision = nullptr;

		btScalar	addSingleResult(btCollisionWorld::LocalConvexResult& convexResult, bool normalInWorldSpace)
		{
			if (convexResult.m_hitCollisionObject == me
				|| convexResult.m_hitCollisionObject->getUserIndex() & enCollisionAttr_Character
				|| convexResult.m_hitCollisionObject->getUserIndex() & enCollisionAttr_NonHitIK)
			{
				chit += 1;
				return 0.0f;
			}
			Vector3 norm;
			norm.Set(convexResult.m_hitNormalLocal);
			norm.Normalize();
			auto rad = fabsf(acosf(norm.Dot(Vector3::Up)));
			if (rad >= Math::PI * 0.2f && rad < Math::PI * 0.85f)
			{
				isHit = true;
				Vector3 hitp = *(Vector3*)& convexResult.m_hitPointLocal;
				Vector3 div = startPos - hitp;
				float dis = div.Length();
				if (dis < dist)
				{
					hitNormal = *(Vector3*)& convexResult.m_hitNormalLocal;
					hitPos = *(Vector3*)& convexResult.m_hitPointLocal;
					dist = dis;
					hitCollision = (btCollisionObject*)convexResult.m_hitCollisionObject;
				}
			}
			return 0.0f;
		}
	};
	/// <summary>
	/// ボーンのワールドマトリクスを持ってくる
	/// </summary>
	/// <param name="bone">マトリクスの欲しいボーン</param>
	/// <param name="worldMat">モデルのマトリクス</param>
	/// <returns></returns>
	Matrix GetBoneWorldMatrix(Skeleton* skele, Bone* bone,const Matrix& worldMat)
	{
		Bone* parents[32] = { NULL };
		int cont = 0;
		Bone* b = bone;
		while (true)
		{
			parents[cont++] = b;
			int pNo = b->GetParentBoneNo();
			if (pNo == -1)
				break;
			b = skele->GetBone(pNo);
			
		}
		auto mat = worldMat;
		for (int i = cont - 1; i >= 0; i--)
		{
			const auto& locmat = parents[i]->GetLocalMatrix();
			mat.Multiply(locmat, mat);
		}
		//mat.Mul(bone->GetLocalMatrix(),mat);
		return mat;
	}

	/// <summary>
	/// ボーンのワールドマトリクスを持ってくる
	/// </summary>
	/// <param name="bone">マトリクスの欲しいボーン</param>
	/// <param name="worldMat">モデルのマトリクス</param>
	/// <returns></returns>
	Matrix GetBoneWorldMatrix_InRootBoneSpace(Skeleton* skele, Bone* bone, const Matrix& worldMat)
	{
		Matrix mat = bone->GetLocalMatrix() * worldMat;
		return mat;
	}
	/// <summary>
	/// 
	/// </summary>
	/// <param name="bone"></param>
	/// <param name="wm"></param>
	/// <param name="worldMat"></param>
	/// <returns></returns>
	Matrix GetBoneLocalMatrix(Skeleton* skel,Bone * bone, const Matrix& wm, const Matrix& worldMat)
	{
		Matrix mat = wm;
		Matrix inv;
		inv.Inverse(GetBoneWorldMatrix(skel, skel->GetBone(bone->GetParentBoneNo()),worldMat));
		mat.Multiply(wm, inv);
		return mat;
	}
	/// <summary>
	/// 
	/// </summary>
	/// <param name="effector"></param>
	/// <param name="end"></param>
	/// <returns></returns>
	float GetLengthEffector2End(Skeleton* skele,Bone* effector, Bone* end)
	{
		float len = 0.f;
		Vector3 vlen = Vector3::Zero;
		Bone* pb= skele->GetBone(effector->GetParentBoneNo());
		for (Bone* cb = effector; pb != end; pb = skele->GetBone(cb->GetParentBoneNo()))
		{
			vlen += (cb->GetWorldMatrix().GetTransrate() - pb->GetWorldMatrix().GetTransrate());
			cb = pb;
		}
		for (Bone* b = effector; b != end; b = skele->GetBone(b->GetParentBoneNo()))
		{
			len += b->GetWorldMatrix().GetTransrate().Length();
		}
		return len;
	}


	IK::IK(Skeleton* skeleton, Bone* effector, Bone* end, float radius, bool isOnRigitBody, const Vector3& pos) :
		m_skeleton(skeleton),
		m_effectorBone(effector),
		m_endBone(end),
		m_radius(radius),
		m_isUseRigidBody(isOnRigitBody)
	{
		m_collider.Create(radius);
		if(isOnRigitBody)
			InitRigidBody(pos);
	}

	IK::~IK()
	{
		if (m_isUseRigidBody)
		{
			Physics().RemoveRigidBody(m_rigidBody);
			m_rigidBody.Release();
		}
		m_collider.Release();
	}

	void IK::InitRigidBody(const Vector3& pos)
	{
		RigidBodyInfo rbinfo;
		rbinfo.mass = 0.f;
		rbinfo.collider = &m_collider;
		m_rigidBody.Create(rbinfo);
		m_rigidBody.GetBody()->setUserIndex(enCollisionAttr_Character|enCollisionAttr_NonHitIK|enCOllisionAttr_IK);
		m_rigidBody.GetBody()->setCollisionFlags(btCollisionObject::CF_CHARACTER_OBJECT);
		auto& rpos = m_rigidBody.GetBody()->getWorldTransform();
		Matrix base,tr;
		base = Matrix::Identity;
		tr.MakeTranslation(pos);
		tr.Multiply(base, tr);
		auto m = GetBoneWorldMatrix(m_skeleton, m_effectorBone, tr);
		rpos.setOrigin(btVector3(m.m[3][0], m.m[3][1], m.m[3][2]));
		Physics().AddRigidBody(m_rigidBody);
	}

	void IK::UpdateTarget(const Matrix& worldMat)
	{
		switch (m_ikMode)
		{
		case enMode_Normal:
			UpdateTarget_Normal(worldMat);
			break;
		case enMode_Foot:
			UpdateTarget_Foot(worldMat);
			break;
		case enMode_NoneHit:
			UpdateTarget_NoneHit(worldMat);
			break;
		case enMode_NoAnimHit:
			UpdateTarget_NoAnimHit(worldMat);
			break;
		}
		//if (m_ikMode != enMode_NoAnimHit)
		m_isToggleNoAnimHit = false;
	}


	/// <summary>
	/// 
	/// </summary>
	/// <param name="worldMat"></param>
	void IK::UpdateTarget_Normal(const Matrix& worldMat)
	{
		auto effmat = GetBoneWorldMatrix(m_skeleton, m_effectorBone, worldMat);
		auto effpos = effmat.GetTransrate();
		auto currentBone = m_skeleton->GetBone(m_effectorBone->GetParentBoneNo());						//作業中のボーン

		auto newpos = effpos + m_velocity;	//移動先のポジション
		if (m_isSetNextTarget)
		{
			newpos = m_nextTarget;
			m_isSetNextTarget = false;
			m_isSetOldTarget = true;
		}

		auto oldpos = m_effectorBone->GetWorldMatrix().GetTransrate();	//移動前のポジション
		if (m_isFirst)
		{
			oldpos += worldMat.GetTransrate();
			m_isFirst = false;
		}
		else
		{
			oldpos = m_target;
		}
		Vector3 o2n = (newpos - oldpos);
		if (o2n.Length() <= 2.0e-5f)
			return;

		SweepResultIK sr;
		sr.startPos = oldpos;
		sr.me = m_rigidBody.GetBody();

		Vector3 target = newpos;//ターゲット

		//if((oldpos-newpos).Length() > FLT_EPSILON)

		btTransform bstart, bend;
		bstart.setIdentity();
		bend.setIdentity();
		bstart.setOrigin(btVector3(oldpos.x, oldpos.y, oldpos.z));
		bend.setOrigin(btVector3(newpos.x, newpos.y, newpos.z));
		bool isHit = false;
		Vector3 tarbuf = newpos;
		while(true)
		{

			Physics().ConvexSweepTest((const btConvexShape*)m_collider.GetBody(), bstart, bend, sr);
			if (sr.isHit)
			{
				auto meri = target - sr.hitPos;
				float rad = sr.hitNormal.Dot(meri);
				//auto ntarget = target + sr.hitNormal * (-rad + m_radius + 0.2f);
				if (!m_isHit)
				{
					//m_effectorBone->SetIsONGround(true);

					target = sr.hitPos + sr.hitNormal * (m_radius+0.2f);
					//target.Lerp(m_rubbing, ntarget, target);
					//m_rubTarget = ntarget;
				}
				else
				{
					target = sr.hitPos + sr.hitNormal * (m_radius + 0.2f);
					//target.Lerp(m_rubbing, ntarget, m_target);
				}
				auto t2t = (target - tarbuf);
				float l = t2t.Length();
				if (l < 2.0e-5f)
					break;
				auto o2t = (target - oldpos);
				l = o2t.Length();
				if (l < 2.0e-5f)
					return;
				isHit = true;
				tarbuf = target;
				bend.setOrigin(btVector3(target.x, target.y, target.z));
				sr.dist = FLT_MAX;
				sr.isHit = false;
			}
			else
			{
				/*if(!isHit)
					m_effectorBone->SetIsONGround(false);
				if (m_isHit)
				{
					m_effectorBone->SetMomentum(newpos - oldpos);
				}
				else
				{
					m_effectorBone->SetMomentum(Vector3::Zero);
				}*/
				break;
			}
		}

		//m_target += (target - oldpos);// *m_speed;
		m_target = target;

		//m_isHit = sr.isHit;
		m_isHit = isHit;

		//m_move = newpos - m_target;
		//auto o2n = m_target - oldpos;
		//m_effectorBone->SetMove(m_move);

		//m_offset = Vector3::Zero;
		m_velocity = Vector3::Zero;
		if (m_isUseRigidBody)
			UpdateRigidBody(m_target);
	}


	/// <summary>
	/// 
	/// </summary>
	/// <param name="worldMat"></param>
	void IK::UpdateTarget_Foot(const Matrix& worldMat)
	{
#if 0
		auto effmat = GetBoneWorldMatrix(m_effectorBone, worldMat);
		auto effpos = effmat.GetTransrate();
		auto currentBone = m_effectorBone->GetParent();						//作業中のボーン
		auto modelpos = worldMat.GetTransrate();		//モデルのポジション

		auto orgpos = effpos - modelpos;//モデルのポジションからの距離
		orgpos += m_offset;
		Vector3 newpos;	//移動先のポジション
		if (m_isSetNextTarget)
		{
			newpos = m_nextTarget + m_offset;
			orgpos = newpos- modelpos;
			m_isSetNextTarget = false;
			m_isSetOldTarget = true;
		}
		else
			newpos = effpos + m_offset;

		//if (m_timer <= 0.5f)		//ボーンの移動量からターゲットを求めるやつ
		//{
		//	//effmat = GetBoneWorldMatrix(m_effectorBone, imat);
		//	//effpos = effmat.GetTransrate();
		//	//auto move = (effpos+m_offset) - m_oldNewTarget;
		//	//newpos = m_target + move;
		//}
		//else
		//{
		//	m_timer = 0.0f;
		//}
		//m_timer += gameTime()->GetDeltaTime();

		m_oldTarget = m_target;
		Vector3 oldpos;	//移動前のポジション
		if (m_isFirst)
		{
			oldpos = m_effectorBone->GetBindPoseMatrix().GetTransrate();
			oldpos += modelpos;
			m_isFirst = false;
		}
		else
		{
			oldpos = m_target;
		}

		m_animMove = orgpos - m_oldNewTarget;
		//重力処理。
		float time = gameTime()->GetDeltaTime();
		if ((m_animMove.y) <= m_radius*0.05f)	//下に移動している
		{
			m_gravitPow += m_gravity * time;
			m_effectorBone->SetIsDown(true);
		}
		else	//上に移動している
		{
			m_gravitPow = 0.f;
			m_effectorBone->SetIsDown(false);
		}
		newpos.y -= m_gravitPow;		//プラスグラビティ(マイナス)

		if ((newpos - oldpos).Length() < FLT_EPSILON)		//ほんまに移動してんのか？
			return;	//移動してないやん！！

		//oldpos.y += m_radius;
		Vector3 nowpos = oldpos;		//正直いらない
		Vector3 target = oldpos;		//真の移動先。
		btTransform bstart, bend;			
		bstart.setIdentity();
		bend.setIdentity();
		bool isHitGround = false;		//地面に当たった？


		if (fabsf(orgpos.y - m_oldNewTarget.y) > FLT_EPSILON)
		{
			SweepResultIK_Floor sr;
			sr.startPos = oldpos;
			sr.me = m_rigidBody.GetBody();

			bstart.setOrigin(btVector3(oldpos.x, oldpos.y, oldpos.z));
			//bend.setOrigin(btVector3(newpos.x, newpos.y, newpos.z));
			bend.setOrigin(btVector3(oldpos.x, newpos.y, oldpos.z));

			//target = newpos;//ターゲット
			Physics().ConvexSweepTest((const btConvexShape*)m_collider.GetBody(), bstart, bend, sr);
			if (sr.isHit)
			{
				target = oldpos;
				target.y = sr.hitPos.y + m_radius;
			}
			else
			{
				target.y = newpos.y;
				if (m_isHit)
				{
					m_effectorBone->SetMomentum(newpos - nowpos);
				}
				else
				{
					m_effectorBone->SetMomentum(Vector3::Zero);
				}
			}
			m_effectorBone->SetIsONGround(sr.isHit);
			isHitGround = sr.isHit;
			m_hitCollision = sr.hitCollision;
		}
		else
		{
			m_effectorBone->SetIsONGround(m_isHit);
			isHitGround = m_isHit;
		}
		


		if ((Vector3( oldpos.x, 0.f, oldpos.z) -Vector3( newpos.x, 0.f, newpos.z )).Length() > FLT_EPSILON)
		{
			//壁の判定。
			bstart.setOrigin(btVector3(oldpos.x, target.y, oldpos.z));
			bend.setOrigin(btVector3(newpos.x, target.y, newpos.z));
			Vector3 newpos2 = newpos;
			newpos2.y = target.y;
			SweepResultIK_Wall sr_w;
			sr_w.me = m_rigidBody.GetBody();
			sr_w.startPos = Vector3(oldpos.x, target.y, oldpos.z);
			Physics().ConvexSweepTest((const btConvexShape*)m_collider.GetBody(), bstart, bend, sr_w);
			if (sr_w.isHit)
			{
				//m_effectorBone->SetIsONGround(true);

				auto norm = sr_w.hitNormal;
				norm.Normalize();
				auto meri = newpos2 - sr_w.hitPos;

				float rad = norm.Dot(meri);
				auto ntar = newpos2 + sr_w.hitNormal * (-rad + m_radius);
				if (isHitGround)
					target.Lerp(m_rubbing, ntar, target);
				else
					target = ntar;
			}
			else
			{
				if (isHitGround)
					target.Lerp(m_rubbing, newpos2, target);
				else
					target = newpos2;
			}
			m_hitCollision = sr_w.hitCollision;
		}

//#if 1
		if (fabsf(orgpos.y - m_oldNewTarget.y) > m_radius * 0.05f && fabsf(target.y - newpos.y) > FLT_EPSILON)
		{
			SweepResultIK_Floor sr;
			sr.startPos = target;
			sr.me = m_rigidBody.GetBody();

			bstart.setOrigin(btVector3(target.x, target.y, target.z));
			bend.setOrigin(btVector3(target.x, newpos.y, target.z));

			//target = newpos;//ターゲット
			Physics().ConvexSweepTest((const btConvexShape*)m_collider.GetBody(), bstart, bend, sr);
			if (sr.isHit)
			{
				target.y = sr.hitPos.y + m_radius;
			}
			else
			{
				target.y = newpos.y;
				if (m_isHit)
				{
					m_effectorBone->SetMomentum(newpos - nowpos);
				}
				else
				{
					m_effectorBone->SetMomentum(Vector3::Zero);
				}
			}
			m_effectorBone->SetIsONGround(sr.isHit);
			isHitGround = sr.isHit;
			m_hitCollision = sr.hitCollision;
		}
//#endif

		m_target += (target - nowpos) * m_speed;
		
		m_isHit = isHitGround;
		
		m_move = (orgpos+m_offset) - (m_target- modelpos);

		m_effectorBone->SetMove(m_move);
		m_oldNewTarget = orgpos;
		m_offset = Vector3::Zero;

		if (m_isUseRigidBody)
			UpdateRigidBody(m_target);
#endif
	}


	void IK::UpdateTarget_NoneHit(const Matrix& worldMat)
	{
		m_oldTarget = m_target;
		if (m_isSetNextTarget)
			m_target = m_nextTarget;
		
		if (m_isUseRigidBody)
			UpdateRigidBody(m_target);
	}

	void IK::UpdateTarget_NoAnimHit(const Matrix& worldMat)
	{

		if (m_isToggleNoAnimHit)
		{
			auto effmat = GetBoneWorldMatrix(m_skeleton, m_effectorBone, worldMat);
			m_target = effmat.GetTransrate();
		}

		Vector3 newpos = m_target;

		if (m_isSetNextTarget)
		{
			newpos = m_nextTarget;
			m_isSetNextTarget = false;
			m_isSetOldTarget = true;
		}
		newpos += m_velocity;

		auto oldpos = m_effectorBone->GetWorldMatrix().GetTransrate();	//移動前のポジション
		if (m_isFirst)
		{
			oldpos += worldMat.GetTransrate();
			m_isFirst = false;
		}
		else
		{
			oldpos = m_target;
		}
		Vector3 o2n = (newpos - oldpos);
		if (o2n.Length() <= 2.0e-5f)
			return;

		SweepResultIK sr;
		sr.startPos = oldpos;
		sr.me = m_rigidBody.GetBody();

		Vector3 target = newpos;//ターゲット


		btTransform bstart, bend;
		bstart.setIdentity();
		bend.setIdentity();
		bstart.setOrigin(btVector3(oldpos.x, oldpos.y, oldpos.z));
		bend.setOrigin(btVector3(newpos.x, newpos.y, newpos.z));
		bool isHit = false;
		Vector3 tarbuf = newpos;
		while (true)
		{

			Physics().ConvexSweepTest((const btConvexShape*)m_collider.GetBody(), bstart, bend, sr);
			if (sr.isHit)
			{
				auto meri = target - sr.hitPos;
				float rad = sr.hitNormal.Dot(meri);
				if (!m_isHit)
				{
					target = sr.hitPos + sr.hitNormal * (m_radius + 0.2f);
				}
				else
				{
					target = sr.hitPos + sr.hitNormal * (m_radius + 0.2f);
				}
				auto t2t = (target - tarbuf);
				float l = t2t.Length();
				if (l < 2.0e-5f)
					break;
				auto o2t = (target - oldpos);
				l = o2t.Length();
				if (l < 2.0e-5f)
					return;
				isHit = true;
				tarbuf = target;
				bend.setOrigin(btVector3(target.x, target.y, target.z));
				sr.dist = FLT_MAX;
				sr.isHit = false;
			}
			else
			{
				break;
			}
		}

		m_target = target;

		m_isHit = isHit;

		m_velocity = Vector3::Zero;
		if (m_isUseRigidBody)
			UpdateRigidBody(m_target);
	}

	void IK::Update(const Matrix& worldMat)
	{
		switch (m_usingIK)
		{
		case enUSE_CCD:
			EXE_CCD(worldMat);
			break;
		}
	}
	void IK::EXE_CCD(const Matrix& worldMat)
	{
		//どこにもヒットしてない、もしくはNextTargetもされてなく、IKモードがFootじゃない場合必ず早期リターン
		/*if (!(m_isHit or m_isSetOldTarget) and m_ikMode!=IKMode::enMode_Foot)
			return;*/
		switch (m_ikMode)
		{
		case IKMode::enMode_Foot:
			break;
		case IKMode::enMode_Normal:
			/*if (!(m_isHit or m_isSetNextTarget))
				return;*/
			break;
		case IKMode::enMode_NoneHit:
			if (!m_isSetNextTarget && m_velocity.Length() <= FLT_EPSILON)
				return;
			break;
		}

		for (int i = 0; i < 3; i++)
		{
			auto effmat = GetBoneWorldMatrix(m_skeleton, m_effectorBone, worldMat);
			auto effpos = effmat.GetTransrate();
			auto currentBone = m_skeleton->GetBone(m_effectorBone->GetParentBoneNo());						//作業中のボーン
			while (true)
			{
				auto currentmat = GetBoneWorldMatrix(m_skeleton, currentBone, worldMat);//作業ボーンの世界行列。

				auto currentlocal = currentBone->GetLocalMatrix();//作業ボーンのローカル行列。
				Vector3 lopo, losc;
				Quaternion loro;
				currentlocal.CalcMatrixDecompose(lopo, loro, losc);

				Matrix inv;							//カレントボーンの逆行列。
				inv.Inverse(currentmat);

				Vector3 localeffpos = effpos;
				inv.Apply(localeffpos);					//ワールド座標のエフェクタボーンをカレントボーンのローカル座標にする
				Vector3 localtarpos = m_target;
				inv.Apply(localtarpos);					//ワールド座標のターゲットをカレントボーンのローカル座標にする

				auto toEffector = localeffpos;			//エフェクターからカレントボーンのベクトル
				auto toTarget = localtarpos;			//ターゲットからカレントボーンのベクトル

				{
					Quaternion addrot(toEffector, toTarget);
					
					loro.Multiply(addrot, loro);
					
					Matrix mfin;
					mfin.MakeTransform(lopo, losc, loro);
					
					currentBone->SetLocalMatrix(mfin);
				}

				if (m_endBone == currentBone)
					break;
				effmat = GetBoneWorldMatrix(m_skeleton, m_effectorBone, worldMat);
				effpos = effmat.GetTransrate();

				currentBone = m_skeleton->GetBone(currentBone->GetParentBoneNo());
			}
		}
		
		m_isSetOldTarget = false;
		m_isSetNextTarget = false;
		//UpdateRigidBody(m_target);
	}

	void IK::UpdateRigidBody(const Vector3& pos)
	{
		auto body = m_rigidBody.GetBody();
		body->setActivationState(DISABLE_DEACTIVATION);
		auto& bpo = body->getWorldTransform();
		bpo.setOrigin({ pos.x,pos.y, pos.z });
	}

	void IK::SetPos(const Vector3& p)
	{
		m_target = p;
		m_oldNewTarget = p;
		m_oldTarget = p;
		//m_nextTarget = p;
		m_isSetNextTarget = false;
		//m_rubTarget = p;
		if(m_isUseRigidBody)
			UpdateRigidBody(p);
	}
}