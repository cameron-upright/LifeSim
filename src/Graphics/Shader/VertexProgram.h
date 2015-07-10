#ifndef VERTEX_PROGRAM_H
#define VERTEX_PROGRAM_H

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

class VertexProgram : public Resource {

public:

  GLuint vertexProgram;

  VertexProgram(const string &filename);
  ~VertexProgram();

};



#endif // VERTEX_PROGRAM_H
