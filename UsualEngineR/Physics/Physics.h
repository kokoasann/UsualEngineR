#pragma once

#if DEBUG_FUNC
#include "physics/PhysicsDebugDraw.h"
#endif

namespace UER
{
	class RigidBody;
	class PhysicsWorld
	{
		btDefaultCollisionConfiguration* collisionConfig = nullptr;
		btCollisionDispatcher* collisionDispatcher = nullptr;	//!<衝突解決処理。
		btBroadphaseInterface* overlappingPairCache = nullptr;	//!<ブロードフェーズ。衝突判定の枝切り。
		btSequentialImpulseConstraintSolver* constraintSolver = nullptr;		//!<コンストレイントソルバー。拘束条件の解決処理。
		btDiscreteDynamicsWorld* dynamicWorld = nullptr;			//!<ワールド。

#if DEBUG_FUNC
		PhysicsDebugDraw m_debugDraw;
#endif
		

		std::mutex m_physicsWorld_mutex;
	public:
		~PhysicsWorld();
		void Init();
		void Update();
		void Release();
		/*!
		* @brief	ダイナミックワールドを取得。
		*/
		btDiscreteDynamicsWorld* GetDynamicWorld()
		{
			return dynamicWorld;
		}
		/*!
		* @brief	剛体を登録。
		*/
		void AddRigidBody(RigidBody& rb);
		/*!
		* @brief	剛体を破棄。
		*/
		void RemoveRigidBody(RigidBody& rb);
		void ConvexSweepTest(
			const btConvexShape* castShape,
			const btTransform& convexFromWorld,
			const btTransform& convexToWorld,
			btCollisionWorld::ConvexResultCallback& resultCallback,
			btScalar allowedCcdPenetration = 0.0f
		)
		{
			dynamicWorld->convexSweepTest(castShape, convexFromWorld, convexToWorld, resultCallback, allowedCcdPenetration);
		}
		void ContactText(
			btCollisionObject* colObj,
			btCollisionWorld::ContactResultCallback& resultCallback
		)
		{
			dynamicWorld->contactTest(colObj, resultCallback);
		}

		void DebugDraw();
	};

	//extern PhysicsWorld g_physics;

}
