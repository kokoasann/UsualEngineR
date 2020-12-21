#pragma once


namespace UER
{
//#if DEBUG_FUNC
#if 1
	class DebugSwitch
	{
	private:
		DebugSwitch(){}
	public:
		static std::function<void()> nullFunc;
		~DebugSwitch() 
		{
			for (auto s : m_radioButton)
				delete s;
			for (auto s : m_checkButton)
				delete s;
			for (auto it = m_radioBoxs.begin(); it != m_radioBoxs.end(); it++)
			{
				for (auto s : it->second->box)
					delete s;
				delete it->second;
			}
		}

		static DebugSwitch* Instance()
		{
			static DebugSwitch ds;
			return &ds;
		}
		/// <summary>
		/// �X�C�b�`�̍\���́B
		/// key��2�ݒ肳��Ă�����͓̂�����������Ɣ�������B
		/// </summary>
		struct SSwitch
		{
			SSwitch(char k1, char k2)
			{
				key1 = k1;
				key2 = k2;
			}
			/// <summary>
			/// 
			/// </summary>
			/// <param name="k1">key1</param>
			/// <param name="k2">key2</param>
			/// <param name="update">�L���ɂȂ��Ă���ԌĂ΂��֐�</param>
			SSwitch(char k1, char k2, const std::function<void()>& update)
			{
				key1 = k1;
				key2 = k2;
				updateFunc = update;
			}

			/// <summary>
			/// 
			/// </summary>
			/// <param name="k1">key1</param>
			/// <param name="k2">key2</param>
			/// <param name="on">�L���ɂȂ����u�ԌĂ΂��֐�</param>
			/// <param name="off">�����ɂȂ����u�ԌĂ΂��֐�</param>
			SSwitch(char k1, char k2, const std::function<void()>& on, const std::function<void()>& off)
			{
				key1 = k1;
				key2 = k2;
				triggerOnFunc = on;
				triggerOffFunc = off;
			}

			/// <summary>
			/// 
			/// </summary>
			/// <param name="k1">key1</param>
			/// <param name="k2">key2</param>
			/// <param name="update">�L���ɂȂ��Ă���ԌĂ΂��֐�</param>
			/// <param name="on">�L���ɂȂ����u�ԌĂ΂��֐�</param>
			/// <param name="off">�����ɂȂ����u�ԌĂ΂��֐�</param>
			SSwitch(char k1, char k2, const std::function<void()>& update, const std::function<void()>& on, const std::function<void()>& off)
			{
				key1 = k1;
				key2 = k2;
				updateFunc = update;
				triggerOnFunc = on;
				triggerOffFunc = off;
			}
			char key1 = 0;		//key1
			char key2 = 0;		//key2
			bool isPushed = false;			//�����ꂽ
			bool oldFrameIsPushed = false;	//�O�̃t���[���ɉ�����Ă���
			std::function<void()> updateFunc = nullFunc;		//�L���ɂȂ��Ă���ԌĂ΂��֐��B
			std::function<void()> triggerOnFunc = nullFunc;		//�L���ɂȂ����u�ԌĂ΂��֐��B
			std::function<void()> triggerOffFunc = nullFunc;	//�����ɂȂ����u�ԌĂ΂��֐��B
		};
		/// <summary>
		/// ���W�I�{�^���݂�����
		/// </summary>
		struct SRadioBox
		{
			std::vector<SSwitch*> box;		//�X�C�b�`
		};

		/// <summary>
		/// �X�V�B
		/// </summary>
		void Update();

		/// <summary>
		/// ��ł����{�^���̒ǉ��B
		/// </summary>
		/// <param name="s"></param>
		void AddRadioButton(SSwitch* s)
		{
			m_radioButton.push_back(s);
		}
		/// <summary>
		/// �`�F�b�N�{�^���̒ǉ��B
		/// </summary>
		/// <param name="s"></param>
		void AddCheckButton(SSwitch* s)
		{
			m_checkButton.push_back(s);
		}
		/// <summary>
		/// ���W�I�{�b�N�X��ǉ��B
		/// </summary>
		/// <param name="name">�{�b�N�X�̖��O</param>
		void NewRadioBox(const char* name)
		{
			m_radioBoxs.insert({ name, new SRadioBox() });
		}
		/// <summary>
		/// ���W�I�{�b�N�X�Ƀ{�^����ǉ��B
		/// </summary>
		/// <param name="name">�{�b�N�X�̖��O</param>
		/// <param name="s">�X�C�b�`</param>
		void AddRadioBoxButton(const char* name, SSwitch* s)
		{
			auto it = m_radioBoxs.find(name);
			it->second->box.push_back(s);
		}

