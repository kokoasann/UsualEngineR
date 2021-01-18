/*!
 * @brief	コリジョン属性
 */
#pragma once

namespace UER
{
	/*!
	 * @brief	コリジョン属性の大分類。
	 */
	enum EnCollisionAttr {
		enCollisionAttr_None = 0,
		enCollisionAttr_Ground = 1,
		enCollisionAttr_Character = 2,
		enCollisionAttr_NonHitIK = 4,
		enCOllisionAttr_IK = 8,
		enCollisionAttr_NonHit = 16,
		enCollisionAttr_Wall = 32,
		enCollisionAttr_User = 64
	};
}