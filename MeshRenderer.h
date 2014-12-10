#pragma once
#include "IRenderer.h"

namespace LSys
{
	class MeshRenderer : public IRenderer
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

		MeshRenderer()
		{
		}

		~MeshRenderer()
		{
		}

		void Initialise(octet::visual_scene *app_scene, const LSystemInstance *system)
		{


		}
		
		void Rebuild(const LSystemInstance *system)
		{


		}
	};
}
