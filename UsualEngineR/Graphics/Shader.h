#pragma once


namespace UER
{
	
	
	class Shader {
	public:
		/// <summary>
		/// ピクセルシェーダーをロード。
		/// </summary>
		/// <param name="filePath">ファイルパス。</param>
		/// <param name="entryFuncName">エントリーポイントの関数名。</param>
		void LoadPS(const wchar_t* filePath, const char* entryFuncName);
		/// <summary>
		/// 頂点シェーダーをロード。
		/// </summary>
		/// <param name="filePath">ファイルパス</param>
		/// <param name="entryFuncName">エントリーポイントの関数名。</param>
		void LoadVS(const wchar_t* filePath, const char* entryFuncName);
		/// <summary>
		/// コンピュートシェーダーをロード。
		/// </summary>
		/// <param name="filePath"></param>
		/// <param name="entryFuncName"></param>
		void LoadCS(const wchar_t* filePath, const char* entryFuncName);
		/// <summary>
		/// レイトレーシング用のシェーダーをロード。
		/// </summary>
		void LoadRaytracing(const wchar_t* filePath);
		/// <summary>
		/// コンパイル済みシェーダーデータを取得。
		/// </summary>
		/// <returns></returns>
		ID3DBlob* GetCompiledBlob() const
		{
			return m_blob;
		}
		IDxcBlob* GetCompiledDxcBlob() const
		{
			return m_dxcBlob;
		}
	private:
		/// <summary>
		/// シェーダーをロード。
		/// </summary>
		/// <param name="filePath">ファイルパス</param>
		/// <param name="entryFuncName">エントリーポイントの関数名。</param>
		/// <param name="shaderModel">シェーダーモデル</param>
		void Load(const wchar_t* filePath, const char* entryFuncName, const char* shaderModel);
	private:
		ID3DBlob*	m_blob = nullptr;	//コンパイル済みのシェーダーデータ。
		IDxcBlob* m_dxcBlob = nullptr;	//DXCコンパイラを使用したときのシェーダーデータ。
	};
	


}
