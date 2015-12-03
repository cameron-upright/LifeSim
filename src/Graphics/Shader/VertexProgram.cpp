#include <iostream>
#include <fstream>
#include <istream>
#include <sstream>

#include "VertexProgram.h"

using namespace std;

VertexProgram::VertexProgram(const string &filename) : Resource(filename, 0) {

  // Open the file
  ifstream f(filename.c_str());
  if (!f.is_open()) {
    cout << "Error loading vertex program : " << filename << endl;
    return;
  }

  // Get the file in a string
  stringstream buffer;
  buffer << f.rdbuf();
  string fileString = buffer.str();

  //  cout << fileString << endl;

  int len = fileString.length();

  //  printf("%s \n (len=%d)",fileString.c_str(), len);

  glGenProgramsARB(1, &vertexProgram); 
  glBindProgramARB(GL_VERTEX_PROGRAM_ARB, vertexProgram);
  glProgramStringARB(GL_VERTEX_PROGRAM_ARB, GL_PROGRAM_FORMAT_ASCII_ARB, len, fileString.c_str());


}


VertexProgram::~VertexProgram() {
  glDeleteProgramsARB(1, &vertexProgram);
}
