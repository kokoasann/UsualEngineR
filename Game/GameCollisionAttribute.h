#pragma once



enum class GameCollisionAttribute : int
{
	None	= 0,
	Player	= UER::enCollisionAttr_User,
	Enemy	= Player * 2,
	Boss	= Player * 4,
	BombShield	= Player * 8,
};


/// <summary>
///  OR OPERATOR
/// </summary>

int operator|(GameCollisionAttribute v1, GameCollisionAttribute v2)
{
	return TO_INT(v1) | TO_INT(v2);
}
template<class T>
int operator|(T v1, GameCollisionAttribute v2)
{
	return TO_INT(v1) | TO_INT(v2);
}

template<class T>
int operator|(GameCollisionAttribute  v1, T v2)
{
	return TO_INT(v1) | TO_INT(v2);
}


/// <summary>
///  AND OPERATOR
/// </summary>

template<class T>
int operator&(T v1, GameCollisionAttribute v2)
{
	return TO_INT(v1) & TO_INT(v2);
}