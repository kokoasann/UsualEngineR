/*!
 * @brief	�J�v�Z���R���C�_�[�B
 */

#include "PreCompile.h"
#include "Physics/CapsuleCollider.h"


namespace UER
{
	/*!
		* @brief	�f�X�g���N�^�B
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
