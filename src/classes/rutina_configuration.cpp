#include "classes/rutina_configuration.hpp"
using namespace boost;

RutinaConfiguration::RutinaConfiguration ( std::string path ) : path ( path )
{
	this->parse();
}

RutinaConfig RutinaConfiguration::get()
{
	return this->configuration;
}

void RutinaConfiguration::parse()
{
	// http://theboostcpplibraries.com/boost.propertytree
	
	RutinaConfig config;

	try {

		property_tree::ptree ptree;
		property_tree::read_json ( this->path, ptree );


		//busco varios colores para el robot.
		// We are not considering unsafe spots yet.
		BOOST_FOREACH ( property_tree::ptree::value_type & val, ptree.get_child ( "checkpoints" ) ) {
			property_tree::ptree child = val.second;

			Checkpoint check;
			check.id = child.get ( "id", 0 );
			check.altura = child.get ( "altura", 0 );

			config.checkpoints.push_back ( check );
		}


	} catch ( std::exception const& e ) {
		std::cerr << e.what() << std::endl;
	}

	this->configuration = config;
}


RutinaConfiguration::~RutinaConfiguration()
{

}

