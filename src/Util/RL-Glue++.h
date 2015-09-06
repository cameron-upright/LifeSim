#ifndef RL_GLUE_CXX_H
#define RL_GLUE_CXX_H

#include <string>
#include <vector>

#include <rlglue/Environment_common.h>

namespace RLGlueCxx {


	/*
	class AbstractType { 


	protected:

		rl_abstract_type_t abstractType_;


	public:

		AbstractType() {
			allocateRLStruct(&abstractType_, 0, 0, 0);
		}

		AbstractType(int numInt, int numDouble, int numChar) {
			allocateRLStruct(&abstractType_, numInt, numDouble, numChar);
		}

		~AbstractType() {
			clearRLStruct(&abstractType_);
		}

		AbstractType(const rl_abstract_type_t &abstractType) {
			allocateRLStruct(&abstractType_,
											 abstractType.numInts,
											 abstractType.numDoubles,
											 abstractType.numChars);
		}


		void setIntData(const int index, const int value) {
			abstractType_.intArray[index] = value;
		}

		void setDoubleData(const int index, const double value) {
			abstractType_.doubleArray[index] = value;
		}

		void setCharData(const int index, const char value) {
			abstractType_.charArray[index] = value;
		}



		operator rl_abstract_type_t*() {
			return &abstractType_;
		}

	};



	class Observation : public AbstractType {

	public:

		Observation() {}

		Observation(const int numInts, const int numDoubles, const int numChars) :
			AbstractType(numInts, numDoubles, numChars) {}

		Observation(const observation_t &observation) : AbstractType(observation) {}

		operator observation_t*() {
			return &abstractType_;
		}


	};
	*/


	class Observation {

		observation_t observation_;

	public:

		Observation() {
			observation_.intArray    = NULL;
			observation_.doubleArray = NULL;
			observation_.charArray   = NULL;
		}

		Observation(const int numInts, const int numDoubles, const int numChars) {
			allocateRLStruct(&observation_, numInts, numDoubles, numChars);
		}

		~Observation() {
			clearRLStruct(&observation_);
			observation_.intArray    = NULL;
			observation_.doubleArray = NULL;
			observation_.charArray   = NULL;
		}


		void setIntData(const int index, const int value) {
			observation_.intArray[index] = value;
		}

		void setDoubleData(const int index, const double value) {
			observation_.doubleArray[index] = value;
		}

		void setCharData(const int index, const char value) {
			observation_.charArray[index] = value;
		}

		observation_t* getObservationPtr() {
			return &observation_;
		}

	};


	/*
	class Action : public AbstractType {

	public:

		Action() {}

		Action(const action_t &action) : AbstractType(action) {}

	};
	*/

	/*
	class ObservationAction {

	public:

		const Observation *observation_;
		const Action      *action_;

		ObservationAction() :
			observation_(NULL), action_(NULL) {}

	};
	*/


	class RewardObservationTerminal {

	public:

		reward_observation_terminal_t rewardObservationTerminal_;

		RewardObservationTerminal() {
			rewardObservationTerminal_.reward = 0.0;
			rewardObservationTerminal_.observation = NULL;
			rewardObservationTerminal_.terminal = 0;
		}

		RewardObservationTerminal(const double reward, const observation_t *observation, const int terminal) {
			rewardObservationTerminal_.reward = reward;
			rewardObservationTerminal_.observation = observation;
			rewardObservationTerminal_.terminal = terminal;
		}

		void setReward(double reward) {
			rewardObservationTerminal_.reward = 0.0;
		}			

		void setObservation(const observation_t *observation) {
			rewardObservationTerminal_.observation = observation;
		}			

		void setTerminal(int terminal) {
			rewardObservationTerminal_.terminal = terminal;
		}			


		

		operator reward_observation_terminal_t*() {
			return &rewardObservationTerminal_;
		}


	};


	/*
	class RewardObservationActionTerminal {

	public:

		double      reward;
		Observation observation;
		Action      action;
		int         terminal;
	};
	*/


};


#endif // RL_GLUE_CXX_H

