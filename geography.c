// packages
#include <math.h>

// heder files
#include "geography.h"


// constants
#define EARTH_RADIUS_MILES 3959
#define LATITUDE_NORTH_POLE_DEGREES 90.0

// distance
double distance(double lat1, double lon1, double lat2, double lon2) {
    // we have the angles, so we want the differences between the angles, which
    // gives us arclength. we turn degress into radians, multiply by the radius
    // of the earth, and then use pythagorean distance
    
    double colat1 = colatitude(lat1);
    colat1 = to_radians(colat1);
    double colat2 = to_radians(colatitude(lat2));
    
    // compute difference in longitudes in radians
    double delta_lon = to_radians(lon1 - lon2);
    
    // spherical law of cosines to compute angle
    double x = acos(
        cos(colat1)*cos(colat2) + sin(colat1)*sin(colat2)*cos(delta_lon)
        );
        
    // convert radians to arc length 
    return x * EARTH_RADIUS_MILES;
}

// colatitude
double colatitude(double lat) {
    return LATITUDE_NORTH_POLE_DEGREES - lat;
}
