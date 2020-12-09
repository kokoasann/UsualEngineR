#pragma once



enum class EConstantBuffer : char
{
	cb_modelData,
	cb_cameraData,
	cb_lightData,
	cb_shadowData,
};

/// <summary>
/// 0~5ÇÕGBufferÇ…Ç‡égÇ¢Ç‹Ç∑ÅB
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
	tb_shadow1,
	tb_shadow2,
	tb_shadow3,
};

