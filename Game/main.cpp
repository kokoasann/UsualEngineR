#include "stdafx.h"


#include "Game.h"
#include "Title.h"
#include "Test/Test.h"
#include "GameDebugger.h"


const int NUM_DIRECTIONAL_LIGHT = 4;	//�f�B���N�V�������C�g�̐��B

/// <summary>
/// �f�B���N�V�������C�g�B
/// </summary>
struct DirectionalLight {
	Vector3 direction;	//���C�g�̕����B
	float pad0;			//�p�f�B���O�B
	Vector4 color;		//���C�g�̃J���[�B
};
/// <summary>
/// ���C�g�\���́B
/// </summary>
struct Light {
	DirectionalLight directionalLight[NUM_DIRECTIONAL_LIGHT];	//�f�B���N�V�������C�g�B
	Vector3 eyePos;					//�J�����̈ʒu�B
	float specPow;					//�X�y�L�����̍i��B
	Vector3 ambinetLight;			//�����B
};

#define USE_UNITY_CHAN 0
#define USE_NINJA 1
#define USE_ROBO 2

#define USE_MODEL 2






///////////////////////////////////////////////////////////////////
// �E�B���h�E�v���O�����̃��C���֐��B
///////////////////////////////////////////////////////////////////
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	//�Q�[���̏������B
	InitGame(hInstance, hPrevInstance, lpCmdLine, nCmdShow, TEXT("Game"));


	AllocConsole();
	freopen("CON", "r", stdin);
	freopen("CON", "w", stdout);
	freopen("CON", "w", stderr);


	//////////////////////////////////////
	// ���������s���R�[�h�������̂͂����܂ŁI�I�I
	//////////////////////////////////////
	//auto& renderContext = g_graphicsEngine->GetRenderContext();

	bool isInit = false;

	SetDebugConsoleUser(EDebugConsoloUser::COMMON /*| EDebugConsoloUser::NOMOTO */|  EDebugConsoloUser::WATA);
	
	GRandom().Init(0);

	
	//auto test = NewGO<Test>(0);
	//test->Test_CreateTransform();

	//NewGO<Title>(0);
	NewGO<Game>(0);
	NewGO<GameDebugger>(0);
	//Ghost Object test
	/*BoxCollider bc;
	bc.Create({ 50,50,50 });
	GhostObject go;
	go.Create(&bc, Vector3::Zero, { 0,0,0,1 });*/


	Vector3 dir = { 0.707,0.707,0 };
	dir.Normalize();
	auto lig = NewGO<LightDirection>(0);
	lig->SetDir(dir);
	lig->SetCol(g_vec3One * 0.8);

	lig = NewGO<LightDirection>(0);
	dir = { -0.1,-1,0 };
	dir.Normalize();
	lig->SetDir(dir);
	lig->SetCol(Vector3(0.7,0.4,0.1) * .5);
	lig = NewGO<LightDirection>(0);
	dir = { 0,1,0 };
	dir.Normalize();
	lig->SetDir(dir);
	lig->SetCol(Vector3(0.3, 0.4, 0.9) * .7);

	lig = NewGO<LightDirection>(0);
	lig->SetDir({ -0.707,0.707,0 });
	lig->SetCol(g_vec3One * 0.3);
	lig = NewGO<LightDirection>(0);
	lig->SetDir({ 0,0.707,-0.707 });
	lig->SetCol(g_vec3One * 0.1);
	lig = NewGO<LightDirection>(0);
	lig->SetDir({ 0,0.707,0.707 });
	lig->SetCol(g_vec3One * 0.1);

	g_engine->GameLoop();
	
	return 0;
}


