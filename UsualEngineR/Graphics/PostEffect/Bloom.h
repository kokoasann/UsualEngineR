#pragma once



namespace UER
{
	class Bloom
	{
	public:
		Bloom(){}
		~Bloom()
		{
			Release();
		}

		void Release();

		void Init();

		void Render();
	private:
		RenderTarget m_renderTarget;

		DescriptorHeap m_;
	};
}