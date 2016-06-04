#ifndef RUTINA_CONFIGURATION_H
#define RUTINA_CONFIGURATION_H

#include "config/configutarion.hpp"

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/foreach.hpp>

#include <exception>
#include <iostream>
#include <string>


class RutinaConfiguration
{
public:
	RutinaConfiguration ( std::string path );
	RutinaConfig get();

	~RutinaConfiguration();
private:
	RutinaConfig configuration;
	std::string path;

	void parse();
};

#endif                                                      // RUTINA_CONFIGURATION_H


