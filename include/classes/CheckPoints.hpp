#ifndef CHECKPOINTS_CONFIGURATION_H
#define CHECKPOINTS_CONFIGURATION_H

#include "config/configutarion.hpp"

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/foreach.hpp>

#include "structures.hpp"
#include <exception>
#include <iostream>
#include <string>

struct Destino {
	SafeSpot sp;
	Checkpoint check;

};
class CheckPoints
{
public:
	CheckPoints ( std::vector<Checkpoint> checkpoints, std::vector<SafeSpot> spots );
	bool refreshSpots ( std::vector<SafeSpot> spots );
	std::vector<Destino> getDestinos();
	Destino* getDestino ( int pos );
	Destino getDestino();
	bool nextDestino();

	~CheckPoints();
private:
	std::vector<Checkpoint> checkpoints;
	std::vector<Destino> destinos;
	int destino;

};

#endif                                                      // CHECKPOINTS_CONFIGURATION_H


