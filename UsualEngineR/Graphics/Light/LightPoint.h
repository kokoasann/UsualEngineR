#pragma once


namespace UER
{
	class LightPoint:public LightBase
	{
	public:
		LightPoint() {}
		~LightPoint() {}


		//更新
		void Update() override;

		//本体をゲット
		SPointLight& GetBody()
		{
			return m_light;
		}


		void SetPos(const Vector3& pos)
		{
			m_light.pos = pos;
		}

		const Vector3& GetPos() const
		{
			return m_light.pos;
		}

		//光の色をセットする
		void SetCol(const Vector4& col)
		{
			m_light.color = col;
		}

		//光の色をゲットする
		const Vector4& GetCol() const
		{
			return m_light.color;
		}

		void SetDecay(float v)
		{
			m_light.decay = v;
		}
		void SetRadius(float v)
		{
			m_light.radius = v;
		}
	private:
		SPointLight m_light;
	};
}