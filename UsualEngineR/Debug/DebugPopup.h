#pragma once



namespace UER
{
	static void DebugAssert(bool flag, const char* mess)
	{
		if (!flag)
		{
			MessageBoxA(nullptr, mess, "アサート", MB_OK);
			std::abort();
		}
	}
	static void DebugWarning_MB(const char* mess)
	{
		MessageBoxA(nullptr, mess, "warning", MB_OK);
	}
#if DEBUG_FUNC
#define UER_ASSERT(flag,mess) DebugAssert(flag,mess)
#define UER_WARNING_MB(mess) DebugWarning_MB(mess)
#else
#define UER_ASSERT(flag,mess)
#define UER_WARNING_MB(mess)
#endif
}