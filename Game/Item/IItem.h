#pragma once
class Player;

class IItem
{
public:
	IItem();
	virtual ~IItem();

	//����
	virtual void Apply(Player* player) = 0;

	//�X�V
	virtual void Update() {};
	
	void SetPosition(const Vector3& pos) {
		m_position = pos;
	}
	
	const Vector3& GetPosition() const {
		return m_position;
	}

	const float GetRange() {
		return m_range;
	}

protected:
	Vector3 m_position = Vector3::Zero;
	float m_range = 20.f;
};

