#pragma once


namespace UER
{
	
	class ConstantBuffer;
	class Texture;
	class DescriptorHeap;
	class RenderTarget;
	/// <summary>
	/// レンダリングコンテキスト。
	/// </summary>
	class RenderContext {
	public:
		/// <summary>
		/// 初期化。
		/// </summary>
		/// <param name="commandList">コマンドリスト。</param>
		void Init(ID3D12GraphicsCommandList4* commandList)
		{
			m_commandList = commandList;
		}

		ID3D12GraphicsCommandList4* GetCommandList()
		{
			return m_commandList;
		}
	
		/// <summary>
		/// 頂点バッファを設定。
		/// </summary>
		/// <param name="vb">頂点バッファ。</param>
		void SetVertexBuffer(const VertexBuffer& vb)
		{
			m_commandList->IASetVertexBuffers(0, 1, &vb.GetView());
		}
		/// <summary>
		/// インデックスバッファを設定。
		/// </summary>
		/// <param name="ib"></param>
		void SetIndexBuffer(const IndexBuffer& ib)
		{
			m_commandList->IASetIndexBuffer(&ib.GetView());
		}
		/// <summary>
		/// プリミティブのトポロジーを設定。
		/// </summary>
		/// <remarks>
		/// ID3D12GraphicsCommandList::のIASetPrimitiveTopologyのラッパー関数。
		/// 詳細はMicrosoftのヘルプを参照。
		/// </remarks>
		void SetPrimitiveTopology(D3D12_PRIMITIVE_TOPOLOGY topology)
		{
			m_commandList->IASetPrimitiveTopology(topology);
		}
		/// <summary>
		/// コマンドリストを設定。
		/// </summary>
		/// <param name="commandList">コマンドリスト。</param>
		void SetCommandList(ID3D12GraphicsCommandList4* commandList)
		{
			m_commandList = commandList;
		}
		/// <summary>
		/// ビューポートを設定
		/// </summary>
		/// <param name="viewport">ビューポート</param>
		void SetViewport(D3D12_VIEWPORT& viewport)
		{
			m_commandList->RSSetViewports(1, &viewport);
		}
		/// <summary>
		/// シザリング矩形を設定
		/// </summary>
		/// <param name="rect"></param>
		void SetScissorRect(D3D12_RECT& rect)
		{
			m_commandList->RSSetScissorRects(1, &rect);
		}
	
		/// <summary>
		/// ルートシグネチャを設定。
		/// </summary>
		void SetRootSignature(ID3D12RootSignature* rootSignature)
		{
			m_commandList->SetGraphicsRootSignature(rootSignature);
		}
		void SetRootSignature(RootSignature& rootSignature)
		{
			m_commandList->SetGraphicsRootSignature(rootSignature.Get());
		}
		void SetComputeRootSignature(ID3D12RootSignature* rootSignature)
		{
			m_commandList->SetComputeRootSignature(rootSignature);
		}
		void SetComputeRootSignature(RootSignature& rootSignature)
		{
			m_commandList->SetComputeRootSignature(rootSignature.Get());
		}
		/// <summary>
		/// パイプラインステートを設定。
		/// </summary>
		void SetPipelineState(ID3D12PipelineState* pipelineState)
		{
			m_commandList->SetPipelineState(pipelineState);
		}
		void SetPipelineState(PipelineState& pipelineState)
		{
			m_commandList->SetPipelineState(pipelineState.Get());
		}
		void SetPipelineState1(ID3D12StateObject* pipelineState)
		{
			m_commandList->SetPipelineState1(pipelineState);
		}
		/// <summary>
		/// ディスクリプタヒープを設定。
		/// </summary>
		void SetDescriptorHeap(ID3D12DescriptorHeap* descHeap)
		{
			m_descriptorHeaps[0] = descHeap;
			m_commandList->SetDescriptorHeaps(1, m_descriptorHeaps);
		}
		void SetDescriptorHeap(DescriptorHeap& descHeap);
		void SetComputeDescriptorHeap(DescriptorHeap& descHeap);
		/// <summary>
		/// 定数バッファを設定。
		/// </summary>
		/// <param name="registerNo">設定するレジスタの番号。</param>
		/// <param name="cb">定数バッファ。</param>
		void SetConstantBuffer(int registerNo, ConstantBuffer& cb)
		{
			if (registerNo < MAX_CONSTANT_BUFFER) {
				m_constantBuffers[registerNo] = &cb;
			}
			else {
				//範囲外アクセス。
				std::abort();
			}
		}
		/// <summary>
		/// シェーダーリソースを設定。
		/// </summary>
		/// <param name="registerNo">設定するレジスタの番号。</param>
		/// <param name="texture">テクスチャ。</param>
		void SetShaderResource(int registerNo, Texture& texture)
		{
			if (registerNo < MAX_SHADER_RESOURCE) {
				m_shaderResources[registerNo] = &texture;
			}
			else {
				//範囲外アクセス。
				std::abort();
			}
		}
		void SetDescriptorHeaps(int numDescriptorHeap, ID3D12DescriptorHeap* descHeaps[])
		{
			for (int i = 0; i < numDescriptorHeap; i++) {
				m_descriptorHeaps[i] = descHeaps[i];
			}
			m_commandList->SetDescriptorHeaps(numDescriptorHeap, m_descriptorHeaps);
		}

