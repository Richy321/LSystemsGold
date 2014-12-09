#pragma once

#include <stack>

#include "IRenderer.h"
#include "LSystemInstance.h"
#include "Node.h"
#include "Tree.h"
#include "../../octet.h"

namespace LSys
{
	class LineRenderer : public IRenderer
	{

	private:
		struct my_vertex 
		{
			octet::vec3p pos;
		};

	public:

		octet::ref<octet::scene_node>node;
		octet::ref<octet::material> green;
		octet::ref<octet::mesh>mesh;

		std::map<char, RenderActionType> SymbolActionMap;
		LineRenderer()
		{
		}

		~LineRenderer()
		{
		}

		void Initialise(octet::visual_scene *app_scene, const LSystemInstance *system)
		{
			node = new octet::scene_node();
			octet::param_shader *shader = NULL;//new octet::param_shader("shaders/default.vs", "shaders/simple_color.fs");
			green = new octet::material(octet::vec4(0, 1, 0, 1), shader);
			mesh = new octet::mesh();
			mesh->set_params(sizeof(my_vertex), 0, 0, GL_LINES, NULL);
			mesh->add_attribute(octet::attribute_pos, 3, GL_FLOAT, 0);
			mesh->add_attribute(octet::attribute_color, 4, GL_UNSIGNED_BYTE, 12, GL_TRUE);
			Rebuild(system);
		
			app_scene->add_child(node);
			app_scene->add_mesh_instance(new octet::mesh_instance(node, mesh, green));
		}

		void Rebuild(const LSystemInstance *system)
		{
			const LSystemInstance::LSystemState *state = system->GetCurrentState();

			size_t num_vertices = 100000;
			size_t num_indices = num_vertices * 2;
			
			octet::dynarray<my_vertex> verts;

			std::stack<octet::mat4t> *renderNodeStack = new std::stack<octet::mat4t>();
			octet::mat4t identMat;
			renderNodeStack->push(identMat);

			const char *result = state->getResult();

			printf("result:%s", result);

			octet::mat4t matrix;

			for (int i = 0; i < strlen(result); i++)
			{
				if (result[i] == GetDrawLineSymbol() || result[i] == GetDrawLineSymbol2())
				{
					my_vertex vert;

					vert.pos = renderNodeStack->top()[3].xyz();
					verts.push_back(vert);

					my_vertex vertTop;

					renderNodeStack->top().translate(octet::vec3(0.0f, translateAmount, 0.0f));
					vertTop.pos = renderNodeStack->top()[3].xyz();
					verts.push_back(vertTop);

				}
				else if (result[i] == GetTurnClockwiseSymbol())
				{
					renderNodeStack->top().rotateZ(-system->GetAngle());
				}
				else if (result[i] == GetTurnAntiClockwiseSymbol())
				{
					renderNodeStack->top().rotateZ(system->GetAngle());
				}
				else if (result[i] == GetStackPushSymbol())
				{
					matrix = renderNodeStack->top();
					renderNodeStack->push(matrix);
				}
				else if (result[i] == GetStackPopSymbol())
				{
					renderNodeStack->pop();
				}
			}
			mesh->set_vertices(verts);
		}

	};
}

