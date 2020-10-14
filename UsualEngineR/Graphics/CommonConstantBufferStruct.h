#pragma once



namespace UER
{
	/// <summary>
	/// カメラ用の定数バッファの構造体。
	/// </summary>
	struct SCameraCB
	{
		Vector4  pos;
		Vector4 dir;
		Matrix mView;		//ビュー行列。
		Matrix mProj;		//プロジェクション行列。
	};
	
}