		void SetGraphicsRootDescriptorTable(UINT rootParameterIndex, DescriptorHeap& descHeap)
		{
			m_commandList->SetGraphicsRootDescriptorTable(rootParameterIndex, descHeap.Get()->GetGPUDescriptorHandleForHeapStart());
		}
		/// <summary>
		/// レンダリングターゲットとビューポートを同時に設定する。
		/// </summary>
		/// <param name="renderTarget"></param>
		void SetRenderTarget(D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle, D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle)
		{
			m_commandList->OMSetRenderTargets(1, &rtvHandle, FALSE, &dsvHandle);
		}
		/// <summary>
		/// レンダリングターゲットとビューポートを同時に設定する。
		/// </summary>
		/// <param name="numRT"></param>
		/// <param name="renderTarget"></param>
		void SetRenderTargets(UINT numRT, RenderTarget* renderTargets[]);

		void SetRenderTargets(UINT numRT, RenderTarget rts[]);
		/// <summary>
		/// レンダリングターゲットビューのクリア。
		/// </summary>
		/// <param name="renderTarget">レンダリングターゲット</param>
		/// <param name="clearColor">クリアカラー</param>
		void ClearRenderTargetViews(
			int numRt, 
			RenderTarget* renderTargets[]
		);
		void ClearRenderTargetView(D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle, const float* clearColor)
		{
			m_commandList->ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr);
		}
		
		/// <summary>
		/// デプスステンシルビューをクリア
		/// </summary>
		/// <param name="renderTarget">レンダリングターゲット</param>
		/// <param name="clearValue">クリア値</param>
		void ClearDepthStencilView(D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle, float clearValue)
		{
			m_commandList->ClearDepthStencilView(
				dsvHandle,
				D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL,
				clearValue,
				0,
				0,
				nullptr);
		}
		/// <summary>
		/// レンダリングターゲットへの描き込み待ち。
		/// </summary>
		/// <remarks>
		/// レンダリングターゲットとして使われているテクスチャをシェーダーリソースビューとして
		/// 使用したい場合は、この関数を使って描き込み完了待ちを行う必要があります。
		/// </remarks>
		/// <param name="renderTarget">レンダリングターゲット</param>
		void WaitUntilFinishDrawingToRenderTargets(int numRt, RenderTarget* renderTargets[]);
		void WaitUntilFinishDrawingToRenderTargets(int numRt, RenderTarget renderTargets[]);
		void WaitUntilFinishDrawingToRenderTarget(RenderTarget& renderTarget);
		void WaitUntilFinishDrawingToRenderTarget( ID3D12Resource* renderTarget )
		{
			auto r = CD3DX12_RESOURCE_BARRIER::Transition(
				renderTarget,
				D3D12_RESOURCE_STATE_RENDER_TARGET,
				D3D12_RESOURCE_STATE_PRESENT);
			m_commandList->ResourceBarrier(
				1,
				&r);
		}
		/// <summary>
		/// レンダリングターゲットとして使用可能になるまで待つ。
		/// </summary>
		/// <remarks>
		/// レンダリングターゲットとして設定したい場合は、
		/// 本関数を使って使用可能になるまで待機する必要があります。
		/// </remarks>
		void WaitUntilToPossibleSetRenderTargets(int numRt, RenderTarget* renderTargets[]);
		void WaitUntilToPossibleSetRenderTargets(int numRt, RenderTarget renderTargets[]);
		void WaitUntilToPossibleSetRenderTarget(RenderTarget& renderTarget);
		void WaitUntilToPossibleSetRenderTarget( ID3D12Resource* renderTarget)
		{
			auto r = CD3DX12_RESOURCE_BARRIER::Transition(renderTarget, D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);
			m_commandList->ResourceBarrier(
				1,
				&r
			);
		}
		void WaitUAV(RWStructuredBuffer& uav)
		{
			auto r = CD3DX12_RESOURCE_BARRIER::UAV(uav.GetD3DResoruce());
			m_commandList->ResourceBarrier(
				1,
				&r
			);
		}

