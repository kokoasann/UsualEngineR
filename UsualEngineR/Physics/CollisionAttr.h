/*!
 * @brief	�R���W��������
 */
#pragma once

namespace UER
{
	/*!
	 * @brief	�R���W���������̑啪�ށB
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