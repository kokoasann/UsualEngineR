/*!
 * @brief	コライダー。
 */

#pragma once

class btCollisionShape;
namespace UER
{
	/*!
	 * @brief	コライダーのインターフェースクラス。
	 */
	class ICollider {
	public:
		virtual btCollisionShape* GetBody() const = 0;
	};
}