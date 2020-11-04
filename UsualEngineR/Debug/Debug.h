#pragma once
#include "DebugLog.h"

namespace UER
{
	/// <summary>
	/// �V���O���g��
	/// �f�o�b�O�p�̃N���X
	/// </summary>
	class Debug
	{
		Debug(){}
	public:
		~Debug(){}
		static Debug& Instance()
		{
			static Debug ins;
			return ins;
		}
		/// <summary>
		/// debugSwitch�̏������B
		/// </summary>
		void InitDebugSwitch();
		struct DebugState
		{
			bool isPhysicsDebugDraw = false;
		};
		DebugState debugState;
		enum GBufferDraw
		{
			gdDefault,
			gdDiffuse,
			gdNormal,
			gdDepth,
			gdShadow,
			gdSpecular,
		};
		GBufferDraw gbufferDraw = gdDefault;

		/// <summary>
		/// DebugLog�ɕ\�����镶������́B
		/// </summary>
		/// <param name="str"></param>
		void DebugPrint(const wchar_t* str);
		/// <summary>
		/// ���O�̕`��B
		/// </summary>
		void DrawLog();
	private:
		DebugLog m_log;		//�f�o�b�O���O
	};
	/// <summary>
	/// �f�o�b�O�v�����g
	/// </summary>
	/// <param name="str"></param>
	static void DebugPrint(const wchar_t* str)
	{
		Debug::Instance().DebugPrint(str);
	}


	
}