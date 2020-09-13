#pragma once

#include "Animation.h"

namespace UER
{
	class ModelRender:public GameObject
	{
	public:
		ModelRender() {}
		/// <summary>
		/// 
		/// </summary>
		virtual void Release() override;
		/// <summary>
		/// 
		/// </summary>
		virtual void OnDestroy() override;

		/// <summary>
		/// 
		/// </summary>
		/// <param name="initdata"></param>
		void Init(const ModelInitData& initdata);

		void InitAnimation(const std::vector<CAnimationClipPtr>& anim, int animNum);

		/// <summary>
		/// 
		/// </summary>
		void Awake() override;

		void Update() override;

		void Render() override;

		void Play(const unsigned int animNum,float interpolateTime=0.0f)
		{
			m_animation.Play(animNum, interpolateTime);
		}

		void SetPosition(const Vector3& v)
		{
			m_position = v;
		}
		void SetScale(const Vector3& v)
		{
			m_scale = v;
		}
		void SetRotation(const Quaternion& v)
		{
			m_rotation = v;
		}

		const Vector3& GetPosition() const
		{
			return m_position;
		}
		const Vector3& GetScale() const
		{
			return m_scale;
		}
		const Quaternion& GetRotation() const
		{
			return m_rotation;
		}
	private:
		Model m_model;
		CAnimation m_animation;

		Vector3 m_position = Vector3::Zero;
		Vector3 m_scale = Vector3::One;
		Quaternion m_rotation = Quaternion::Identity;
	};
}