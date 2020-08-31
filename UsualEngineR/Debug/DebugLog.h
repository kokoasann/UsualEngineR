#pragma once


namespace UER
{
	class DebugLog
	{
	public:
		struct Log
		{
			std::wstring log;
			//const wchar_t* log = nullptr;
			float lifeTime = 0.0f;
		};
		DebugLog();
		~DebugLog();
		/// <summary>
		/// �����str��\�������B
		/// </summary>
		/// <param name="str"></param>
		void Print(const wchar_t* str);
		/// <summary>
		/// �`��B
		/// </summary>
		void Render();
	private:
		Font m_font;												//�t�H���g
		std::list<Log> m_printList;							//�\�����郍�O�̃��X�g
		Vector2 m_originPos = { -640,360 };			//���O�̕\���ʒu�B
		float m_lifeTime = 10.f;								//���O�̐������ԁB
		float m_firstFontSize = 0.7f;						//��ԏ�ɕ\�������t�H���g�̃T�C�Y
		float m_firstFontSpace = 30.f;						//��ԏ�ɕ\�������t�H���g�̊Ԋu�B
		float m_fontSize = 0.4f;								//�t�H���g�̃T�C�Y
		float m_fontSpace = 20.f;							//�t�H���g�̊Ԋu�B
		Vector4 m_fontColor = Vector4::White;	//�t�H���g�̐F�B
		int m_logMaxNum = 16;								//���O�̕\�������ő吔�B
	};
}