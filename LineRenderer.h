#pragma once
#include "IRenderer.h"
#include "LSystemInstance.h"

#include "../../octet.h"
namespace LSys
{
	class LineRenderer : public IRenderer
	{
	public:

		octet::scene_node *node;
		octet::material *green;
		octet::mesh *mesh;
		LineRenderer()
		{
			green = new octet::material();
			mesh = new octet::mesh();
			node = new octet::scene_node();
		}

		~LineRenderer()
		{
		}

		void Render(LSystemInstance &system)
		{

		}
	};
}

