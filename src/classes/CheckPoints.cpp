#include "classes/CheckPoints.hpp"
using namespace boost;

CheckPoints::CheckPoints ( std::vector< Checkpoint > checkpoints, std::vector<SafeSpot> spots )
{
	destino = 0;
	this->checkpoints = checkpoints;
	for ( int i = 0; i < checkpoints.size(); i++ ) {
		Destino dest;
		dest.check = checkpoints[i];

		int e = 0;
		while ( e < spots.size() && spots[e].id !=  dest.check.id )
			e++;


		// update safe spot position
		if ( e < spots.size() )
			dest.sp = spots[e];
			
		else {
			SafeSpot sp;
			sp.id = dest.check.id;
			sp.pos.x = -1;
			sp.pos.y = -1;
			dest.sp = sp;
		}
		destinos.push_back ( dest );
	}
}

bool CheckPoints::refreshSpots ( std::vector<SafeSpot> spots )
{
	bool faltan = false;
	for ( int i = 0; i < this->destinos.size(); i++ ) {

		int e = 0;
		while ( e < spots.size() && spots[e].id !=  destinos[i].check.id )
			e++;

		if ( e < spots.size() ) {
			destinos[i].sp = spots[e];
		}
		if(destinos[i].sp.pos.x == -1)
			faltan = true;
	}
	return !faltan;
	

}

std::vector<Destino> CheckPoints::getDestinos()
{
	return destinos;
}

bool CheckPoints::nextDestino()
{
	int siz =destinos.size();
	if ( destino < ( siz - 1 ) ) {
		int d = destino;
		do {
			d++;
		} while ( destinos[d].sp.pos.x == -1 && d < ( destinos.size() - 1 ) );

		if ( destinos[d].sp.pos.x != -1 ) {
			destino = d;
			return true;
		} else
			return false;
	} else
		return false;

}

Destino CheckPoints::getDestino()
{
	return destinos[destino];
}

Destino* CheckPoints::getDestino ( int pos )
{
	if ( pos < destinos.size() )
		return &destinos[pos];
	else
		return NULL;

}

CheckPoints::~CheckPoints()
{

}
