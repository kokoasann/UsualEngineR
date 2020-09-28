/*!
 * @brief	静的オブジェクト。
 */


#include "PreCompile.h"
#include "physics/PhysicsStaticObject.h"

namespace UER
{
	PhysicsStaticObject::PhysicsStaticObject()
	{
	}
	PhysicsStaticObject::~PhysicsStaticObject()
	{
		if(m_isCreated)
			Physics().RemoveRigidBody(m_rigidBody);
	}

	void PhysicsStaticObject::CreateMeshObject(const Model& skinModel, Vector3 pos, Quaternion rot, Vector3 sca)
	{
		Matrix mTra, mRot, mSca;
		mTra.MakeTranslation(pos);
		mRot.MakeRotationFromQuaternion(rot);
		mSca.MakeScaling(sca);
		Matrix mat;
		mat.Multiply(mSca, mRot);
		mat.Multiply(mat, mTra);
		//メッシュコライダーを作成。
		m_meshCollider.CreateFromSkinModel(skinModel, &mat);
		//剛体を作成、
		RigidBodyInfo rbInfo;
		rbInfo.collider = &m_meshCollider; //剛体に形状(コライダー)を設定する。
		rbInfo.mass = 0.0f;
		rbInfo.pos = {0,0,0};
		rbInfo.rot = g_quatIdentity;
		//rbInfo.sca = sca;
		m_rigidBody.Create(rbInfo);
		m_rigidBody.GetBody()->setUserIndex(enCollisionAttr_Ground);
		//剛体を物理ワールドに追加する。
		Physics().AddRigidBody(m_rigidBody);

		m_isCreated = true;
	}
}