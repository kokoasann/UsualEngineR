#pragma once

#include "Animation.h"

namespace UER
{
	//
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
		void SetMulColor(const Vector4& col)
		{
			m_mulColor = col;
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

		void SetIsRender(bool b)
		{
			m_isRender = b;
		}
		bool IsRender() const
		{
			return m_isRender;
		}

		const Model& GetModel() const
		{
			return m_model;
		}
	private:
		Model m_model;				//モデル
		CAnimation m_animation;		//アニメーション

		Vector3 m_position = Vector3::Zero;				//位置。
		Vector3 m_scale = Vector3::One;					//大きさ
		Quaternion m_rotation = Quaternion::Identity;	//回転。
		Vector4 m_mulColor = Vector4::White;

		bool m_isInit = false;
		bool m_isRender = false;	//レンダリングする?
	};
}