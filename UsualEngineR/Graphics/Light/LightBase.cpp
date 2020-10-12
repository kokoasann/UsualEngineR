#include "PreCompile.h"
#include "LightBase.h"
#include "LightManager.h"

namespace UER
{
	bool LightBase::Start()
	{
		
		g_graphicsEngine->GetLightManager().AddLight(this);
		return true;
	}
	void LightBase::OnDestroy()
	{
		g_graphicsEngine->GetLightManager().RmvLight(this);
	}
}