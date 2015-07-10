#ifdef __APPLE__
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#endif

#include <stdio.h>
#include <math.h>

#include "g_axis.h"

#define RADTODEG (180.0/M_PI)


void draw_arrow_2d(float shaft_width, float shaft_height, float head_width, float head_height) {

  float          a_tmp[2];

  a_tmp[0] = shaft_width/2;
  a_tmp[1] = head_width/2;


  glBegin(GL_LINE_LOOP);
  glVertex2f(a_tmp[0], 0);
  glVertex2f(a_tmp[0], shaft_height);
  glVertex2f(a_tmp[1], shaft_height);
  glVertex2f(0, shaft_height + head_height);
  glVertex2f(-a_tmp[1], shaft_height);
  glVertex2f(-a_tmp[0], shaft_height);
  glVertex2f(-a_tmp[0], 0);
  glEnd();


}

void draw_arrow(float shaft_radius, float shaft_length, float head_radius, float head_length) {
  GLUquadric *quad;

  quad = gluNewQuadric();
  draw_arrow_quad(quad, shaft_radius, shaft_length, head_radius, head_length);
  gluDeleteQuadric(quad);
}

void draw_arrow_dir(float a_point[], float a_dir[], float shaft_radius, float shaft_length, float head_radius, float head_length) {
  GLboolean       lights;
  double          d_theta, d_phi, d_radius;

  d_radius = sqrt(a_dir[0]*a_dir[0] + a_dir[2]*a_dir[2]);
  d_phi = atan(a_dir[1] / d_radius);
  d_theta = atan2(a_dir[0], a_dir[2]);

  glGetBooleanv(GL_LIGHTING, &lights);
  glDisable(GL_LIGHTING);

  glPushMatrix();
  glTranslatef(a_point[0], a_point[1], a_point[2]);
  glRotated(d_theta * RADTODEG, 0.0, 1.0, 0.0);
  glRotated(-d_phi * RADTODEG, 1.0, 0.0, 0.0);

  draw_arrow(shaft_radius, shaft_length, head_radius, head_length);

  glPopMatrix();


  if (lights)
    glEnable(GL_LIGHTING);

}


void draw_arrow_quad(GLUquadric *quad, float shaft_radius, float shaft_length, float head_radius, float head_length) {

  glPushMatrix();

  gluCylinder(quad, shaft_radius, shaft_radius, shaft_length, 8, 1);
  gluDisk(quad, 0, shaft_radius, 8, 1);

  glTranslated(0,0,shaft_length);
  gluCylinder(quad, head_radius, 0, head_length, 8, 1);
  gluDisk(quad, shaft_radius, head_radius, 8, 1);

  glPopMatrix();

}

void draw_axis(void) {
  GLUquadric *quad;
  GLboolean lights;

  glPushMatrix();


  glGetBooleanv(GL_LIGHTING, &lights);

  quad = gluNewQuadric();

  glDisable(GL_LIGHTING);

  glColor3f(0,0,1);
  draw_arrow_quad(quad, 0.05, 0.8, 0.1, 0.2);

  glColor3f(1,0,0);
  glRotated(90,0,1,0);
  draw_arrow_quad(quad, 0.05, 0.8, 0.1, 0.2);

  glColor3f(0,1,0);
  glRotated(-90,1,0,0);
  draw_arrow_quad(quad, 0.05, 0.8, 0.1, 0.2);

  gluDeleteQuadric(quad);

  if (lights)
    glEnable(GL_LIGHTING);

  glPopMatrix();


}




void drawOrientationAxis(float theta, float phi) {

  int old_viewport[4];

  // save old viewport
  glGetIntegerv(GL_VIEWPORT, old_viewport);

  // modify viewport
  glViewport(0, 0, 48, 48);

  glClear(GL_DEPTH_BUFFER_BIT);

  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
  glLoadIdentity();
  gluPerspective(45.0f,1.0,0.1f,100.0f);
  glMatrixMode(GL_MODELVIEW);


  glPushMatrix();

  glLoadIdentity();
  glTranslated(0,-0.2,-3);
  glRotated(-phi*180.0/M_PI, 1.0, 0.0, 0.0);
  glRotated(-theta*180.0/M_PI, 0.0, 1.0, 0.0);

  draw_axis();

  glPopMatrix();


  glMatrixMode(GL_PROJECTION);
  glPopMatrix();
  glMatrixMode(GL_MODELVIEW);

  glViewport(old_viewport[0], old_viewport[1], old_viewport[2], old_viewport[3]);

}
