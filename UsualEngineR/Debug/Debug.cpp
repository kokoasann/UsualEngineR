#include "PreCompile.h"
#include "Debug.h"
#include "DebugSwitch.h"

namespace UER
{
	void Debug::InitDebugSwitch()
	{
		std::function<void()> on = [&]()
		{
			debugState.isPhysicsDebugDraw = true;
			DebugPrint(L"�t�B�W�N�X�h���[ON");
			
		};
		std::function<void()> off = [&]()
		{
			debugState.isPhysicsDebugDraw = false;
			DebugPrint(L"�t�B�W�N�X�h���[OFF");
		};
		DebugSwitchAddCheck(DebugSwitchNewSwitch(VK_F1, 0, on, off));


#if 1
		DebugSwitchNewRadioBox("GBufferDraw");
		on = [&]()
		{
			gbufferDraw = gdDefault;
			DebugPrint(L"�f�t�H���g");
		};
		DebugSwitchAddRadioBoxButton("GBufferDraw", DebugSwitchNewSwitch(VK_F2, 0, on,DebugSwitch::nullFunc));
		on = [&]()
		{
			gbufferDraw = gdDiffuse;
			DebugPrint(L"G-Buffer �f�B�t���[�Y");
		};
		DebugSwitchAddRadioBoxButton("GBufferDraw", DebugSwitchNewSwitch(VK_F3, 0, on, DebugSwitch::nullFunc));
		on = [&]()
		{
			gbufferDraw = gdNormal;
			DebugPrint(L"G-Buffer �@��");
		};
		DebugSwitchAddRadioBoxButton("GBufferDraw", DebugSwitchNewSwitch(VK_F4, 0, on, DebugSwitch::nullFunc));
		on = [&]()
		{
			gbufferDraw = gdDepth;
			DebugPrint(L"G-Buffer �[�x�l");
		};
		DebugSwitchAddRadioBoxButton("GBufferDraw", DebugSwitchNewSwitch(VK_F5, 0, on, DebugSwitch::nullFunc));
		on = [&]()
		{
			gbufferDraw = gdShadow;
			DebugPrint(L"G-Buffer �e");
		};
		DebugSwitchAddRadioBoxButton("GBufferDraw", DebugSwitchNewSwitch(VK_F6, 0, on, DebugSwitch::nullFunc));
		on = [&]()
		{
			gbufferDraw = gdSpecular;
			DebugPrint(L"G-Buffer �X�y�L����");
		};
		DebugSwitchAddRadioBoxButton("GBufferDraw", DebugSwitchNewSwitch(VK_F7, 0, on, DebugSwitch::nullFunc));
#endif
	}
	void Debug::DebugPrint(const wchar_t* str)
	{
		m_log.Print(str);
	}
	void Debug::DrawLog()
	{
		m_log.Render();
	}

	
}