#pragma once
#include "LSystemInstance.h"
namespace LSys
{
	class IRenderer
	{
	public:
		virtual void Render(LSystemInstance &system) = 0;
	};
}
