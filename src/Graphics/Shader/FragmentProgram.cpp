#include <iostream>
#include <fstream>
#include <istream>
#include <sstream>

#include "FragmentProgram.h"

using namespace std;

FragmentProgram::FragmentProgram(const string &filename) : Resource(filename, 0) {

  // Open the file
  ifstream f(filename.c_str());
  if (!f.is_open()) {
    cout << "Error loading fragment program : " << filename << endl;
    return;
  }

  // Get the file in a string
  stringstream buffer;
  buffer << f.rdbuf();
  string fileString = buffer.str();

  //  cout << fileString << endl;

  int len = fileString.length();

  glGenProgramsARB(1, &fragmentProgram); 
  glBindProgramARB(GL_FRAGMENT_PROGRAM_ARB, fragmentProgram);
  glProgramStringARB(GL_FRAGMENT_PROGRAM_ARB, GL_PROGRAM_FORMAT_ASCII_ARB, len, fileString.c_str());

}


FragmentProgram::~FragmentProgram() {

  glDeleteProgramsARB(1, &fragmentProgram);

}
