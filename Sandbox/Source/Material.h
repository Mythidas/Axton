#pragma once

#include "Axton.h"

using namespace Axton;

struct Material
{
	float Specular{ 0.5f };
	float Metallic{ 0.5f };
	float Roughness{ 0.5f };
	float Emissive{ 0.5f };
	alignas(16) Vector3 EmissiveHue{ 0.0f };
};