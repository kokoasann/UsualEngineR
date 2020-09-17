#pragma once

#include "ICollider.h"
namespace UER
{
	class SphereCollider:public ICollider
	{
	public:
		//
		SphereCollider(){}
		//
		~SphereCollider();

		/// <summary>
		/// äJï˙ÅB
		/// </summary>
		void Release();
		
		/// <summary>
		/// 
		/// </summary>
		/// <param name="radius"></param>
		void Create(float radius);

		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		btSphereShape* GetBody() const override
		{
			return m_body;
		}
	private:
		btSphereShape* m_body=nullptr;			//
	};
}