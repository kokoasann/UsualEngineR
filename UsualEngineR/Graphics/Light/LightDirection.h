#pragma once

#include "LightBase.h"
#include "LightStruct.h"

namespace UER
{
	//ディレクションライト
	class LightDirection:public LightBase
	{
	public:
		LightDirection();
		~LightDirection();


		//更新
		void Update() override;

		//本体をゲット
		SDirectionLight& GetBody()
		{
			return m_light;
		}

		//光の指す方をセットする
		void SetDir(const Vector3& dir)
		{
			m_light.dir = dir;
		}

		//輝きの導く方をゲットする
		Vector3 GetDir() const
		{
			return m_light.dir;
		}


		//光の色をセットする
		void SetCol(const Vector4& col)
		{
			m_light.color = col;
		}

		//光の色をゲットする
		Vector4 GetCol() const
		{
			return m_light.color;
		}

	private:
		SDirectionLight m_light;		//DirectionLightの構造体
	};
}