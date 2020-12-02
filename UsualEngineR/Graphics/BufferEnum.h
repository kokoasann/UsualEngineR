#pragma once



enum class EConstantBuffer : char
{
	cb_modelData,
	cb_cameraData,
	cb_lightData,
};

/// <summary>
/// 0~5はGBufferにも使います。
/// </summary>
enum class ETextureBuffer : char
{
	tb_albedo,
	tb_normal,
	tb_specular,
	tb_emmission,
	tb_bone			= 5,
	tb_dirLight		= 6,
	tb_pointLight	= 7,
};

