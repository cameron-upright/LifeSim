#include <fstream>
#include <istream>
#include <sstream>

#include "RLAgent.h"



void RLAgent::load(Scene *scene, const string &filename) {

	// TODO : hack
	creature = scene->getCreature("creature");

}
