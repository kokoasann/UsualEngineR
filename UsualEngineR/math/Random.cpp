#include "PreCompile.h"
#include "Random.h"


#undef min
#undef max
namespace UER
{
	void GameRandom::Init(UINT seed)
	{
		m_engine.seed(seed);
		m_min = m_engine.min();
		m_max = m_engine.max();
	}
}