		/// <summary>
		/// リソースバリア。
		/// </summary>
		/// <param name="barrier"></param>
		void ResourceBarrier(D3D12_RESOURCE_BARRIER& barrier)
		{
			m_commandList->ResourceBarrier(1, &barrier);
		}
		/// <summary>
		/// リソースステートを遷移する。
		/// </summary>
		/// <param name="resrouce"></param>
		/// <param name="beforeState"></param>
		/// <param name="afterState"></param>
		void TransitionResourceState(ID3D12Resource* resrouce , D3D12_RESOURCE_STATES beforeState, D3D12_RESOURCE_STATES afterState)
		{
			auto r = CD3DX12_RESOURCE_BARRIER::Transition(resrouce, beforeState, afterState);
			m_commandList->ResourceBarrier(
				1,
				&r
			);
		}
		/// <summary>
		/// コマンドリストを閉じる
		/// </summary>
		void Close()
		{
			m_commandList->Close();
		}
		/// <summary>
		/// コマンドリストをリセット。
		/// </summary>
		/// <param name="commandAllocator"></param>
		/// <param name="pipelineState"></param>
		void Reset( ID3D12CommandAllocator* commandAllocator, ID3D12PipelineState* pipelineState)
		{
			m_commandList->Reset(commandAllocator, pipelineState);
		}
		/// <summary>
		/// インデックスつきプリミティブを描画。
		/// </summary>
		/// <param name="indexCount">インデックスの数。</param>
		void DrawIndexed(UINT indexCount)
		{
			m_commandList->DrawIndexedInstanced(indexCount, 1, 0, 0, 0);
		}

		/// <summary>
		/// インデックスつきプリミティブを描画。
		/// </summary>
		/// <param name="indexCount">インデックスの数。</param>
		void DrawIndexedInstanced(
			UINT indexCount,
			UINT InstanceCount,
			UINT StartIndexLocation,
			INT BaseVertexLocation,
			UINT StartInstanceLocation)
		{
			m_commandList->DrawIndexedInstanced(indexCount, InstanceCount, StartIndexLocation, BaseVertexLocation, StartInstanceLocation);
		}
		/// <summary>
		/// コンピュートシェーダーをディスパッチ。
		/// </summary>
		/// <param name="ThreadGroupCountX"></param>
		/// <param name="ThreadGroupCountY"></param>
		/// <param name="ThreadGroupCountZ"></param>
		void Dispatch( 
			UINT ThreadGroupCountX,
			UINT ThreadGroupCountY,
			UINT ThreadGroupCountZ )
		{
			m_commandList->Dispatch(ThreadGroupCountX, ThreadGroupCountY, ThreadGroupCountZ);
		}
		/// <summary>
		/// GPUでレイトレーシングアクセラレーション構造のビルドを行います。
		/// </summary>
		void BuildRaytracingAccelerationStructure(D3D12_BUILD_RAYTRACING_ACCELERATION_STRUCTURE_DESC desc)
		{
			m_commandList->BuildRaytracingAccelerationStructure(&desc, 0, nullptr);
		}
		/// <summary>
		/// レイをディスパッチ
		/// </summary>
		/// <param name="rayDesc"></param>
		void DispatchRays(D3D12_DISPATCH_RAYS_DESC& rayDesc)
		{
			m_commandList->DispatchRays(&rayDesc);
		}
		/// <summary>
		/// リソースをコピー。
		/// </summary>
		/// <param name="pDst">コピー先のリソース</param>
		/// <param name="pSrc">コピー元のリソース</param>
		void CopyResource(ID3D12Resource* pDst, ID3D12Resource* pSrc)
		{
			m_commandList->CopyResource(pDst, pSrc);
		}
	private:
	
		/// <summary>
		/// ディスクリプタテーブルを設定。
		/// </summary>
		/// <param name="RootParameterIndex"></param>
		/// <param name="BaseDescriptor"></param>
		void SetGraphicsRootDescriptorTable(
			UINT RootParameterIndex,
			D3D12_GPU_DESCRIPTOR_HANDLE BaseDescriptor)
		{
			m_commandList->SetGraphicsRootDescriptorTable(
				RootParameterIndex,
				BaseDescriptor
			);
		}
		/// <summary>
		/// ディスクリプタテーブルを設定。
		/// </summary>
		/// <param name="RootParameterIndex"></param>
		/// <param name="BaseDescriptor"></param>
		void SetComputeRootDescriptorTable(
			UINT RootParameterIndex,
			D3D12_GPU_DESCRIPTOR_HANDLE BaseDescriptor)
		{
			m_commandList->SetComputeRootDescriptorTable(
				RootParameterIndex,
				BaseDescriptor
			);
		}
	private:
		enum { MAX_DESCRIPTOR_HEAP = 4 };	//ディスクリプタヒープの最大数。
		enum { MAX_CONSTANT_BUFFER = 8 };	//定数バッファの最大数。足りなくなったら増やしてね。
		enum { MAX_SHADER_RESOURCE = 16 };	//シェーダーリソースの最大数。足りなくなったら増やしてね。
	
		ID3D12GraphicsCommandList4* m_commandList;	//コマンドリスト。
		ID3D12DescriptorHeap* m_descriptorHeaps[MAX_DESCRIPTOR_HEAP];			//ディスクリプタヒープの配列。
		ConstantBuffer* m_constantBuffers[MAX_CONSTANT_BUFFER] = { nullptr };	//定数バッファの配列。
		Texture* m_shaderResources[MAX_SHADER_RESOURCE] = { nullptr };			//シェーダーリソースの配列。
	};
	


}
