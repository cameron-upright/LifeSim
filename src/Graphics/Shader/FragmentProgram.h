#ifndef FRAGMENT_PROGRAM_H
#define FRAGMENT_PROGRAM_H

#define GL_GLEXT_PROTOTYPES
#define GLX_GLXEXT_PROTOTYPES

#ifdef __APPLE__
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#include <GL/gl.h>
#include <GL/glext.h>
#endif


#include "Resource.h"

class FragmentProgram : public Resource{

public:

  GLuint fragmentProgram;

  FragmentProgram(const string &filename);
  ~FragmentProgram();

};



#endif // FRAGMENT_PROGRAM_H
