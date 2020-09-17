#pragma once


class ThreadTest:public GameObject
{
public:
	// GameObject ‚ğ‰î‚µ‚ÄŒp³‚³‚ê‚Ü‚µ‚½
	virtual void Release() override;

	virtual void OnDestroy() override;

	bool Start() override;
	void Update() override;
private:
	std::unique_ptr<std::thread> m_thread;
	ModelRender* m_model;

	bool m_isOK = false;
	bool m_isEndThred = false;
};