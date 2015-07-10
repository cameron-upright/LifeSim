// fpscounter.h
// Sherif Ghali

#include <sys/time.h>
#include <stdio.h>

#ifndef FPSCOUNTER_H
#define FPSCOUNTER_H

class FPScounter
{
protected:
    struct timeval startTime;
    char str[30];// = {0};

public:
    FPScounter()
    {
	gettimeofday(&startTime, NULL);
	str[0] = 0;
    }

    void
    showFrameRate()
    {
	static struct timeval currentTime;
	static float elapsedTime=0.0; 
	float lambda = 0.05;

	gettimeofday(&currentTime, NULL);

	elapsedTime *= 1-lambda;
	elapsedTime += (currentTime.tv_sec - startTime.tv_sec +
	    ((currentTime.tv_usec - startTime.tv_usec) / 1.0E6))*lambda;
    
	startTime = currentTime;
	sprintf(str, "Frames per second: %.2f", 1.0 / elapsedTime);

	DisplayFrameRate(str, GLUT_BITMAP_HELVETICA_18, 1.0, 1.0, 1.0, 0.07, 0.035);
    }

protected:
    void DisplayFrameRate(char* str,
			  void *font, GLclampf r, GLclampf g, GLclampf b, 
			  GLfloat x, GLfloat y)
    {
	/* font: font to use, e.g., GLUT_BITMAP_HELVETICA_10
	   r, g, b: text colour
	   x, y: text position in window: range [0,0] (bottom left of window)
	   to [1,1] (top right of window). */

	char *ch; 
	GLint matrixMode;
	GLboolean lightingOn;

	lightingOn= glIsEnabled(GL_LIGHTING);        /* lighting on? */
	if (lightingOn) glDisable(GL_LIGHTING);

	glGetIntegerv(GL_MATRIX_MODE, &matrixMode);  /* matrix mode? */

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
        glLoadIdentity();
        gluOrtho2D(0.0, 1.0, 0.0, 1.0);
        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
	glLoadIdentity();
	glPushAttrib(GL_COLOR_BUFFER_BIT);       /* save current colour */
	glColor3f(r, g, b);
	glRasterPos3f(x, y, 0.0);
	for(ch= str; *ch; ch++) {
	    glutBitmapCharacter(font, (int)*ch);
	}
	glPopAttrib();
        glPopMatrix();
        glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(matrixMode);
	if (lightingOn) glEnable(GL_LIGHTING);
    }
};

#endif // FPSCOUNTER_H
