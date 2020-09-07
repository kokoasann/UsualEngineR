#include "PreCompile.h"
#include "Shader.h"
#include <stierr.h>
#include <sstream>
#include <fstream>
#include <atlbase.h>


namespace UER
{
	
	
	namespace {
		const char* g_vsShaderModelName = "vs_5_0";	//頂点シェーダーのシェーダーモデル名。
		const char* g_psShaderModelName = "ps_5_0";	//ピクセルシェーダーのシェーダモデル名。
		const char* g_csShaderModelName = "cs_5_0";	//コンピュートシェーダーのシェーダーモデル名。
	}
	void Shader::Load(const wchar_t* filePath, const char* entryFuncName, const char* shaderModel)
	{
		ID3DBlob* errorBlob;
	#ifdef _DEBUG
		// Enable better shader debugging with the graphics debugging tools.
		UINT compileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
	#else
		UINT compileFlags = 0;
	#endif
		auto hr = D3DCompileFromFile(filePath, nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, entryFuncName, shaderModel, compileFlags, 0, &m_blob, &errorBlob);
		
		if (FAILED(hr)) {
			if (hr == STIERR_OBJECTNOTFOUND) {
				std::wstring errorMessage;
				errorMessage = L"指定されたfxファイルが開けませんでした";
				errorMessage += filePath;
				MessageBoxW(nullptr, errorMessage.c_str(), L"エラー", MB_OK);
			}
			if (errorBlob) {
				static char errorMessage[10 * 1024];
				sprintf_s(errorMessage, "filePath : %s, %s", "hoge", (char*)errorBlob->GetBufferPointer());
				MessageBoxA(NULL, errorMessage, "シェーダーコンパイルエラー", MB_OK);
				return;
			}
		}
	}
	void Shader::LoadPS(const wchar_t* filePath, const char* entryFuncName)
	{
		Load(filePath, entryFuncName, g_psShaderModelName);
	}
	void Shader::LoadVS(const wchar_t* filePath, const char* entryFuncName)
	{
		Load(filePath, entryFuncName, g_vsShaderModelName);
	}
	void Shader::LoadCS(const wchar_t* filePath, const char* entryFuncName)
	{
		Load(filePath, entryFuncName, g_csShaderModelName);
	}
	void Shader::LoadRaytracing(const wchar_t* filePath)
	{
		std::ifstream shaderFile(filePath);
		if (shaderFile.good() == false){
			std::wstring errormessage = L"シェーダーファイルのオープンに失敗しました。\n";
			errormessage += filePath;
			MessageBoxW( nullptr, errormessage.c_str(), L"エラー", MB_OK);
			std::abort();
		}
	
		std::stringstream strStream;
		strStream << shaderFile.rdbuf();
		std::string shader = strStream.str();
		//シェーダーのテキストファイルから、BLOBを作成する。
		CComPtr<IDxcLibrary> dxclib;
		auto hr = DxcCreateInstance(CLSID_DxcLibrary, IID_PPV_ARGS(&dxclib));
		if (FAILED(hr)) {
			MessageBox(nullptr, L"DXCLIBの作成に失敗しました", L"エラー", MB_OK);
			std::abort();
		}
		//dxcコンパイラの作成。
		CComPtr<IDxcCompiler> dxcCompiler;
		hr = DxcCreateInstance(CLSID_DxcCompiler, IID_PPV_ARGS(&dxcCompiler));
		if (FAILED(hr)) {
			MessageBox(nullptr, L"dxcコンパイラの作成に失敗しました", L"エラー", MB_OK);
			std::abort();
		}
		//ソースコードのBLOBを作成する。
		uint32_t codePage = CP_UTF8;
		CComPtr< IDxcBlobEncoding> sourceBlob;
		hr = dxclib->CreateBlobFromFile(filePath, &codePage, &sourceBlob);
		if (FAILED(hr)) {
			MessageBox(nullptr, L"シェーダーソースのBlobの作成に失敗しました", L"エラー", MB_OK);
			std::abort();
		}
		//コンパイル。
		CComPtr<IDxcOperationResult> result;
		hr = dxcCompiler->Compile(
			sourceBlob, // pSource
			filePath, // pSourceName
			L"",		// pEntryPoint
			L"lib_6_3", // pTargetProfile
			nullptr, 0, // pArguments, argCount
			nullptr, 0, // pDefines, defineCount
			nullptr, // pIncludeHandler
			&result); // ppResult
		if (SUCCEEDED(hr)) {
			result->GetStatus(&hr);
		}
	
		if (FAILED(hr))
		{
			if (result)
			{
				CComPtr<IDxcBlobEncoding> errorsBlob;
				hr = result->GetErrorBuffer(&errorsBlob);
				if (SUCCEEDED(hr) && errorsBlob)
				{
					std::string errormessage = "Compilation failed with errors:\n%hs\n";
					errormessage += (const char*)errorsBlob->GetBufferPointer();
					MessageBoxA(nullptr, errormessage.c_str(), "エラー", MB_OK);
	
				}
			}
			// Handle compilation error...
		}
		else {
			result->GetResult(&m_dxcBlob);
		}
	}
	


}
