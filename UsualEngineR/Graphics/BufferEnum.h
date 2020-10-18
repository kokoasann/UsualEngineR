#pragma once



enum class EConstantBuffer : char
{
	cb_modelData,
	cb_cameraData,
	cb_lightData,
};

/// <summary>
/// 0~5ÇÕGBufferÇ…Ç‡égÇ¢Ç‹Ç∑ÅB
/// </summary>
enum class ETextureBuffer : char
{
	tb_albedo,
	tb_normal,
	tb_specular,
	tb_bone			= 5,
	tb_dirLight		= 6,
	tb_pointLight	= 7,
};

