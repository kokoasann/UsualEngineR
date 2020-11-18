#pragma once



enum class GameCollisionAttribute : int
{
	None	= 0,
	Player	= UER::enCollisionAttr_User,
	Enemy	= TO_INT(UER::enCollisionAttr_User) * 2,
	Boss	= TO_INT(UER::enCollisionAttr_User) * 4,
};

int operator|(GameCollisionAttribute v1, GameCollisionAttribute v2)
{
	return TO_INT(v1) | TO_INT(v2);
}
int operator|(int v1, GameCollisionAttribute v2)
{
	return v1 | TO_INT(v2);
}
int operator|(GameCollisionAttribute  v1, int v2)
{
	return TO_INT(v1) | v2;
}