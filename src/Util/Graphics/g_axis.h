#ifndef AXIS_H
#define AXIS_H

#ifdef __APPLE__
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#endif

void draw_arrow_2d(float shaft_width, float shaft_height, float head_width, float head_height);
void draw_arrow_quad(GLUquadric *quad, float shaft_radius, float shaft_length, float head_radius, float head_length);
void draw_arrow(float shaft_radius, float shaft_length, float head_radius, float head_length);
void draw_arrow_dir(float a_point[], float a_dir[], float shaft_radius, float shaft_length, float head_radius, float head_length);
void draw_axis(void);
void drawOrientationAxis(float theta, float phi);

inline void draw_arrow_default(void) { draw_arrow(0.05, 0.8, 0.1, 0.2); }


#endif
