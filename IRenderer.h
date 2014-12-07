#pragma once
#include "LSystemInstance.h"
namespace LSys
{
	class IRenderer
	{
	public:
		virtual void Initialise(octet::visual_scene *app_scene, const LSystemInstance::LSystemState *state) = 0;
		virtual void Rebuild(const LSystemInstance::LSystemState *state) = 0;
	};
}
