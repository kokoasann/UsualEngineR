#pragma once




namespace UER
{
	class ICollider;
	//剛体情報。
	struct RigidBodyInfo {

		Vector3 pos = g_vec3Zero;			//座標。
		Quaternion rot = g_quatIdentity;	//回転。
		Vector3 sca = g_vec3One;				
		ICollider* collider = nullptr;				//コライダー。
		float mass = 0.0f;							//質量。

	};
	//剛体クラス。
	class RigidBody
	{
		btRigidBody* rigidBody = nullptr;		//剛体。
		btDefaultMotionState* myMotionState = nullptr;	//モーションステート。
	public:

		~RigidBody();
		void Release();
		void Create(RigidBodyInfo& rbInfo);
		btRigidBody* GetBody()
		{
			return rigidBody;
		}
	};

}