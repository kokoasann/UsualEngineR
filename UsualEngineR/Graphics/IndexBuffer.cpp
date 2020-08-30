#include "PreCompile.h"
#include "IndexBuffer.h"


namespace UER
{
	
	
	
	
	IndexBuffer::~IndexBuffer()
	{
		if (m_indexBuffer) {
			m_indexBuffer->Release();
		}
	}
	void IndexBuffer::Init(int size, int stride)
	{
		auto d3dDevice = g_graphicsEngine->GetD3DDevice();
		auto hr = d3dDevice->CreateCommittedResource(
			&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
			D3D12_HEAP_FLAG_NONE,
			&CD3DX12_RESOURCE_DESC::Buffer(size),
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&m_indexBuffer));
		
		//インデックスバッファのビューを作成。
		m_indexBufferView.BufferLocation = m_indexBuffer->GetGPUVirtualAddress();
		m_indexBufferView.SizeInBytes = size;
		if (stride == 2) {
			//ストライドが２バイト。
			m_indexBufferView.Format = DXGI_FORMAT_R16_UINT;
		}
		else if (stride == 4) {
			//ストライドが４バイト。
			m_indexBufferView.Format = DXGI_FORMAT_R32_UINT;
		}
		m_count = size / stride;
	}
	void IndexBuffer::Copy(void* srcIndecies)
	{
		uint8_t* pData;
		m_indexBuffer->Map(0, nullptr, (void**)&pData);
		memcpy(pData, srcIndecies, m_indexBufferView.SizeInBytes);
		m_indexBuffer->Unmap(0, nullptr);
	}

}
