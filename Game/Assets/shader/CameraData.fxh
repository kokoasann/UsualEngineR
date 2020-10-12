#pragma once

cbuffer CameraCB:register(b2)
{
    float4 cam_Pos;
    float4 cam_Dir;
    float4x4 cam_mView;
	float4x4 cam_mProj;
}