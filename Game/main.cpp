#include "stdafx.h"


#include "Game.h"
#include "Test/Test.h"


const int NUM_DIRECTIONAL_LIGHT = 4;	//ディレクションライトの数。

/// <summary>
/// ディレクションライト。
/// </summary>
struct DirectionalLight {
	Vector3 direction;	//ライトの方向。
	float pad0;			//パディング。
	Vector4 color;		//ライトのカラー。
};
/// <summary>
/// ライト構造体。
/// </summary>
struct Light {
	DirectionalLight directionalLight[NUM_DIRECTIONAL_LIGHT];	//ディレクションライト。
	Vector3 eyePos;					//カメラの位置。
	float specPow;					//スペキュラの絞り。
	Vector3 ambinetLight;			//環境光。
};

#define USE_UNITY_CHAN 0
#define USE_NINJA 1
#define USE_ROBO 2

#define USE_MODEL 2






///////////////////////////////////////////////////////////////////
// ウィンドウプログラムのメイン関数。
///////////////////////////////////////////////////////////////////
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	//ゲームの初期化。
	InitGame(hInstance, hPrevInstance, lpCmdLine, nCmdShow, TEXT("Game"));


	AllocConsole();
	freopen("CON", "r", stdin);
	freopen("CON", "w", stdout);
	freopen("CON", "w", stderr);


	//////////////////////////////////////
	// 初期化を行うコードを書くのはここまで！！！
	//////////////////////////////////////
	//auto& renderContext = g_graphicsEngine->GetRenderContext();

	bool isInit = false;

	SetDebugConsoleUser(EDebugConsoloUser::COMMON | /*EDebugConsoloUser::NOMOTO | */ EDebugConsoloUser::WATA);
	
	GRandom().Init(0);

	
	//auto test = NewGO<Test>(0);
	//test->Test_CreateTransform();

	NewGO<Game>(0);



	auto lig = NewGO<LightDirection>(0);
	lig->SetDir({ 0.707,0.707,0 });
	lig->SetCol(g_vec3One * 0.3);
	lig = NewGO<LightDirection>(0);
	lig->SetDir({ -0.707,0.707,0 });
	lig->SetCol(g_vec3One * 0.3);
	lig = NewGO<LightDirection>(0);
	lig->SetDir({ 0,0.707,-0.707 });
	lig->SetCol(g_vec3One * 0.3);
	lig = NewGO<LightDirection>(0);
	lig->SetDir({ 0,0.707,0.707 });
	lig->SetCol(g_vec3One * 0.3);

	g_engine->GameLoop();
	
	return 0;
}


