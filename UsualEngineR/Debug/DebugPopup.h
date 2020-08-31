#pragma once



namespace UER
{
	void DebugAssert(bool flag, const char* mess)
	{
		if (!flag)
		{
			MessageBoxA(nullptr, mess, "アサート", MB_OK);
			std::abort();
		}
	}
#if DEBUG_FUNC
#define UER_ASSERT(flag,mess) DebugAssert(flag,mess)
#else
#define UER_ASSERT(flag,mess)
#endif
}