#include <Inventor/Win/SoWin.h>
#include <Inventor/Win/SoWinRenderArea.h>

#include <Inventor/nodes/SoCube.h>
#include <Inventor/nodes/SoRotor.h>
#include <Inventor/nodes/SoArray.h>
#include <Inventor/nodes/SoDirectionalLight.h>
#include <Inventor/nodes/SoPerspectiveCamera.h>
#include <Inventor/nodes/SoSeparator.h>

// Set up a simple scenegraph, just for demonstration purposes.
static SoSeparator *
	get_scene_graph(void)
{
	SoSeparator * root = new SoSeparator;

	SoGroup * group = new SoGroup;

	SoRotor * rotor = new SoRotor;
	rotor->rotation = SbRotation(SbVec3f(0.2, 0.5, 0.9), M_PI/4.0);
	//rotor->rotation.getValue()
	group->addChild(rotor);

	SoCube * cube = new SoCube;
	group->addChild(cube);

	SoArray * array = new SoArray;
	array->origin = SoArray::CENTER;
	array->addChild(group);
	array->numElements1 = 2;
	array->numElements2 = 2;
	array->separation1 = SbVec3f(4, 0, 0);
	array->separation2 = SbVec3f(0, 4, 0);

	root->addChild(array);
	return root;
}

int
	main(int argc, char ** argv)
{
	HWND window = SoWin::init(argv[0]);

	SoSeparator * root = new SoSeparator;
	root->ref();

	SoPerspectiveCamera * camera;
	root->addChild(camera = new SoPerspectiveCamera);

	root->addChild(new SoDirectionalLight);

	SoSeparator * userroot = get_scene_graph();
	root->addChild(userroot);

	SoWinRenderArea * renderarea = new SoWinRenderArea(window);
	camera->viewAll(userroot, renderarea->getViewportRegion());
	renderarea->setSceneGraph(root);
	renderarea->setBackgroundColor(SbColor(0.0f, 0.2f, 0.3f));
	if (argc > 1) {
		renderarea->setTitle(argv[1]);
		renderarea->setIconTitle(argv[1]);
	}
	renderarea->show();

	SoWin::show(window);
	SoWin::mainLoop();

	delete renderarea;
	root->unref();

	return 0;
}
