#include "stdafx.h"


#include "Game.h"
#include "Test/Test.h"


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


	//////////////////////////////////////
	// ���������s���R�[�h�������̂͂����܂ŁI�I�I
	//////////////////////////////////////
	//auto& renderContext = g_graphicsEngine->GetRenderContext();

	bool isInit = false;
	
	//NewGO<Test>(0);
	NewGO<Game>(0);
	auto lig = NewGO<LightDirection>(0);
	lig->SetDir({ 0.707,0.707,0 });
	lig->SetCol(g_vec3One * 1.5);
	lig = NewGO<LightDirection>(0);
	lig->SetDir({ -0.707,0.707,0 });
	lig->SetCol(g_vec3One * 1.5);

	g_engine->GameLoop();
	
	return 0;
}


