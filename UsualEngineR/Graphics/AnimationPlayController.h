/*!
 * @brief	�A�j���[�V�����Đ��R���g���[���B
 */

#pragma once


namespace UER{
	class CSkeleton;
	class CAnimationClip;

	/*!
	 * @brief	�A�j���[�V�����̍Đ��R���g���[���B
	 * @details
	 *  ��̃A�j���[�V�����N���b�v�ɑ΂��ăA�j���[�V������i�߂āA���[�J���|�[�Y���v�Z���܂��B
	 */
	class CAnimationPlayController{
	public:
		/*!
		 * @brief	�R���X�g���N�^�B
		 */
		CAnimationPlayController()
		{
		}
		/*!
		 * @brief	�f�X�g���N�^�B
		 */
		~CAnimationPlayController()
		{
		}
		/// <summary>
		/// ������
		/// </summary>
		/// <param name="skeleton">�X�P���g��</param>
		/// <param name="footStepBoneNo">footstep�{�[���̔ԍ�</param>
		void Init(CSkeleton* skeleton, int footStepBoneNo);
		/*!
		 * @brief	�A�j���[�V�����N���b�v�̕ύX�B
		 */
		void ChangeAnimationClip(CAnimationClip* clip)
		{
			m_animationClip = clip;
			m_currentKeyFrameNo = 0;
			m_time = 0.0f;
			m_isPlaying = true;
			m_footstepPos = g_vec3Zero;
			m_footstepDeltaValue = g_vec3Zero;
			//�A�j���[�V�����C�x���g�̔����t���O�����ׂ�false�ɂ���B
			auto& animEventArray = m_animationClip->GetAnimationEvent();
			for (auto i = 0; i < m_animationClip->GetNumAnimationEvent(); i++) {
				animEventArray[i].SetInvokedFlag(false);
			}
		}
		void SetInterpolateTime(float interpolateTime)
		{
			if (interpolateTime < 0.0f) {
				TK_WARNING("interpolateTime is negative!!");
			}
			m_interpolateEndTime = interpolateTime;
			m_interpolateTime = 0.0f;
		}
		/*!
		 *@brief	�⊮�����擾�B
		 */
		float GetInterpolateRate() const
		{
			if (m_interpolateEndTime <= 0.0f) {
				return 1.0f;
			}
			return min( 1.0f, m_interpolateTime / m_interpolateEndTime );
		}
		/*!
		 * @brief	�A�j���[�V������i�߂�B
		 *@param[in]	deltaTime		�A�j���[�V������i�߂鎞�ԁB
		 */
		void Update(float deltaTime, CAnimation* animation);
		/*!
		 * @brief	���[�J���{�[���s����擾�B
		 */
		const std::vector<CMatrix>& GetBoneLocalMatrix() const
		{
			return m_boneMatrix;
		}
		CAnimationClip* GetAnimClip() const
		{
			return m_animationClip;
		}
		/*!
		* @brief	�Đ����H
		*/
		bool IsPlaying() const
		{
			return m_isPlaying;
		}
		/// <summary>
		/// Update�֐����Ăяo�����Ƃ��́Afootstep�{�[���̈ړ��ʂ��擾�B
		/// </summary>
		/// <returns></returns>
		CVector3 GetFootStepDeltaValueOnUpdate() const
		{
			return m_footstepDeltaValue;
		}
	private:
		/*!
		*@brief	�A�j���[�V�����C�x���g���N������B
		*/
		void InvokeAnimationEvent(CAnimation* animation);
		/*!
		*@brief	���[�v�Đ��J�n���鎞�̏����B
		*/
		void StartLoop();
		/// <summary>
		/// ���[�g�̃{�[����Ԃł̃{�[���s����v�Z����B
		/// </summary>
		/// <param name="bone">�v�Z���鍜</param>
		/// <param name="parentMatrix">�e�̍s��</param>
		void CalcBoneMatrixInRootBoneSpace(CBone& bone, CMatrix parentMatrix);
		/// <summary>
		/// �{�[���s����A�j���[�V�����N���b�v����T���v�����O����B
		/// </summary>
		void SamplingBoneMatrixFromAnimationClip();
		/// <summary>
		/// ���[�g�{�[����Ԃł̍s����v�Z����B
		/// </summary>
		void CalcBoneMatrixInRootBoneSpace();
		/// <summary>
		/// footstep�{�[���̕ω��ʂ��T���v�����O����B
		/// </summary>
		void SamplingDeltaValueFootstepBone();
		/// <summary>
		/// footstep�{�[���̍��W��S�̂̍����猸�Z����B
		/// </summary>
		void SubtractFootstepbonePosFromAllBone();
		/// <summary>
		/// �L�[�t���[���ԍ���i�߂�B
		/// </summary>
		void ProgressKeyframeNo(float deltaTime);
	private:
		CAnimationClip*			m_animationClip = nullptr;			//�A�j���[�V�����N���b�v�B
		int						m_currentKeyFrameNoLastFrame = 0;	//��t���[���O�̃L�[�t���[���ԍ��B
		int						m_currentKeyFrameNo = 0;			//���ݍĐ����̃L�[�t���[���ԍ��B
		float					m_time = 0.0f;
		std::vector<CMatrix>	m_boneMatrix;						//!<���̃R���g���[���ōĐ����̃A�j���[�V�����̃{�[���s��B
		float					m_interpolateTime;					//!<�⊮����
		float					m_interpolateEndTime;				//!<�⊮�I������
		bool					m_isPlaying = false;				//!<�Đ����H
		CSkeleton*				m_skeleton = nullptr;				//!<�X�P���g���B
		CVector3				m_footstepDeltaValue = g_vec3Zero;	//�t�b�g�X�e�b�v�̈ړ��x�N�g���B
		CVector3				m_footstepPos = g_vec3Zero;			//�t�b�g�X�e�b�v�{�[���̍��W�B
		int						m_footstepBoneNo = -1;				//�t�b�g�X�e�b�v�̃{�[���̔ԍ��B
	};
}