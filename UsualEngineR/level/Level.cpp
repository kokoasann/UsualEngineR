#include "PreCompile.h"
#include "Level.h"
#include "MapChip.h"

using namespace std;
namespace UER
{
	Level::Level()
	{
	}
	Level::~Level()
	{
	}

	void Level::Init(const char* levelDataFilePath, Level::HookWhenBuildObjectFunc hookFunc)
	{
		//�X�P���g�������[�h����B
		Skeleton skeleton;
		skeleton.Init(levelDataFilePath);
		//�\�z�\�z�B
		//0�Ԗڂ̓��[�g�I�u�W�F�N�g�Ȃ̂Ŕ�΂��B
		for (auto i = 1; i < skeleton.GetNumBones(); i++) {
			//�����擾�B
			auto bone = skeleton.GetBone(i);
			if (bone->GetNo() == 0) {	//�e�����[�g�̏ꍇ�����}�b�v�`�b�v�𐶐�����B
				LevelObjectData objData;
				Vector3 scale;
				bone->CalcWorldTRS(objData.position, objData.rotation, objData.scale);
				//3dsMax�Ƃ͎����Ⴄ�̂ŁA�␳������B
				auto t = objData.position.y;
				objData.position.y = objData.position.z;
				objData.position.z = -t;

				t = objData.rotation.y;
				objData.rotation.y = objData.rotation.z;
				objData.rotation.z = -t;
				objData.name = bone->GetName();
				bool isHook = false;
				if (hookFunc != nullptr) {
					//hook�֐����w�肳��Ă���̂�hook�֐����Ăяo���B
					isHook = hookFunc(objData);
				}
				if (isHook == false) {
					//�t�b�N����Ȃ������̂ŁA�}�b�v�`�b�v���쐬����B
					auto mapChip = std::make_unique<MapChip>(objData);
					m_mapChipArray.push_back(std::move(mapChip));
				}
			}
		}
	}
	void Level::Draw()
	{
		for (auto& mapChip : m_mapChipArray) {
			mapChip->Draw();
		}
	}
}