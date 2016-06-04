#include "util/util.hpp"
#include <boost/concept_check.hpp>

using namespace robot;
Util::Util()
{

}

Util::~Util()
{

}

float Util::rad_to_deg ( float rad )
{
	return rad * 180.0 / M_PI;
}

float Util::deg_to_rad ( float deg )
{
	return deg * M_PI / 180.0;
}

float Util::distance ( Point opoint, Point dpoint )
{
	Point point;
	point.x = opoint.x - dpoint.x;
	point.y = opoint.y - dpoint.y;
	return std::sqrt ( std::abs ( point.x * point.x + point.y * point.y ) );
}
float Util::distance ( Point3D opoint, Point3D dpoint )
{
	Point3D point;
	point.x = opoint.x - dpoint.x;
	point.y = opoint.y - dpoint.y;
	point.z = opoint.z - dpoint.z;
	float d1 = std::sqrt ( std::abs ( point.x * point.x + point.y * point.y ) );
	return std::sqrt ( std::abs ( d1 * d1 + point.z * point.z ) );
}
float Util::get_angle_as_deg ( Point opoint, Point dpoint )
{
	Point point;
	float angle = 0;

	point.x = opoint.x - dpoint.x;
	point.y = opoint.y - dpoint.y;

	if ( point.x != 0 ) {
		angle = std::atan ( std::abs ( ( float ) point.y / ( float ) point.x ) );
		angle = Util::rad_to_deg ( angle );

		if ( point.x > 0 ) {
			if ( point.y < 0 ) {
				angle = ( angle + 90 ) * -1;
			} else {
				angle = ( 90 - angle ) * -1;
			}
		} else {
			if ( point.y < 0 ) {
				angle += 90;
			} else {
				angle = 90 - angle;
			}
		}
	}

	return angle;
}

Point Util::get_point ( int dist, float deg )
{
	Point point;
	float rad = Util::deg_to_rad ( deg );

	point.x = dist * std::sin ( rad );
	point.y = dist * std::cos ( rad );

	return point;
}

Point Util::toPoint2D( Point3D p2)
{
	Point p;
	p.x = p2.x;
	p.y = p2.y;
	return p;
	
}
float Util::normalize_angle ( float angle )
{
	if ( angle > 180 ) {
		angle -= 360;
	} else if ( angle < -180 ) {
		angle += 360;
	}

	return angle;
}

