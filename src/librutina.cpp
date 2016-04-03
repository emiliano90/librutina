#include "../include/librutina.hpp"

using namespace robot;

SafeSpot getNextDestination( std::vector<SafeSpot> destinations, SafeSpot actualDestination );

extern "C" void run( tesis::MessageServer* msgServer )
{
    bool quit = false;
    struct timeval start, end;
    start.tv_sec = 0;
    long seconds, useconds, mtime = 0;

    bool start_count = true;
    auto start_time = std::chrono::high_resolution_clock::now();
    bool autocontrol = true;
    bool autocontrol_go_next_destination = false;

    long elapsed_time = 0;

    SafeSpot lastDestination;
    SafeSpot nextDestination;
    nextDestination.id = -1;
    std::vector<SafeSpot> destinations;

    msgServer->announce("routine/destination/first");
    msgServer->announce("routine/destination/last");
    
    while( !quit )
    {

        gettimeofday( &end, NULL );

        seconds = end.tv_sec - start.tv_sec;
        useconds = end.tv_usec - start.tv_usec;
        mtime = ( ( seconds ) * 1000 + useconds / 1000.0 ) + 0.5;

        if( mtime > 35 ) //0.035 segundos
        {
            gettimeofday( &start, NULL );

            bool gui_go_next_destination = msgServer->getBool( "gui/go_next_destination", "true" );

            Point robot_position;
            robot_position.x = msgServer->getFloat( "camera/robot_position/x", 0 );
            robot_position.y = msgServer->getFloat( "camera/robot_position/y", 0 );
            robot_position.z = msgServer->getFloat( "camera/robot_position/z", 0 );

            destinations.clear();

            for( int i = 0; i <  msgServer->getInt( "camera/destination/count", 0 ); i++ )
            {
                SafeSpot sp;
                sp.pos.x = msgServer->getInt( "camera/destination/" + std::to_string( i ) + "/x", 0 );
                sp.pos.y = msgServer->getInt( "camera/destination/" + std::to_string( i ) + "/y", 0 );
                sp.pos.z = msgServer->getInt( "camera/destination/" + std::to_string( i ) + "/z", 0 );
                sp.id = msgServer->getInt( "camera/destination/" + std::to_string( i ) + "/id", 0 );
                sp.comment = msgServer->getInt( "camera/destination/" + std::to_string( i ) + "/comment", 0 );
                sp.time = msgServer->getInt( "camera/destination/" + std::to_string( i ) + "/time", 0 );

                destinations.push_back( sp );
            }

            if( autocontrol )
            {
                long elapsed_time = msgServer->getLong( "camera/elapsed_time", 0 );
                bool is_visible = msgServer->getBool( "camera/robot_found", false );

                //si no hay un destino asignado
                if( nextDestination.id == -1 )
                {
                    nextDestination = getNextDestination( destinations, nextDestination );
                }
                //si aparecio y estoy Yendo al centro
                else if( nextDestination.id == -2 && is_visible )
                    nextDestination = lastDestination;

                float distance = Util::distance( robot_position, nextDestination.pos );

                // if distance between robot and destination is <= 50
                if( is_visible && distance <=  30 )//30cm de distancia
                {
                    if( start_count )
                    {
                        start_time = std::chrono::high_resolution_clock::now();
                        start_count = false;
                    }

                    auto end_time = std::chrono::high_resolution_clock::now();
                    auto elapsed_time_over_spot = std::chrono::duration_cast<std::chrono::seconds>( end_time - start_time );

                    if( elapsed_time_over_spot.count() >=  nextDestination.time )
                    {
                        autocontrol_go_next_destination = true;
                    }
                }
                else
                {
                    start_count = true;
                    autocontrol_go_next_destination = false;

                    //si estoy desaparecido voy al centro
                    if( !is_visible && nextDestination.id != -2 )
                    {
                        lastDestination = nextDestination;
                        nextDestination.pos.y = msgServer->getLong( "camera/space/height" ) / 2;
                        nextDestination.pos.x = msgServer->getLong( "camera/space/width" ) / 2;
                        nextDestination.pos.z = 500;
                        nextDestination.id = -2;

                    }

                }

                if( elapsed_time > 5000 )
                    msgServer->publish( "gui/action/land", "true" );
            }

            if( gui_go_next_destination || autocontrol_go_next_destination )
            {
                nextDestination = getNextDestination( destinations, nextDestination );

                // TODO esto es un hack. GUI deberia publicar sus propios mensajes.
                msgServer->publish( "gui/go_next_destination", "false" );
            }

            msgServer->publish( "camera/destination/x", std::to_string( nextDestination.pos.x ) );
            msgServer->publish( "camera/destination/y", std::to_string( nextDestination.pos.y ) );
            msgServer->publish( "camera/destination/z", std::to_string( nextDestination.pos.z ) );
            msgServer->publish( "camera/destination/id", std::to_string( nextDestination.id ) );

            // is first destination
            if( destinations.front().id == nextDestination.id )
            {
                msgServer->publish( "routine/destination/first", "true" );
            }
            // is last destination
            else if( destinations.back().id ==  nextDestination.id )
            {
                msgServer->publish( "routine/destination/last", "true" );
            }
            else
            {
                // is neither first nor last destination
                msgServer->publish( "routine/destination/first", "false" );
                msgServer->publish( "routine/destination/last", "false" );
            }


            quit = msgServer->getBool( "gui/finish", false );
        }

        usleep( 500 ); //0,035 segundos//0,010
    }

}

SafeSpot getNextDestination( std::vector<SafeSpot> destinations, SafeSpot actualDestination )
{
    SafeSpot n;
    n.id = -1;

    for( int i = 0; i < destinations.size(); i++ )
        if( destinations.at( i ).id > actualDestination.id && ( destinations.at( i ).id < n.id || n.id == -1 ) )
            n = destinations.at( i );

    return n;

}
