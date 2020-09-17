/*!
 * @brief	カプセルコライダー。
 */

#include "PreCompile.h"
#include "Physics/CapsuleCollider.h"


namespace UER
{
	/*!
		* @brief	デストラクタ。
		*/
	CapsuleCollider::~CapsuleCollider()
	{
		Release();
	}
	void CapsuleCollider::Release()
	{
		delete shape;
	}
}
