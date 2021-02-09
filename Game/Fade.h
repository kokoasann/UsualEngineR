#pragma once

/// <summary>
/// 
/// </summary>
class Fade
{
public:
	static Fade& GetInstance() {
		static Fade fade;
		return fade;
	}

	Fade();
	~Fade();

	void Update();

	void FadeIn() {
		m_state = State::enFadeIn;
		m_alpha = 1.f;
		m_isFaded = false;
	}

	void FadeOut() {
		m_state = State::enFadeOut;
		m_alpha = 0.f;
		m_isFaded = false;
	}

	void BlackOut() {
		m_panel->SetMulColor({ 0,0,0,1 });
	}

	void MakeBright() {
		m_panel->SetMulColor({ 0,0,0,0 });
	}

	const bool IsFaded() {
		return m_isFaded;
	}

	void SetFadeColor(const Vector3& c)
	{
		m_color = c;
	}
	void SetFadeSpeed(float s)
	{
		m_fadeSpeed = s;
	}

private:
	enum class State {
		enFadeIn,
		enFadeOut,
		enIdle,
	};

	State m_state = State::enIdle;

	SpriteRender* m_panel = nullptr;
	const Vector3 m_panelScale = { 100,100,100 };
	const float m_panelHeight = 15;
	const float m_panelWidth = 300;

	float m_alpha = 1.f;
	float m_fadeSpeed = 1.f;

	bool m_isFaded = false;

	Vector3 m_color = Vector3::Zero;
};
