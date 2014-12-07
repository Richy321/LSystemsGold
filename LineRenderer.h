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

		void Initialise(octet::visual_scene *app_scene, const LSystemInstance::LSystemState *state)
		{
			node = new octet::scene_node();
			octet::param_shader *shader = new octet::param_shader("shaders/default.vs", "shaders/simple_color.fs");
			green = new octet::material(octet::vec4(0, 1, 0, 1), shader);
			mesh = new octet::mesh();
			mesh->set_mode(GL_LINES);
			Rebuild(state);

			app_scene->add_child(node);
			app_scene->add_mesh_instance(new octet::mesh_instance(node, mesh, green));
		}

		void Rebuild(const LSystemInstance::LSystemState *state)
		{
			const char *result = state->getResult();
			for (int i = 0; i < strlen(result); i++)
			{
				//if(
			}
		}


		void AddBranchToNode(octet::scene_node node)
		{

		}

		void AddLineToNode(octet::scene_node node)
		{

		}

		void RotateNode(octet::scene_node node, float angle)
		{

		}


	};
}

