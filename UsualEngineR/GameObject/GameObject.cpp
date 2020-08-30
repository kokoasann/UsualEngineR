#include "PreCompile.h"
#include "GameObject.h"

UER::GameObject::GameObject()
{
}

UER::GameObject::~GameObject()
{
}

void UER::GameObject::WrapStart()
{
	if (!m_isStartGO && m_isActiveGO && !m_isDeadGO)
	{
		m_isStartGO = Start();
	}
}

void UER::GameObject::WrapPreUpdate()
{
	if (m_isStartGO && m_isActiveGO && !m_isDeadGO)
	{
		PreUpdate();
	}
}

void UER::GameObject::WrapUpdate()
{
	if (m_isStartGO && m_isActiveGO && !m_isDeadGO)
	{
		Update();
	}
}

void UER::GameObject::WrapPostUpdate()
{
	if (m_isStartGO && m_isActiveGO && !m_isDeadGO)
	{
		PostUpdate();
	}
}

void UER::GameObject::WrapRender()
{
	if (m_isStartGO && m_isActiveGO && !m_isDeadGO)
	{
		Render();
	}
}

void UER::GameObject::WrapPostRender()
{
	if (m_isStartGO && m_isActiveGO && !m_isDeadGO)
	{
		PostRender();
	}
}