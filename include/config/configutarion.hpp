#ifndef CONFIGURATION_HPP
#define CONFIGURATION_HPP

#include <vector>
#include <string>
#include <boost/concept_check.hpp>

struct Checkpoint {
	int id;
	int altura;
	int time;
};
struct RutinaConfig {
	std::vector< Checkpoint > checkpoints;

};

#endif                                                      //  CONFIGURATION_HPP
