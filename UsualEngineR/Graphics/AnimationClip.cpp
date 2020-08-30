/*!
 *@brief	�A�j���[�V�����N���b�v�B
 */

#include "PreCompile.h"
#include "AnimationClip.h"

namespace UER {

	CAnimationClip::~CAnimationClip()
	{
	}

	void CAnimationClip::Load(const char* filePath)
	{
		m_tkaFile.Load(filePath);
		if(m_tkaFile.IsLoaded() == false){
		
			TK_WARNING_MESSAGE_BOX_W(L"�A�j���[�V�����N���b�v�̃I�[�v���Ɏ��s���܂����B�t�@�C���p�X : %s\n"
									 L"�����Ƃ��āA���L�̂Q�_���l�����܂��B\n"
									 L"�@ �t�@�C���p�X���ԈႦ�Ă���B\n"
									 L"�A Assets�t�H���_�̒��Ƀt�@�C�������݂��Ȃ��B\n"
									 L"���̂Q�_���m�F���āA��肪���݂��Ȃ��ꍇ�́A��xVisualStudio�̃r���h/���r���h���s���Ă݂Ă��������B\n", filePath);
			
			return;
		}
	}

	void CAnimationClip::BuildKeyFramesAndAnimationEvents()
	{
		//�A�j���[�V�����C�x���g�̍\�z�B
		auto numAnimEvent = m_tkaFile.GetNumAnimationEvent();
		if (numAnimEvent > 0) {
			m_animationEvent = std::make_unique<CAnimationEvent[]>(numAnimEvent);
			int eventNo = 0;
			m_tkaFile.QueryAnimationEvents([&](const CTkaFile::AnimationEvent & animEvent) {
				static wchar_t wEventName[256];

				mbstowcs(wEventName, animEvent.eventName.c_str(), 255);
				m_animationEvent[eventNo].SetInvokeTime(animEvent.invokeTime);
				m_animationEvent[eventNo].SetEventName(wEventName);
				eventNo++;
				});

		}
		//�L�[�t���[�����̍\�z�B
		m_keyframes.reserve(m_tkaFile.GetNumKeyFrame());
		m_tkaFile.QueryKeyFrames([&](const CTkaFile::KeyFrame & tkaKeyFrame) {
			auto keyframe = std::make_unique<KeyFrame>();
			keyframe->boneIndex = tkaKeyFrame.boneIndex;
			keyframe->transform = CMatrix::Identity;
			keyframe->time = tkaKeyFrame.time;
			for (auto j = 0; j < 4; j++) {
				keyframe->transform.m[j][0] = tkaKeyFrame.transform[j].x;
				keyframe->transform.m[j][1] = tkaKeyFrame.transform[j].y;
				keyframe->transform.m[j][2] = tkaKeyFrame.transform[j].z;
			}
			m_keyframes.push_back(std::move(keyframe));
			});
		//�{�[���C���f�b�N�X���Ƃ̃L�[�t���[���̘A�����X�g���쐬����B
		m_keyFramePtrListArray.resize(MAX_BONE);
		for (auto& keyframe : m_keyframes) {
			m_keyFramePtrListArray[keyframe->boneIndex].push_back(keyframe.get());
			if (m_topBoneKeyFramList == nullptr) {
				m_topBoneKeyFramList = &m_keyFramePtrListArray[keyframe->boneIndex];
			}
		}
	}
}

