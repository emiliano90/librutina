#include "../include/librutina.hpp"

using namespace robot;


//SafeSpot getNextDestination ( std::vector<SafeSpot> destinations, SafeSpot actualDestination );

extern "C" void run ( tesis::MessageServer* msgServer)
{

	RutinaConfiguration rut_configuration ( "./config/rutina.json" );
	RutinaConfig conf = rut_configuration.get();

	msgServer->announce ( "rutina/destination/x" );
	msgServer->announce ( "rutina/destination/y" );
	msgServer->announce ( "rutina/destination/z" );
	msgServer->announce ( "rutina/destination/id" );
	msgServer->announce ( "rutina/destination/time" );
	
	bool quit = false;
	struct timeval start, end;
	start.tv_sec = 0;
	long seconds, useconds, mtime = 0;

	bool start_count = true;
	auto start_time = std::chrono::high_resolution_clock::now();
	bool autocontrol = true;
	bool autocontrol_go_next_destination = false;

	long elapsed_time = 0;

	Destino nextDestination;
	nextDestination.sp.pos.x = -1;
	nextDestination.sp.pos.y = -1;
	nextDestination.check.altura = -1;
	bool alCentro = false;
	std::vector<SafeSpot> spots;
	CheckPoints ch ( conf.checkpoints, spots );
	bool buscandoSp = true;
	bool init = true;
	while ( !quit ) {

		gettimeofday ( &end, NULL );

		seconds = end.tv_sec - start.tv_sec;
		useconds = end.tv_usec - start.tv_usec;
		mtime = ( ( seconds ) * 1000 + useconds / 1000.0 ) + 0.5;

		if ( mtime > 35 ) { //0.035 segundos
			gettimeofday ( &start, NULL );

			bool gui_go_next_destination = msgServer->getBool ( "gui/go_next_destination", "false" );

			Point3D robot_position;
			robot_position.x = msgServer->getFloat ( "camera/robot_position/x", 0 );
			robot_position.y = msgServer->getFloat ( "camera/robot_position/y", 0 );
			robot_position.z = msgServer->getFloat ( "camera/robot_position/z", 0 );

			spots.clear();
			for ( int i = 0; i <  msgServer->getInt ( "camera/spots/count", 0 ); i++ ) {
				SafeSpot sp;
				sp.pos.x = msgServer->getInt ( "camera/spots/" + std::to_string ( i ) + "/x", 0 );
				sp.pos.y = msgServer->getInt ( "camera/spots/" + std::to_string ( i ) + "/y", 0 );
				sp.id = msgServer->getInt ( "camera/spots/" + std::to_string ( i ) + "/id", 0 );
				sp.comment = msgServer->getInt ( "camera/spots/" + std::to_string ( i ) + "/comment", 0 );
				//sp.time = msgServer->getInt ( "camera/spots/" + std::to_string ( i ) + "/time", 0 );

				spots.push_back ( sp );
			}
			buscandoSp = !ch.refreshSpots ( spots );

			if ( autocontrol && !buscandoSp) {
				if(init)
				{
					init = false;
					nextDestination = ch.getDestino();
				}
				long elapsed_time = msgServer->getLong ( "camera/elapsed_time", 0 );
				bool is_visible = msgServer->getBool ( "camera/robot_found", false );
				//si no hay un destino asignado
				/*if ( nextDestination.id == -1 )
					nextDestination = ch.getDestino();
				*/
				//si aparecio y estoy llendo al centro
				if ( alCentro && is_visible )
					nextDestination = ch.getDestino();

				//float distance = Util::distance ( Util::toPoint2D(robot_position), nextDestination.sp.pos );
				Point3D pos;
				pos.x = nextDestination.sp.pos.x;
				pos.y = nextDestination.sp.pos.y;
				pos.z = nextDestination.check.altura / 10;
				Point3D pos2 = robot_position;
				pos2.z *= 100;
				
				float distance = Util::distance ( pos2, pos );

				// if distance between robot and destination is <= 50
				if ( is_visible && distance <=  30 ) { //30cm de distancia
					if ( start_count ) {
						start_time = std::chrono::high_resolution_clock::now();
						start_count = false;
					}

					auto end_time = std::chrono::high_resolution_clock::now();
					auto elapsed_time_over_spot = std::chrono::duration_cast<std::chrono::seconds> ( end_time - start_time );
					
					std::cout<<"time: "<< elapsed_time_over_spot.count()<<std::endl;
					
					if ( elapsed_time_over_spot.count() >=  nextDestination.check.time) {
						autocontrol_go_next_destination = true;
					}
				} else {
					start_count = true;
					autocontrol_go_next_destination = false;
					//si estoy desaparecido voy al centro
					if ( !is_visible && !alCentro ) {
						nextDestination.sp.pos.y = msgServer->getLong ( "camera/space/height" ) / 2;
						nextDestination.sp.pos.x = msgServer->getLong ( "camera/space/width" ) / 2;
						nextDestination.check.altura = 500;
						alCentro = true;

					}

				}
				if ( elapsed_time > 5000 )
					msgServer->publish ( "gui/action/land", "true" );


			}

			if ( gui_go_next_destination || autocontrol_go_next_destination ) {
				if (!buscandoSp && !ch.nextDestino() ) 
					msgServer->publish ( "gui/finish", "true" );
				
				else
					nextDestination = ch.getDestino();
				// TODO esto es un hack. GUI deberia publicar sus propios mensajes.
				msgServer->publish ( "gui/go_next_destination", "false" );
			}
			msgServer->publish ( "rutina/destination/x", std::to_string ( nextDestination.sp.pos.x ) );
			msgServer->publish ( "rutina/destination/y", std::to_string ( nextDestination.sp.pos.y ) );
			msgServer->publish ( "rutina/destination/z", std::to_string ( nextDestination.check.altura ) );
			msgServer->publish ( "rutina/destination/id", std::to_string ( nextDestination.sp.id ) );
			msgServer->publish ( "rutina/destination/time", std::to_string ( nextDestination.check.time ) );
			
			quit = msgServer->getBool ( "gui/finish", false );
		}
		usleep ( 500 ); //0,035 segundos//0,010
	}

}
/*
SafeSpot getNextDestination ( std::vector<SafeSpot> destinations, SafeSpot actualDestination )
{
	SafeSpot n;
	n.id = -1;

	for ( int i = 0; i < destinations.size(); i++ )
		if ( destinations.at ( i ).id > actualDestination.id && ( destinations.at ( i ).id < n.id || n.id == -1 ) )
			n = destinations.at ( i );

	return n;

}*/
