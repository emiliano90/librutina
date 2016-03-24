#include "../include/librutina.hpp"

using namespace robot;

SafeSpot nextDestination(std::vector<SafeSpot> destinations, SafeSpot nextDestination);

extern "C" void run( tesis::MessageServer* msgServer )
{
    RobotConfiguration robotConfig( "./config/libardrone.json" );
    RobotConfig config = robotConfig.get();

    bool quit = false;
    struct timeval start, end;
    start.tv_sec = 0;
    long seconds, useconds, mtime = 0;

    bool start_count = true;
    auto start_time = std::chrono::high_resolution_clock::now();
    bool autocontrol = true;
    bool autocontrol_go_next_destination = false;
    
    long elapsed_time = 0;

    SafeSpot nextDestination;
    nextDestination.id = -1;
    std::vector<SafeSpot> destinations;
    while( !quit )
    {
        
	gettimeofday(&end, NULL);

	seconds = end.tv_sec - start.tv_sec;
	useconds = end.tv_usec - start.tv_usec;
	mtime = ((seconds) * 1000 + useconds / 1000.0) + 0.5;

	if (mtime > 35) //0.035 segundos
	{
	  gettimeofday(&start, NULL);
	 
	  bool gui_go_next_destination = msgServer->getBool( "gui/go_next_destination", "true" );

	  Point robot_position;
	  robot_position.x = msgServer->getFloat ( "camera/robot_position/x", 0 );
	  robot_position.y = msgServer->getFloat( "camera/robot_position/y", 0 );
	  robot_position.z = msgServer->getFloat ( "camera/robot_position/z", 0 );
        
	  destinations.clear();
	  for(int i = 0;i <  msgServer->getInt ( "camera/destination/count", 0);i++)
	  {
	    SafeSpot sp;
	    sp.pos.x = msgServer->getInt ( "camera/destination/" + std::to_string(i) + "/x", 0);
	    sp.pos.y = msgServer->getInt ( "camera/destination/" + std::to_string(i) + "/y", 0);
	    sp.pos.z = msgServer->getInt ( "camera/destination/" + std::to_string(i) + "/z", 0);
	    sp.id = msgServer->getInt ( "camera/destination/" + std::to_string(i) + "/id", 0);
	    sp.comment = msgServer->getInt ( "camera/destination/" + std::to_string(i) + "/comment", 0);
	    sp.time = msgServer->getInt ( "camera/destination/" + std::to_string(i) + "/time", 0);
	    
	    destinations.push_back(sp);
	  }
          if( autocontrol )
	  {
	      if(nextDestination.id == -1)
	      {
		nextDestination = nextDestination(destinations, nextDestination);
	      }
		
	      float distance = Util::distance( robot_position, nextDestination);

	      // if distance between robot and destination is <= 50
	      if( robot_position.x != -1 && distance <=  30 )//30cm de distancia
	      {
		  if( start_count )
		  {
		      start_time = std::chrono::high_resolution_clock::now();
		      start_count = false;
		  }

		  auto end_time = std::chrono::high_resolution_clock::now();
		  auto elapsed_time_over_spot = std::chrono::duration_cast<std::chrono::seconds>( end_time - start_time );

		  if( elapsed_time_over_spot.count() >=  nextDestination.time)
		  {
		      autocontrol_go_next_destination = true;
		  }
	      }
	      else
	      {
		  start_count = true;
		  autocontrol_go_next_destination = false;
	      }
	  }

	  if( gui_go_next_destination || autocontrol_go_next_destination )
	  {
	      nextDestination = nextDestination(destinations, nextDestination);
	      // TODO esto es un hack. GUI deberia publicar sus propios mensajes.
	      msgServer->publish( "gui/go_next_destination", "false" );
	  }
	  msgServer->publish( "camera/destination/x", std::to_string( nextDestination.pos.x ) );
	  msgServer->publish( "camera/destination/y", std::to_string( nextDestination.pos.y ) );
	  msgServer->publish( "camera/destination/z", std::to_string( nextDestination.pos.z ) );
	  msgServer->publish( "camera/destination/id", std::to_string( nextDestination.id ) );

	
	  quit = msgServer->getBool( "gui/finish", false);
	}
	usleep(500); //0,035 segundos//0,010
    }

    robot.close();
}

SafeSpot nextDestination(std::vector<SafeSpot> destinations, SafeSpot nextDestination)
{
  SafeSpot n;
  n.id = -1;
  
  for(int i = 0; i < destinations.size(); i++) 
    if(destinations.at(i).id > nextDestination.id && (destinations.at(i).id < n.id || n.id == -1)) ;
      n = destinations.at(i);
  
  
  return n;
  
}