		void AddButton(SSwitch* s)
		{
			m_button.push_back(s);
		}
	private:
		std::vector<SSwitch*> m_radioButton;			//���W�I�{�^������
		std::vector<SSwitch*> m_checkButton;			//�`�F�b�N�{�^������
		std::map<std::string, SRadioBox*> m_radioBoxs;	//���W�I�{�b�N�X����
		std::vector<SSwitch*> m_button;
	};
#endif
	/// <summary>
	/// �X�C�b�`�̍X�V�B
	/// </summary>
	static void DebugSwitchUpdate()
	{
#if DEBUG_FUNC
		DebugSwitch::Instance()->Update();
#endif
	}

	/// <summary>
	/// �X�C�b�`�̍쐬
	/// </summary>
	/// <param name="k1">key1</param>
	/// <param name="k2">key2</param>
	/// <param name="update">ON�̎����t���[���Ă΂��֐�</param>
	/// <returns></returns>
	static void* DebugSwitchNewSwitch(char k1, char k2, const std::function<void()>& update)
	{
#if DEBUG_FUNC
		return new DebugSwitch::SSwitch(k1, k2, update);
#else
		return nullptr;
#endif
	}

	/// <summary>
	/// �X�C�b�`�̍쐬
	/// </summary>
	/// <param name="k1">key1</param>
	/// <param name="k2">key2</param>
	/// <param name="on">ON�ɂȂ����u�ԌĂ΂��֐�</param>
	/// <param name="off">OFF�ɂȂ����u�ԌĂ΂��֐�</param>
	/// <returns></returns>
	static void* DebugSwitchNewSwitch(char k1, char k2,
		const std::function<void()>& on,
		const std::function<void()>& off)
	{
#if DEBUG_FUNC
		return new DebugSwitch::SSwitch(k1, k2, on,off);
#else
		return nullptr;
#endif
	}

	/// <summary>
	/// 
	/// </summary>
	/// <param name="k1">key1</param>
	/// <param name="k2">key2</param>
	/// <param name="update">ON�̎����t���[���Ă΂��֐�</param>
	/// /// <param name="on">ON�ɂȂ����u�ԌĂ΂��֐�</param>
	/// <param name="off">OFF�ɂȂ����u�ԌĂ΂��֐�</param>
	/// <returns></returns>
	static void* DebugSwitchNewSwitch(char k1, char k2,
		const std::function<void()>& update,
		const std::function<void()>& on,
		const std::function<void()>& off)
	{
#if DEBUG_FUNC
		return new DebugSwitch::SSwitch(k1, k2, update, on, off);
#else
		return nullptr;
#endif
	}

	static void* DebugSwitchNewSwitch_Button(char k1, char k2, const std::function<void()>& on)
	{
#if DEBUG_FUNC
		auto s = new DebugSwitch::SSwitch(k1, k2);
		s->triggerOnFunc = on;
		return s;
#else
		return nullptr;
#endif
	}

	/// <summary>
	/// �`�F�b�N�{�^���̒ǉ��B
	/// </summary>
	/// <param name="s">�X�C�b�`�̃|�C���^</param>
	static void DebugSwitchAddCheck(void* s)
	{
#if DEBUG_FUNC
		DebugSwitch::SSwitch* sw = reinterpret_cast<DebugSwitch::SSwitch*>(s);
		DebugSwitch::Instance()->AddCheckButton(sw);
#endif
	}

	/// <summary>
	/// ���W�I�{�^���ǉ��B
	/// </summary>
	/// <param name="s">�X�C�b�`�̃|�C���^</param>
	static void DebugSwitchAddRadio(void* s)
	{
#if DEBUG_FUNC
		DebugSwitch::Instance()->AddRadioButton(reinterpret_cast<DebugSwitch::SSwitch*>(s));
#endif
	}

	static void DebugSwitchAddButton(void* s)
	{
#if DEBUG_FUNC
		DebugSwitch::Instance()->AddButton(reinterpret_cast<DebugSwitch::SSwitch*>(s));
#endif
	}

	/// <summary>
	/// ���W�I�{�b�N�X�̒ǉ��B
	/// </summary>
	/// <param name="name">�{�b�N�X�̖��O</param>
	static void DebugSwitchNewRadioBox(const char* name)
	{
#if DEBUG_FUNC
		DebugSwitch::Instance()->NewRadioBox(name);
#endif
	}

	/// <summary>
	/// ���W�I�{�b�N�X�Ƀ{�^����ǉ��B
	/// </summary>
	/// <param name="name">�{�b�N�X�̖��O</param>
	/// <param name="s">�{�^���̃|�C���^</param>
	static void DebugSwitchAddRadioBoxButton(const char* name, void* s)
	{
#if DEBUG_FUNC
		DebugSwitch::Instance()->AddRadioBoxButton(name, reinterpret_cast<DebugSwitch::SSwitch*>(s));
#endif
	}
}