#pragma once
#define _CRT_SECURE_NO_WARNINGS

#define DEBUG_FUNC 1

#pragma comment( lib, "xinput.lib")
#pragma comment(lib, "dxcompiler.lib")

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <memory>
#include <math.h>
#include <vector>
#include <list>
#include <D3Dcompiler.h>
#include <Windows.h>
#include <d3d12.h>
#include <map>
#include <functional>
#include <algorithm>
#include <DirectXMath.h>
#include <Xinput.h>
#include <array>
#include <dxcapi.h>
#include <atlbase.h>

#include "d3dx12.h"
#include "math/Math.h"
#include "math/Vector.h"
#include "math/Matrix.h"

#include "UsualEngine.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "IShaderResource.h"
#include "IUnorderAccessResource.h"
#include "Texture.h"
#include "RWStructuredBuffer.h"
#include "Shader.h"
#include "PipelineState.h"
#include "ConstantBuffer.h"
#include "RootSignature.h"
#include "GraphicsEngine.h"
#include "DescriptorHeap.h"
#include "RenderTarget.h"

#include "RenderContext_inline.h"
#include "DescriptorHeap_inline.h"

#include "system/system.h"

#include "Sprite.h"
#include "Model.h"
#include "HID/GamePad.h"
#include "Font.h"

#include "util/GameTime.h"

#if DEBUG_FUNC
#include "Debug/Debug.h"
#include "Debug/DebugSwitch.h"
#endif

const UINT FRAME_BUFFER_W = 1280;				//フレームバッファの幅。
const UINT FRAME_BUFFER_H = 720;				//フレームバッファの高さ。
