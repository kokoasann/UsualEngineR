#pragma once
class Player;

/// <summary>
/// 
/// </summary>
class IPlayerState
{
public:
	IPlayerState() {};
	virtual ~IPlayerState() {};
	virtual void Enter(Player*) = 0;
	virtual IPlayerState* Update(Player*) = 0;
	virtual void Exit(Player*) = 0;

protected:
	float Approach(float goal, float current, float dt) {
		auto diff = goal - current;
		if (diff > dt) {
			return current + dt;
		}
		if (diff < -dt) {
			return current - dt;
		}
		return goal;
	}
};