/*!
 *@brief	�A�j���[�V�����N���b�v�B
 */

#pragma once

#include "TKFile/TkaFile.h"

namespace UER{
	
	/*!
	 *@brief	�L�[�t���[���B
	 */
	struct KeyFrame {
		uint32_t boneIndex;	//!<�{�[���C���f�b�N�X�B
		float time;					//!<���ԁB
		CMatrix transform;			//!<�g�����X�t�H�[���B
	};
	
	/*!
	*@brief	�A�j���[�V�����C�x���g�B
	*/
	class CAnimationEvent : Noncopyable {
	public:
		
		/*!
		*@brief	�C�x���g�������Ԃ�ݒ�B
		*/
		float GetInvokeTime() const
		{
			return m_invokeTime;
		}
		/*!
		*@brief	�C�x���g�����擾�B
		*/
		const wchar_t* GetEventName() const
		{
			return m_eventName.c_str();
		}
		/*!
		*@brief	�C�x���g�������Ԃ�ݒ�B
		*/
		void SetInvokeTime(float time)
		{
			m_invokeTime = time;
		}
		/*!
		*@brief	�C�x���g����ݒ�B
		*/
		void SetEventName(const wchar_t* name)
		{
			m_eventName = name;
		}
		/*!
		*@brief	�C�x���g�������ς݂�����B
		*/
		bool IsInvoked() const
		{
			return m_isInvoked;
		}
		/*!
		*@brief	�C�x���g�������ς݂̃t���O��ݒ肷��B
		*/
		void SetInvokedFlag(bool flag)
		{
			m_isInvoked = flag;
		}
	private:
		bool m_isInvoked = false;	//!<�C�x���g�����ς݁H
		float m_invokeTime = 0.0f;	//!<�C�x���g�������ԁB
		wstring m_eventName;	//!<�C�x���g���B
	};
	/*!
	 *@brief	�A�j���[�V�����N���b�v�B
	 */
	class CAnimationClip : Noncopyable {
	public:
		
		using keyFramePtrList = vector<KeyFrame*>;
		/*!
		* @brief	�R���X�g���N�^
		*/
		CAnimationClip()
		{
		}
		/*!
		 *@brief	�f�X�g���N�^�B
		 */
		~CAnimationClip();
		/// <summary>
		/// �A�j���[�V�����N���b�v�𓯊����[�h�B
		/// </summary>
		/// <param name="filePath"></param>
		void Load(const char* filePath);
		/// <summary>
		/// �񓯊����[�h�B
		/// IsLoaded�֐����g�p���āA�������Ƃ�悤�ɂ��Ă��������B
		/// </summary>
		/// <param name="filePath"></param>
		void LoadAsync(const char* filePath)
		{
			m_tkaFile.LoadAsync(filePath);
		}
		/// <summary>
		/// �ǂݍ��݂��I����Ă��邩����B
		/// </summary>
		/// <returns></returns>
		bool IsLoaded() const
		{
			return m_tkaFile.IsLoaded();
		}
		/// <summary>
		/// �L�[�t���[���ƃA�j���[�V�����C�x���g���\�z����B
		/// </summary>
		/// <remarks>
		/// ���[�h���I������Ƃ��ɌĂяo���Ă��������B
		/// </remarks>
		void BuildKeyFramesAndAnimationEvents();
		/*!
		*@brief	���[�v����H
		*/
		bool IsLoop() const
		{
			return m_isLoop;
		}
		/*!
		*@brief	���[�v�t���O��ݒ肷��B
		*/
		void SetLoopFlag(bool flag)
		{
			m_isLoop = flag;
		}
		/*!
		*@brief
		*/
		const vector<keyFramePtrList>& GetKeyFramePtrListArray() const
		{
			return m_keyFramePtrListArray;
		}
		const keyFramePtrList& GetTopBoneKeyFrameList() const
		{
			return *m_topBoneKeyFramList;
		}
		/*!
		 *@brief	�N���b�v�����擾�B
		 */
		const wchar_t* GetName() const
		{
			return m_clipName.c_str();
		}
		/*!
		*@brief	�A�j���[�V�����C�x���g���擾�B
		*/
		std::unique_ptr<CAnimationEvent[]>& GetAnimationEvent()
		{
			return m_animationEvent;
		}

		/// <summary>
		/// �A�j���[�V�����C�x���g�̐����擾
		/// </summary>
		/// <returns></returns>
		int GetNumAnimationEvent() const
		{
			return m_tkaFile.GetNumAnimationEvent();
		}
	private:
		using KeyframePtr = unique_ptr<KeyFrame>;
		std::wstring m_clipName;	//!<�A�j���[�V�����N���b�v�̖��O�B
		bool m_isLoop = false;	//!<���[�v�t���O�B
		vector<KeyframePtr>					m_keyframes;				//�L�[�t���[���B
		vector<keyFramePtrList>				m_keyFramePtrListArray;		//�{�[�����Ƃ̃L�[�t���[���̃��X�g���Ǘ����邽�߂̔z��B
		unique_ptr<CAnimationEvent[]>		m_animationEvent;			//�A�j���[�V�����C�x���g�B
		int									m_numAnimationEvent = 0;	//�A�j���[�V�����C�x���g�̐��B
		keyFramePtrList*					m_topBoneKeyFramList = nullptr;
		CTkaFile							m_tkaFile;			//tka�t�@�C��
	};
	using CAnimationClipPtr = std::unique_ptr<CAnimationClip>;
}