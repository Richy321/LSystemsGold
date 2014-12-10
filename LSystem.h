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

#include <Shlwapi.h>
#pragma comment(lib, "shlwapi.lib")

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
		octet::camera_instance *camera;
		octet::random* rnd;

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


			rnd = new octet::random();

			camera = app_scene->get_camera_instance(0);
			camera->get_node()->translate(octet::vec3(0.0f, 12.0f, 0.0f));
			InitialiseSystems();

			renderer = new LineRenderer();
			renderer->Initialise(app_scene, systems[currentSystemIndex]);
		}

		octet::dynarray<char*> GetFilesInDirectory(const wchar_t *dirPath)
		{
			WIN32_FIND_DATA fdFile;
			HANDLE hFind = NULL;

			//TCHAR dest[MAX_PATH];
			//GetThisPath(dest, MAX_PATH);

			//DWORD a = GetCurrentDirectory(MAX_PATH, curDir);

			wchar_t path[MAX_PATH];
			wsprintf(path, L"%s\\*.txt", dirPath);

			octet::dynarray<char*> files;

			if((hFind = FindFirstFile(path, &fdFile)) == INVALID_HANDLE_VALUE)
			{
				wprintf(L"Lsystem path not found: [%s]\n", dirPath);
				return NULL;
			}

			do
			{
				if (wcscmp(fdFile.cFileName, L".") != 0 && wcscmp(fdFile.cFileName, L"..") != 0)
				{
					wsprintf(path, L"%s\\%s", dirPath, fdFile.cFileName);

					if (!(fdFile.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
					{
						char* file = new char[MAX_PATH];
						wcstombs(file, path, MAX_PATH);
						files.push_back(file);
					}
				}
			}
			while (FindNextFile(hFind, &fdFile)); //Find the next file. 

			return files;
		}

		TCHAR* GetThisPath(TCHAR* dest, size_t destSize)
		{
			if (!dest) return NULL;

			DWORD length = GetModuleFileName(NULL, dest, destSize);
			PathRemoveFileSpec(dest);
			return dest;
		}


		void InitialiseSystems()
		{
			//TODO - remove fixed path
			octet::dynarray<char*> files = GetFilesInDirectory(L"E:\\Projects\\octet\\octet\\src\\examples\\LSystem\\Systems");//"./src/examples/LSystem/systems");

			for (unsigned int i = 0; i < files.size(); i++)
			{
				LSystemInstance* sys = new LSystemInstance(files[i]);
				systems.push_back(sys);

				sys->Run();
				sys->currentStateIndex = sys->iterations - 1;
			}
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

			// clear to black
			glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			// draw the scene
			app_scene->render((float)vx / vy);
		}


		void HandleInput()
		{
			for (int i = 1; i <= 8; i++)
			{
				char num = '0' + i;

				if (is_key_going_down(num))
				{
					currentSystemIndex = i - 1;
					renderer->Rebuild(systems[currentSystemIndex]);
				}
			}

			if (is_key_going_down('P'))
			{
				if (systems[currentSystemIndex]->currentStateIndex < systems[currentSystemIndex]->iterations - 1)
				{
					systems[currentSystemIndex]->currentStateIndex++;
					renderer->Rebuild(systems[currentSystemIndex]);
				}
			}

			if (is_key_going_down('O'))
			{

				if (systems[currentSystemIndex]->currentStateIndex > 0)
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
			if (is_key_down('Q'))
			{
				camera->get_node()->translate(octet::vec3(0.0f, 0.0f, -cameraVelocity));
			}
			if (is_key_down('E'))
			{
				camera->get_node()->translate(octet::vec3(0.0f, 0.0f, cameraVelocity));
			}

			if (is_key_down('K'))
			{
				systems[currentSystemIndex]->distance -= 0.01f;
				renderer->Rebuild(systems[currentSystemIndex]);
			}
			if (is_key_down('L'))
			{
				systems[currentSystemIndex]->distance += 0.01f;
				renderer->Rebuild(systems[currentSystemIndex]);
			}

			if (is_key_down('H'))
			{
				systems[currentSystemIndex]->angle -= 1.0f;
				renderer->Rebuild(systems[currentSystemIndex]);
			}
			if (is_key_down('J'))
			{
				systems[currentSystemIndex]->angle += 1.0f;
				renderer->Rebuild(systems[currentSystemIndex]);
			}

			if (is_key_going_down('C'))
			{
				systems[currentSystemIndex]->colour.x() = rnd->get(0.0f, 1.0f);
				systems[currentSystemIndex]->colour.y() = rnd->get(0.0f, 1.0f);
				systems[currentSystemIndex]->colour.z() = rnd->get(0.0f, 1.0f);
				renderer->Rebuild(systems[currentSystemIndex]);
			}

		}
	};
}
