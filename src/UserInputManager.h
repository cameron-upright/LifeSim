#ifndef USER_INPUT_H
#define USER_INPUT_H

#include <algorithm>
#include "SceneVis.h"

using namespace std;

enum MouseButton {
	NoButton,
	LeftButton,
	MiddleButton,
	RightButton,
	ScrollUpButton,
	ScrollDownButton
};


MouseButton mouseButtonFromGlutButton(int button);

class UserInputManager {

 private:

	MouseButton currentMouseButtonPressed;
	bool shiftPressed;

	int prevMouseX, prevMouseY;

	SceneVis &sceneVis;

 public:

	UserInputManager(SceneVis &sceneVis_) : sceneVis(sceneVis_) {
		currentMouseButtonPressed = NoButton;
		prevMouseX = prevMouseY = 0;
	}

	~UserInputManager() {}


	void mouseDown(MouseButton button, bool shift, int x, int y) {

		currentMouseButtonPressed = button;
		shiftPressed = shift;

		prevMouseX = x;
		prevMouseY = y;

		// If it's a scroll button, scroll immediately
		switch(currentMouseButtonPressed) {

		case ScrollUpButton:
			zoomBasedOnDelta(-10.0f);
			break;

		case ScrollDownButton:
			zoomBasedOnDelta(10.0f);
			break;

		default:
			break;
		}

		//		printf("%d, %d, %d, %d\n", button, shift, x, y);

	}

	void mouseUp(int x, int y) {

		currentMouseButtonPressed = NoButton;

	}

	void mouseMotion(int x, int y) {

		int dx = x - prevMouseX;
		int dy = y - prevMouseY;

		if (currentMouseButtonPressed == LeftButton) {


			if (shiftPressed) {

				pan(dx, dy);

			} else {

				sceneVis.theta -= dx / 100.0;
				sceneVis.phi = max(-M_PI/2.0, min(M_PI/2.0, sceneVis.phi - dy / 100.0));

			}


		} else if (currentMouseButtonPressed == RightButton) {

			zoomBasedOnDelta((dx - dy) * 0.5);

		} else if (currentMouseButtonPressed == MiddleButton) {

			pan(dx, dy);

		}

		prevMouseX = x;
		prevMouseY = y;

	}


	void scrollWheel(float delta) {

		zoomBasedOnDelta(delta);

	}

	void pan(int dx, int dy) {

		float m[16];
		glGetFloatv(GL_MODELVIEW_MATRIX, m);

		sceneVis.lookat[0] -= (m[0]*dx - m[1]*dy)*0.01;
		sceneVis.lookat[1] -= (m[4]*dx - m[5]*dy)*0.01;
		sceneVis.lookat[2] -= (m[8]*dx - m[9]*dy)*0.01;

	}


	void zoomBasedOnDelta(float delta) {

		delta *= 0.05;
		float scale = 1.0f / (1.0f + exp(delta)) + 0.5;
		sceneVis.dist = max(1.0f, min(150.0f, sceneVis.dist * scale));

	}



	static MouseButton mouseButtonFromGLUTButton(int button) {

		switch(button) {

		case GLUT_LEFT_BUTTON:
			return LeftButton;

		case GLUT_MIDDLE_BUTTON:
			return MiddleButton;

		case GLUT_RIGHT_BUTTON:
			return RightButton;

		case 3:
			return ScrollDownButton;

		case 4:
			return ScrollUpButton;

		default:
			return NoButton;

		}

	}



};


#endif
