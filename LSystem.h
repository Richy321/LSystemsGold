////////////////////////////////////////////////////////////////////////////////
//
// (C) Andy Thomason 2012-2014
//
// Modular Framework for OpenGLES2 rendering on multiple platforms.
//
#include "../../octet.h"
#include "IRenderer.h"
#include "LineRenderer.h"
#include "Rule.h"
#include "LSystemInstance.h"

namespace LSys 
{
  /// Scene containing a box with octet.
  class LSystem : public octet::app 
  {
    // scene for drawing box
    octet::ref<octet::visual_scene> app_scene;
	octet::dynarray<LSystemInstance> systems;

	int currentSystemIndex = 0;
	
	IRenderer *renderer = nullptr;


  public:
    /// this is called when we construct the class before everything is initialised.
    LSystem(int argc, char **argv) : app(argc, argv) {
    }

    /// this is called once OpenGL is initialized
    void app_init() 
	{
      app_scene =  new octet::visual_scene();
      app_scene->create_default_camera_and_lights();

	  InitialiseSystems();

	  renderer = new LineRenderer();
    }

	void InitialiseSystems()
	{
		LSystemInstance* A = new LSystemInstance("src/examples/LSystem/systems/A.txt");
		systems.push_back(*A);
	}


    /// this is called to draw the world
    void draw_world(int x, int y, int w, int h) 
	{
      int vx = 0, vy = 0;
      get_viewport_size(vx, vy);
      app_scene->begin_render(vx, vy);

      // update matrices. assume 30 fps.
      app_scene->update(1.0f/30);

	  renderer->Render(systems[currentSystemIndex]);

      // draw the scene
      app_scene->render((float)vx / vy);
    }
  };
}
