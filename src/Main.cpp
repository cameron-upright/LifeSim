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

#include <thread>

#include "g_axis.h"
#include "fpscounter.h"
#include "timer.h"

#include "UserInputManager.h"

#include "Scene.h"
#include "SceneVis.h"

#include <rlglue/Environment_common.h>
#include <rlglue/utils/C/RLStruct_util.h>

#include "RLEnvironment.h"


#include "RL-Glue++.h"


using namespace std;
using namespace RLGlueCxx;


#define ESCAPE 27


Timer timer;

int window; 

bool b_left_button, b_right_button, b_middle_button, b_shift;
int mouse_prev_x, mouse_prev_y;
int window_width=800, window_height=600;

SceneVis *sceneVis;

UserInputManager *userInputManager;

FPScounter fps;

RLEnvironment env;

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
}

// The function called when our window is resized (which shouldn't happen, because we're fullscreen) 
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


// The main drawing function. 
void DrawGLScene() {

	//	cerr << "ENV DrawGLScene start" << endl;

  static double total = 0.0;

  timer.update();
  float elapsed = timer.get_dif_f();

	//	if (elapsed < 1.0f/240.0f)
	//		usleep((useconds_t)((1.0f/240.0f - elapsed)*1000000));

  timer.reset();

  glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

  total += elapsed;
  if (total > 2.0) {
		//		cerr << "ENV DrawGLScene step start" << endl;
		env.stepSim(elapsed * 1.0f);
		//		cerr << "ENV DrawGLScene step done" << endl;

  }

	sceneVis->render();

  fps.showFrameRate();

  glutSwapBuffers();

	//	cerr << "ENV DrawGLScene end" << endl;

}

// The function called whenever a key is pressed. 
void keyPressed(unsigned char key, int x, int y) 
{
	// avoid thrashing this call 
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


void init() {

  // default random seeds
  srand48(0);
  srand(0);

	int argc = 1;

  glutInit(&argc, NULL);
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

	// Create the scene visualizer, and connect to the scene
	sceneVis = new SceneVis(*(env.getScene()));

	env.getScene()->setObserver(sceneVis);

	// Load the environment
	env.load("res/test_env.prototxt");

	userInputManager = new UserInputManager(*sceneVis);

  timer.reset();

}



std::thread mainThread;


//Observation observation;
RewardObservationTerminal rewardObservationTerminal;


observation_t this_observation;

bool firstInit = true;

int step = 0;


const char* env_init() {

	cerr << "ENV env_init start" << endl;

	const char* task_spec="VERSION RL-Glue-3.0 PROBLEMTYPE episodic DISCOUNTFACTOR 1.0 OBSERVATIONS INTS (0 20) ACTIONS INTS (0 1)  REWARDS (-1.0 1.0)  EXTRA skeleton_environment(C/C++) by Brian Tanner.";

	if (firstInit) {
		// Magic
		std::thread(init).join();

		// Start up the main loop
		mainThread = std::thread(glutMainLoop);

		firstInit = false;
	}

	// Allocate the observation variable 
	allocateRLStruct(&this_observation,
									 0,
									 env.getCreature()->hingeConstraints.size() + env.getCreature()->universalConstraints.size(),
									 0);


	// Setup the reward_observation variable 
 	rewardObservationTerminal = RewardObservationTerminal(0.0, &this_observation, 0);

	/*
	observation = Observation(1,0,0);
	rewardObservationTerminal = RewardObservationTerminal(0.0, observation.getObservationPtr(), 0);
	*/



	cerr << "ENV env_init end" << endl;

	return task_spec;

}


const observation_t *env_start() { 

	cerr << "ENV env_start start" << endl;

	for (int i=0; i<this_observation.numDoubles; i++)
		this_observation.doubleArray[0] = 0.0;

	cerr << "ENV env_start end" << endl;

	step = 0;

	//	return observation.getObservationPtr();
	return &this_observation;

}


const reward_observation_terminal_t *env_step(const action_t *this_action) {

	//	cerr << "ENV env_step start" << endl;

	float the_reward=0;
	int episode_over=0;


	// Prepare the step, creating an action to resist movement
	LifeSim::RLStateDesc state;
	LifeSim::RLActionDesc action;

	const float constraintMultiplier = 0.0f;

	for (unsigned i=0; i<env.getCreature()->hingeConstraints.size(); i++)
		action.add_action(-constraintMultiplier * env.getCreature()->hingeConstraints[i]->getAngle());

	for (unsigned i=0; i<env.getCreature()->universalConstraints.size(); i++) {
		action.add_action(-constraintMultiplier * env.getCreature()->universalConstraints[i]->getAngle(0));
		action.add_action(-constraintMultiplier * env.getCreature()->universalConstraints[i]->getAngle(1));
	}

	// Step the environment
	env.stepRL(state, action, the_reward);



	// Set the observation
	for (int i=0; i<this_observation.numDoubles; i++)
		this_observation.doubleArray[0] = 0.0;

	// Set episode_over
	step++;
	episode_over = step == 50;
	if (episode_over)
		step = 0;

	rewardObservationTerminal.setReward(the_reward);
	rewardObservationTerminal.setTerminal(episode_over);


	//	cerr << "ENV env_step end" << endl;
	return rewardObservationTerminal;

}


void env_cleanup() {

	cerr << "ENV env_cleanup start" << endl;

	clearRLStruct(&this_observation);

	cerr << "ENV env_cleanup end" << endl;
}


const char* env_message(const char* inMessage) {

	if(strcmp(inMessage,"what is your name?")==0)
		return "my name is skeleton_environment!";

	return "I don't know how to respond to your message";

}


