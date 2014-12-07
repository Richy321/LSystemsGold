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
			uint32_t color;
		};

	public:

		octet::scene_node *node;
		octet::material *green;
		octet::mesh *mesh;

		std::map<char, RenderActionType> SymbolActionMap;
		LineRenderer()
		{
			green = new octet::material();
			mesh = new octet::mesh();
			node = new octet::scene_node();
		}

		~LineRenderer()
		{
		}

		void Initialise(octet::visual_scene *app_scene, const LSystemInstance *system)
		{
			node = new octet::scene_node();
			octet::param_shader *shader = new octet::param_shader("shaders/default.vs", "shaders/simple_color.fs");
			green = new octet::material(octet::vec4(0, 1, 0, 1), shader);
			mesh = new octet::mesh();
			mesh->set_mode(GL_LINES);
			Rebuild(system);

			size_t num_vertices = 1000;
			size_t num_indices = num_vertices * 2;

			mesh->allocate(sizeof(my_vertex) * num_vertices, sizeof(uint32_t) * num_indices);
			mesh->set_params(sizeof(my_vertex), num_indices, num_vertices, GL_LINES, GL_UNSIGNED_INT);
		
			// describe the structure of my_vertex to OpenGL
			mesh->add_attribute(octet::attribute_pos, 3, GL_FLOAT, 0);
			mesh->add_attribute(octet::attribute_color, 4, GL_UNSIGNED_BYTE, 12, GL_TRUE);

			app_scene->add_child(node);
			app_scene->add_mesh_instance(new octet::mesh_instance(node, mesh, green));
		}

		void Rebuild(const LSystemInstance *system)
		{
			const LSystemInstance::LSystemState *state = system->GetCurrentState();

			std::stack<Node*> *renderNodeStack = new std::stack<Node*>();

			octet::mat4t forward;
			forward.translate(0.0f, 1.0f, 0.0f);

			octet::mat4t turnAntiClockwise;
			turnAntiClockwise.rotateY(system->GetAngle());

			octet::mat4t turnClockwise;
			turnClockwise.rotateY(-system->GetAngle());

			const char *result = state->getResult();
			octet::mat4t curMatrix;

			octet::gl_resource::wolock vl(mesh->get_vertices());
			my_vertex *vtx = (my_vertex *)vl.u8();

			octet::gl_resource::wolock il(mesh->get_indices());
			uint32_t *idx = il.u32();

			//initial point
			vtx->pos = octet::vec3p(0.0f, 0.0f, 0.0f);
			vtx->color = make_color(0.0f, 1.0f, 0.0f);
			idx[0] = 0;
			idx[1] = 1;
			idx += 2;

			for (int i = 0; i < strlen(result); i++)
			{
				if (result[i] == GetDrawLineSymbol())
				{
					curMatrix = curMatrix * forward;
					AddPoint(vtx, idx, curMatrix);
				}
				else if (result[i] == GetTurnClockwiseSymbol())
				{
					curMatrix = curMatrix * turnClockwise;
				}
				else if (result[i] == GetTurnAntiClockwiseSymbol())
				{
					curMatrix = curMatrix * turnAntiClockwise;
				}
				else if (result[i] == GetStackPushSymbol())
				{
					Node* newNode = new Node(curMatrix);
					renderNodeStack->push(newNode);
				}
				else if (result[i] == GetStackPopSymbol())
				{
					renderNodeStack->pop();
				}
			}
		}

		void AddPoint(my_vertex *vtx, uint32_t *idx, octet::mat4t curMatrix)
		{
			octet::vec3p prevPos = vtx[-1].pos;
			vtx->pos = prevPos * curMatrix;
			vtx->color = make_color(0.0f, 1.0f, 0.0f);
			idx[0] = idx[-1] + 1;
			idx[1] = idx[-1] + 2;
			idx += 2;
		}

		static uint32_t make_color(float r, float g, float b) {
			return 0xff000000 + ((int)(r*255.0f) << 0) + ((int)(g*255.0f) << 8) + ((int)(b*255.0f) << 16);
		}

	};
}

