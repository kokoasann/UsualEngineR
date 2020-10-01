#include "PreCompile.h"
#include "physics/Physics.h"
#include "Physics/RigidBody.h"

namespace UER
{
	//PhysicsWorld g_physics;

	PhysicsWorld::~PhysicsWorld()
	{
		Release();
	}
	void PhysicsWorld::Release()
	{
		delete dynamicWorld;
		delete constraintSolver;
		delete overlappingPairCache;
		delete collisionDispatcher;
		delete collisionConfig;

		dynamicWorld = nullptr;
		constraintSolver = nullptr;
		overlappingPairCache = nullptr;
		collisionDispatcher = nullptr;
		collisionConfig = nullptr;
	}
	void PhysicsWorld::Init()
	{
		Release();
		//�����G���W�����������B
		///collision configuration contains default setup for memory, collision setup. Advanced users can create their own configuration.
		collisionConfig = new btDefaultCollisionConfiguration();

		///use the default collision dispatcher. For parallel processing you can use a diffent dispatcher (see Extras/BulletMultiThreaded)
		collisionDispatcher = new	btCollisionDispatcher(collisionConfig);

		///btDbvtBroadphase is a good general purpose broadphase. You can also try out btAxis3Sweep.
		overlappingPairCache = new btDbvtBroadphase();

		///the default constraint solver. For parallel processing you can use a different solver (see Extras/BulletMultiThreaded)
		constraintSolver = new btSequentialImpulseConstraintSolver;

		dynamicWorld = new btDiscreteDynamicsWorld(
			collisionDispatcher,
			overlappingPairCache,
			constraintSolver,
			collisionConfig
		);

		dynamicWorld->setGravity(btVector3(0, -10, 0));

		//m_debugDraw.Init();
		//dynamicWorld->setDebugDrawer(&m_debugDraw);
	}
	void PhysicsWorld::Update()
	{
		dynamicWorld->stepSimulation(gameTime()->GetDeltaTime());
	}
	void PhysicsWorld::AddRigidBody(RigidBody & rb)
	{
		std::lock_guard<std::mutex> lg(m_physicsWorld_mutex);
		dynamicWorld->addRigidBody(rb.GetBody());
	}
	void PhysicsWorld::RemoveRigidBody(RigidBody & rb)
	{
		auto body = rb.GetBody();
		if (body != nullptr)
		{
			std::lock_guard<std::mutex> lg(m_physicsWorld_mutex);
			dynamicWorld->removeRigidBody(body);
		}
	}
	void PhysicsWorld::DebugDraw()
	{
#if DEBUG_FUNC
		/*m_debugDraw.BeginRender();
		dynamicWorld->debugDrawWorld();
		m_debugDraw.EndRender();*/
#endif
	}
}