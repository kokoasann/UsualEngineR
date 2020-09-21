#include "stdafx.h"
#include "Test.h"
#include "ThreadTest.h"

void Test::Release()
{
	
}

void Test::OnDestroy()
{
}

void Test::Awake()
{
	m_threadObj.Execute([&]()
		{
			m_threadTest = NewGO<ThreadTest>(0);
		});
	
}
