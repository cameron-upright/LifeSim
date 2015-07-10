#ifndef RESOURCE_H
#define RESOURCE_H

#include <string>

using namespace std;

class Resource {

  string name;
  int retainCount;

public:

  Resource(string name, int retainCount) : name(name), retainCount(retainCount) {}
  virtual ~Resource() {}

  bool isUsed() {
    return retainCount > 0;
  }

  const string getName() const {
    return name;
  }

  int getRetainCount() const {
    return retainCount;
  }

  void retain() {
    retainCount++;
  }

  void release() {
    retainCount--;
  }

};


#endif // RESOURCE_H
