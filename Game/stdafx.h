#pragma once


#include "PreCompile.h"
using namespace UER;

//ゲーム側デバッグ表示定義
//#define _PRINT_PLAYER_STATE //ステート遷移をコンソールに表示
//#define _PRINT_ENEMY_STATE

enum class EDebugConsoloUser :int
{
	COMMON = 1,
	NOMOTO = 2,
	WATA = TO_INT(EDebugConsoleKind::num),
	YOSI = TO_INT(EDebugConsoleKind::num) + 2,
	INAM = TO_INT(EDebugConsoleKind::num) + 4,
};

static UINT operator|(EDebugConsoloUser x, EDebugConsoloUser y)
{
	return TO_INT(x) | TO_INT(y);
}
static UINT operator|(UINT x, EDebugConsoloUser y)
{
	return x | TO_INT(y);
}

static void SetDebugConsoleUser(UINT user)
{
	DebugConsoleUser |= TO_INT(user);
}

static void DebugPrintLine(EDebugConsoloUser user,const char* str)
{
	DebugPrintLineConsole(TO_INT(user), str);
}

static void DebugPrint_COMMON(const char* str)
{
	DebugPrintLine(EDebugConsoloUser::COMMON, str);
}

static void DebugPrint_NOMOTO(const char* str)
{
	DebugPrintLine(EDebugConsoloUser::NOMOTO, str);
}

static void DebugPrint_WATA(const char* str)
{
	DebugPrintLine(EDebugConsoloUser::WATA, str);
}


