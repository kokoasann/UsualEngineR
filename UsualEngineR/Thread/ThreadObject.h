#pragma once



namespace UER
{
	/// <summary>
	/// ����y��thread��񋟒v���܂��B
	/// �ʓ|�ȊJ���Ƃ����̑����낢������Ȃ��čς݂܂�!
	/// ��{�I�Ƀ����o�ϐ��ɂ�����ǉ�����Excute�֐����ĂԂ�����OK
	/// 
	/// �ĕĕ� ���̃N���X�͎g���܂킵�ł��܂���!! ������Release�֐����ĂԂƎg���܂킹�܂��B
	/// </summary>
	class ThreadObject
	{
	public:
		ThreadObject();
		~ThreadObject();

		/// <summary>
		/// �J���B
		/// �g���܂킵�����ꍇ���Ăт܂��傤�B
		/// </summary>
		void Release();

		/// <summary>
		/// ���s�B
		/// </summary>
		/// <param name="func">���s����֐�/param>
		void Execute(const std::function<void()>& func);

		/// <summary>
		/// �I�����?
		/// </summary>
		/// <returns></returns>
		bool IsEnd() const
		{
			return m_isEnd;
		}
	private:
		std::thread* m_thread = nullptr;	//�X���b�h
		std::function<void()> m_func;		//�X���b�h�Ŏg���֐��B

		bool m_isEnd = false;				//�I�����?

		bool m_isExecuted = false;			//���s����?
	};
}