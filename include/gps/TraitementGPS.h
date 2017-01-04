/*
 * TraitementGPS.h
 *
 *  Created on: 2 déc. 2016
 *      Author: amaury
 */

#ifndef TRAITEMENTGPS_H_
#define TRAITEMENTGPS_H_
#include <string>
#include "std_msgs/String.h"

class TraitementGPS {
public:
	TraitementGPS();
	virtual ~TraitementGPS();
	int creerNode();
};

#endif /* TRAITEMENTGPS_H_ */
