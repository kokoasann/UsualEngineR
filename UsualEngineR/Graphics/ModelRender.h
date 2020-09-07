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
	private:
		Model m_model;
		CAnimation m_animation;

		Vector3 m_position = Vector3::Zero;
		Vector3 m_scale = Vector3::One;
		Quaternion m_rotation = Quaternion::Identity;
	};
}