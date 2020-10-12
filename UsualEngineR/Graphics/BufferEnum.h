#pragma once

#define TO_INT(en) static_cast<int>(en)
#define TO_UINT(en) static_cast<unsigned int>(en)

enum class EConstantBuffer : char
{
	cb_modelData,
	cb_cameraData,
	cb_lightData,
};

enum class ETextureBuffer: char
{
	tb_albedo,
	tb_normal,
	tb_specular,

};