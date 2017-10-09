#pragma once

#include <glm\glm.hpp>

namespace RGBColors
{
	using namespace glm;

	// You can define more (and prettier) colors by referencing RGB color table in http://www.rapidtables.com/web/color/RGB_Color.htm
	// NOTE divide by 255.0f converts 0~255 colors to 0.0f ~ 1.0f colors
	const vec3 red = vec3(255.0, 0.0, 0.0) / 255.0f;
	const vec3 green = vec3(0.0, 255.0, 0.0) / 255.0f;
	const vec3 blue = vec3(0.0, 0.0, 255.0) / 255.0f;
	const vec3 gray(vec3(128, 128, 128) / 255.0f);
	const vec3 yellow(vec3(255, 255, 0) / 255.0f);
	const vec3 olive(vec3(128, 128, 0) / 255.0f);
	const vec3 black(vec3(0, 0, 0) / 255.0f);
	const vec3 white(vec3(1, 1, 1) / 255.0f);
}

