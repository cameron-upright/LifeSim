#ifndef RL_GLUE_CXX_H
#define RL_GLUE_CXX_H

#include <string>
#include <vector>

#include <rlglue/Environment_common.h>

namespace RLGlueCxx {



	class AbstractType { 

		std::vector<int>    intData_;
		std::vector<double> doubleData_;
		std::string         charData_;

	public:

		/**
		 * Constructor for an rl_abstract_type_t
		 */
		AbstractType(const rl_abstract_type_t &abstractType) :
      intData_(abstractType.intArray, abstractType.intArray + abstractType.numInts),
      doubleData_(abstractType.doubleArray, abstractType.doubleArray + abstractType.numDoubles),
			charData_(abstractType.charArray, abstractType.numChars) {}

		/**
		 * Converts to an rl_abstract_type_t, the user is responsible for cleanup
		 */
		operator rl_abstract_type_t() {

			rl_abstract_type_t abstract;
			
			// Allocate the abstract variable 
			allocateRLStruct(&abstract,
											 intData_.size(),
											 doubleData_.size(),
											 charData_.size());

			// Copy the data
			std::copy(&intData_[0],    abstract.intArray,    abstract.intArray    + intData_.size());
			std::copy(&doubleData_[0], abstract.doubleArray, abstract.doubleArray + doubleData_.size());
			std::copy(&charData_[0],   abstract.charArray,   abstract.charArray   + charData_.size());

			return abstract;

		}



	};



	class Observation : public AbstractType {

	public:

		Observation(const observation_t &observation) : AbstractType(observation) {}

	};


};


#endif // RL_GLUE_CXX_H

