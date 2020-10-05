#pragma once



namespace UER
{
	class TextureCopy
	{
	public:

		TextureCopy(){}
		virtual ~TextureCopy()
		{
			Release();
		}
		void Release();


	private:

	};
}