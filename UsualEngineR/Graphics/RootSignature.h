#pragma once


namespace UER
{
	
	class Shader;
	class RootSignature {
	public:
		~RootSignature()
		{
			if (m_rootSignature) {
				m_rootSignature->Release();
			}
		}
		/// <summary>
		/// 初期化
		/// </summary>
		/// <param name="samplerFilter">サンプラフィルタ</param>
		/// <param name="textureAdressModeU">テクスチャのアドレッシングモード(U方向)</param>
		/// <param name="textureAdressModeV">テクスチャのアドレッシングモード(V方向)</param>
		/// <param name="textureAdressModeW">テクスチャのアドレッシングモード(W方向)</param>
		/// <param name="maxCbvDescriptor">定数バッファ用のディスクリプタの最大数</param>
		/// <param name="maxSrvDescriptor">シェーダーリソース用のディスクリプタの最大数</param>
		/// <param name="maxUavDescritor">UAV用のディスクリプタの最大数</param>
		bool Init(
			D3D12_FILTER samplerFilter,
			D3D12_TEXTURE_ADDRESS_MODE textureAdressModeU,
			D3D12_TEXTURE_ADDRESS_MODE textureAdressModeV,
			D3D12_TEXTURE_ADDRESS_MODE textureAdressModeW,
			UINT maxCbvDescriptor = 8,
			UINT maxSrvDescriptor = 32,
			UINT maxUavDescritor = 8
		);
		bool Init(Shader& shader);
		ID3D12RootSignature* Get()
		{
			return m_rootSignature;
		}
	private:
		ID3D12RootSignature* m_rootSignature = nullptr;	//ルートシグネチャ。
	};

}
