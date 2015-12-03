#include "ResourceManager.h"

#include <vector>
#include <iostream>

using namespace std;

ResourceManager &ResourceManager::instance() {
  static ResourceManager r;
  return r;
}



void ResourceManager::cleanup(void) {
  // TODO : Handle dependency between resources

  vector<string> unusedResources;
  map<string, Resource*>::iterator resourceIterator;

  // find all unused resources, and mark them for deletion
  for (resourceIterator = resourceMap.begin(); resourceIterator != resourceMap.end(); resourceIterator++)
    if ((*resourceIterator).second->isUsed())
      unusedResources.push_back((*resourceIterator).first);

  // delete and remove the unused resources
  for (vector<string>::iterator it = unusedResources.begin(); it != unusedResources.end(); it++) {
    resourceIterator = resourceMap.find(*it);
    delete resourceIterator->second;
    resourceMap.erase(resourceIterator);
  }

}


VertexProgram* ResourceManager::loadVertexProgram(const string &filename) {

  // Check if we already have it
  if (resourceMap.count(filename) > 0)
    return (VertexProgram*)resourceMap[filename];

  // Create the program and track it
  VertexProgram *vp = new VertexProgram(filename);
  resourceMap[filename] = (Resource*)vp;

  return vp;

}


FragmentProgram* ResourceManager::loadFragmentProgram(const string &filename) {

  // Check if we already have it
  if (resourceMap.count(filename) > 0)
    return (FragmentProgram*)resourceMap[filename];

  // Create the program and track it
  FragmentProgram *fp = new FragmentProgram(filename);
  resourceMap[filename] = fp;

  return fp;

}

