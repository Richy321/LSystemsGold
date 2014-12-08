////////////////////////////////////////////////////////////////////////////////
//
// (C) Andy Thomason 2012-2014
//
// Modular Framework for OpenGLES2 rendering on multiple platforms.
//
#include "../../octet.h"
#include "IRenderer.h"
#include "LineRenderer.h"
#include "LSystemInstance.h"

namespace LSys 
{
  /// Scene containing a box with octet.
	class LSystem : public octet::app
	{
		// scene for drawing box
		octet::ref<octet::visual_scene> app_scene;
		octet::dynarray<LSystemInstance*> systems;

		int currentSystemIndex = 0;

		IRenderer *renderer = nullptr;

		bool isKeyDownLastFrame = false;

		octet::camera_instance *camera;

	public:
		/// this is called when we construct the class before everything is initialised.
		LSystem(int argc, char **argv) : app(argc, argv)
		{
		}

		/// this is called once OpenGL is initialized
		void app_init()
		{
			app_scene = new octet::visual_scene();
			app_scene->create_default_camera_and_lights();

			camera = app_scene->get_camera_instance(0);

			InitialiseSystems();

			renderer = new LineRenderer();
			renderer->Initialise(app_scene, systems[currentSystemIndex]);
		}

		void InitialiseSystems()
		{
			LSystemInstance* A = new LSystemInstance("src/examples/LSystem/systems/A.txt");
			systems.push_back(A);

			A->Run();
			A->currentStateIndex = A->iterations - 1;;
		}


		/// this is called to draw the world
		void draw_world(int x, int y, int w, int h)
		{
			int vx = 0, vy = 0;
			get_viewport_size(vx, vy);
			app_scene->begin_render(vx, vy);

			// update matrices. assume 30 fps.
			app_scene->update(1.0f / 30);

			HandleInput();

			// draw the scene
			app_scene->render((float)vx / vy);
		}


		void HandleInput()
		{
			bool keyDown = false;
			for (int i = 1; i <= 8; i++)
			{
				char num = '0' + i;

				if (is_key_down(num))
				{
					keyDown = true;
					if (!isKeyDownLastFrame)
					{
						currentSystemIndex = i - 1;
						renderer->Rebuild(systems[currentSystemIndex]);
					}
				}
			}

			if (is_key_down('>'))
			{
				keyDown = true;
				if (!isKeyDownLastFrame && systems[currentSystemIndex]->currentStateIndex < systems[currentSystemIndex]->iterations - 2)
				{
					systems[currentSystemIndex]->currentStateIndex++;
					renderer->Rebuild(systems[currentSystemIndex]);
				}
			}

			if (is_key_down('<'))
			{
				keyDown = true;

				if (!isKeyDownLastFrame && systems[currentSystemIndex]->currentStateIndex > 0)
				{
					systems[currentSystemIndex]->currentStateIndex--;
					renderer->Rebuild(systems[currentSystemIndex]);
				}
			}

			float cameraVelocity = 1.0f;

			if (is_key_down('W'))
			{
				camera->get_node()->translate(octet::vec3(0.0f, cameraVelocity, 0.0f));
			}
			if (is_key_down('S'))
			{
				camera->get_node()->translate(octet::vec3(0.0f, -cameraVelocity, 0.0f));
			}
			if (is_key_down('A'))
			{
				camera->get_node()->translate(octet::vec3(-cameraVelocity, 0.0f, 0.0f));
			}
			if (is_key_down('D'))
			{
				camera->get_node()->translate(octet::vec3(cameraVelocity, 0.0f, 0.0f));
			}

			isKeyDownLastFrame = keyDown;
		}
	};
}
