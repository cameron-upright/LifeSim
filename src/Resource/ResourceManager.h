#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include <map>
#include <string>

#include "VertexProgram.h"
#include "FragmentProgram.h"

using namespace std;

class ResourceManager {

  map<string, Resource*> resourceMap;

public:
  ResourceManager() {}
  ~ResourceManager() {
    cleanup();
  }

  static ResourceManager &instance();

  VertexProgram*   loadVertexProgram(const string &filename);
  FragmentProgram* loadFragmentProgram(const string &filename);

  void cleanup();

};


#endif // RESOURCE_MANAGER_H
