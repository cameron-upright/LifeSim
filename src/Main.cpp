#ifdef __APPLE__

#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>

#else

#include <GL/glut.h>    // Header File For The GLUT Library 
#include <GL/gl.h>	// Header File For The OpenGL32 Library
#include <GL/glu.h>	// Header File For The GLu32 Library

#endif

#include <stdio.h>
#include <unistd.h>     // needed to sleep
#include <math.h>
#include <iostream>

#include "g_axis.h"
#include "fpscounter.h"
#include "timer.h"

#include "UserInputManager.h"

#include "Scene.h"
#include "SceneVis.h"

#include "RLExperiment.h"


using namespace std;


#define ESCAPE 27

Timer timer;

int window; 

bool b_left_button, b_right_button, b_middle_button, b_shift;
int mouse_prev_x, mouse_prev_y;
int window_width=800, window_height=600;

SceneVis *sceneVis;

UserInputManager *userInputManager;

FPScounter fps;

RLExperiment *experiment;

void InitGL(int Width, int Height)	        // We call this right after our OpenGL window is created.
{
  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);		// This Will Clear The Background Color To Black
  glClearDepth(1.0);				// Enables Clearing Of The Depth Buffer
  glDepthFunc(GL_LESS);			        // The Type Of Depth Test To Do
  glEnable(GL_DEPTH_TEST);		        // Enables Depth Testing
  glShadeModel(GL_SMOOTH);			// Enables Smooth Color Shading

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();				// Reset The Projection Matrix

  gluPerspective(45.0f,(GLfloat)Width/(GLfloat)Height,0.1f,500.0f);	// Calculate The Aspect Ratio Of The Window
  glEnable(GL_LINE_SMOOTH);
  glMatrixMode(GL_MODELVIEW);

	glEnable(GL_MULTISAMPLE);

	/*
  int num;
  glGetIntegerv(GL_MAX_TEXTURE_UNITS, &num);

	glGetIntegerv (GL_SAMPLES_ARB, &num);
	printf ("number of samples is %d\n", num);
	*/
}

/* The function called when our window is resized (which shouldn't happen, because we're fullscreen) */
void ReSizeGLScene(int Width, int Height)
{
  if (Height==0)				// Prevent A Divide By Zero If The Window Is Too Small
    Height=1;

  glViewport(0, 0, Width, Height);		// Reset The Current Viewport And Perspective Transformation

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  gluPerspective(45.0f,(GLfloat)Width/(GLfloat)Height,0.1f,500.0f);

  glMatrixMode(GL_MODELVIEW);

  window_width = Width;
  window_height = Height;
}


/* The main drawing function. */
void DrawGLScene()
{

  static double total = 0.0;

  timer.update();
  float elapsed = timer.get_dif_f();

	//	if (elapsed < 1.0f/240.0f)
	//		usleep((useconds_t)((1.0f/240.0f - elapsed)*1000000));

  timer.reset();

  glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

  total += elapsed;
  if (total > 2.0) {

		experiment->step(1.0f * elapsed);

		//		for (int i=0; i<60; i++)
		//			experiment->step(1.0f/60.0f);

  }

	sceneVis->render();

  fps.showFrameRate();

  glutSwapBuffers();

}

/* The function called whenever a key is pressed. */
void keyPressed(unsigned char key, int x, int y) 
{
	/* avoid thrashing this call */
	usleep(100);

	switch(key) {

	case ESCAPE:
	case 'q':
		glutDestroyWindow(window); 
		//		_exit(0);
		break;
	}

}

void specialKeyPressed(int key, int x, int y) {

  switch(key) {

  case GLUT_KEY_LEFT:
    break;

  case GLUT_KEY_RIGHT:
    break;

  case GLUT_KEY_UP:
    break;

  case GLUT_KEY_DOWN:
    break;

  }

}


void mouseFunc(int button, int state, int x, int y) {

	MouseButton mouseButton = UserInputManager::mouseButtonFromGLUTButton(button);
	bool shiftPressed = glutGetModifiers() == GLUT_ACTIVE_SHIFT;
	bool isDown = state == GLUT_DOWN;

	if (isDown)
		userInputManager->mouseDown(mouseButton, shiftPressed, x, y);
	else
		userInputManager->mouseUp(x, y);

}

void mouseMotionFunc(int x, int y) {

	userInputManager->mouseMotion(x, y);

}




int main(int argc, char **argv) {  

	if (argc != 2) {
		cout << "USAGE :" << endl << " ./gui res/test.yaml" << endl;
		exit(1);
	}

  // default random seeds
  srand48(0);
  srand(0);

  glutInit(&argc, argv);  
  glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA | GLUT_STENCIL | GLUT_DEPTH | GLUT_MULTISAMPLE);  
  glutInitWindowSize(window_width, window_height);  
  glutInitWindowPosition(0, 0);

  window = glutCreateWindow("Life Sim");  

  glutDisplayFunc(&DrawGLScene);  
  glutIdleFunc(&DrawGLScene);
  glutReshapeFunc(&ReSizeGLScene);
  glutKeyboardFunc(&keyPressed);
  glutSpecialFunc(&specialKeyPressed);
  glutMouseFunc(&mouseFunc);
  glutMotionFunc(&mouseMotionFunc);


  InitGL(window_width, window_height);


	// Create the experiment
	experiment = new RLExperiment();

	// Create the scene visualizer, and connect to the scene
	sceneVis = new SceneVis(*(experiment->getScene()));

	experiment->getScene()->setObserver(sceneVis);

	// Load the experiment
	experiment->load(argv[1]);

	userInputManager = new UserInputManager(*sceneVis);

  timer.reset();

  glutMainLoop();  

  return 0;

}





