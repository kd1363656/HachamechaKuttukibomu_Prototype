#pragma once

namespace CommonConstant
{
	constexpr Math::Vector3 STANDARD_SCALE = { 1.0f , 1.0f , 1.0f };

	constexpr float MOVE_DIRECTION_EPSILON = 0.0001f;
	constexpr float DOT_PRODUCT_MIN        = -1.0f;
	constexpr float DOT_PRODUCT_MAX        =  1.0f;
	constexpr float ALL_DEGREE             = 360.0f;
	constexpr float MAX_SCREEN_WIDTH       = 1280.0f;
	constexpr float MAX_SCREEN_HEIGHT      = 760.0f;
}