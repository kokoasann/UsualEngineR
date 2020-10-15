#pragma once

//モデル用の定数バッファ
cbuffer ModelCb : register(b0){
	float4x4 mWorld;
	//float4x4 mView;
	//float4x4 mProj;
};