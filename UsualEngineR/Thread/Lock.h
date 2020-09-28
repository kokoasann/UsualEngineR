#pragma once


namespace UER
{


	/// <summary>
	/// �}���`�X���b�h������ۂɕϐ���r�����䂷�邽�߂̃N���X
	/// 
	/// �R���X�g���N�^�̈����Ő��䂵�����C���X�^���X��n�����A
	/// Set�֐��ň����Ő��䂵�����C���X�^���X��n���Ώ��������B
	/// </summary>
	template<class T>
	class Lock
	{
	public:
		/// <summary>
		/// ���b�N����Ă�ϐ�������N���X
		/// </summary>
		struct Content
		{
			friend Lock;	//Content�̂��̂�Lock�̂���!

			/// <summary>
			/// �R�s�[�R���X�g���N�^
			/// </summary>
			/// <param name="c"></param>
			Content(const Content& c)
			{
				*this = c;
				lock->m_isUsing = true;
			}
			
			/// <summary>
			/// �R���X�g���N�^
			/// </summary>
			/// <param name="c">���e���̃|�C���^</param>
			/// <param name="lock">�Ǘ���</param>
			Content(T* c,Lock* lock)
			{
				this->lock = lock;
				content = c;
				lock->m_isUsing = true;
			}

			virtual ~Content()
			{
				lock->m_isUsing = false;
			}


			T* operator->()
			{
				return content;
			}
			
			T& Get()
			{
				return *content;
			}
		private:
			T* content = 0;	//���e���̃|�C���^�B
			Lock* lock;	//�Ǘ��ҁB
		};
		
		
		Lock() = delete;
		
		Lock(const Lock<T>& t) = default;
		Lock(T& content)
		{
			Set(&content);
		}
		Lock(T* content)
		{
			Set(content);
		}
		~Lock() {};


		/// <summary>
		/// ���䂵�Ă���ϐ���Ԃ�
		/// </summary>
		/// <returns></returns>
		Content Get()
		{
			while (m_isUsing)
			{
				Sleep(SLEEP_TIME);
			}
			m_isUsing = true;
			return m_contens;
		}
		/// <summary>
		/// �ǂݎ���p�B
		/// </summary>
		/// <returns></returns>
		const T& Get_ReadOnly() const
		{
			return *m_content;
		}

		/// <summary>
		/// ���e�����Z�b�g
		/// </summary>
		/// <param name="t"></param>
		void Set(T* t)
		{
			m_content = t;
			m_contens.content = t;
		}
	private:
		
	private:
		static const int SLEEP_TIME = 10;			//�Q�b�g�̎��Ƀ��b�N��Ԃ̏ꍇ�Ƀt���O�̍Ē���������܂ł̎��ԁB
		Content m_contens = Content(nullptr,this);	//���e��
		T* m_content;	//���e���̃|�C���^
		
		bool m_isUsing = false;	//�g���Ă�?
	};
}