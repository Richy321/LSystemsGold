#pragma once
#include "LSystemInstance.h"
namespace LSys
{
	enum RenderActionType
	{
		Draw_Line,
		Stack_Push,
		Stack_Pop,
		Turn_AntiClockwise,
		Turn_Clockwise
	};

	class IRenderer
	{
	public:
		virtual void Initialise(octet::visual_scene *app_scene, const LSystemInstance *system) = 0;
		virtual void Rebuild(const LSystemInstance *system) = 0;

		const char GetDrawLineSymbol() const { return 'F';}
		const char GetStackPushSymbol() const { return '['; }
		const char GetStackPopSymbol() const { return ']'; }
		const char GetTurnAntiClockwiseSymbol() const { return '+'; }
		const char GetTurnClockwiseSymbol() const { return '-'; }
	};
}
