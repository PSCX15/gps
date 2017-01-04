/*
 * TraitementGPS.cpp
 *
 *  Created on: 2 déc. 2016
 *      Author: amaury
 */


#include <string>
#include <vector>
#include "ros/ros.h"
#include "std_msgs/String.h"

#include <sstream>

#include "gps/GPS_refined.h"
#include "PointGPS.h"


std::vector<int> gpsAssignes;

std::vector<PointGPS*> listeGPS;
std::vector<PointGPS*> listePourMoyenne;
PointGPS* positionMoyenne;
int count = 0;
float currentTime = 0; 

int nombreGPS;

void chatterCallback(const gps::GPS_refined::ConstPtr& msg)
{
	int num = msg->numero;
	if(count<4){
		if(gpsAssignes[num]==-1){
			listeGPS[num] = new PointGPS(msg->latitude, msg->longitude, msg->numero);
			//listeGPS[num] = gps;
			                
			gpsAssignes[num]=num;
			count++;
			ROS_INFO("GPS numero %i ajoute", num);
		}
	}
	
	ROS_INFO("%f",msg->time);
	if(currentTime +0.3 < msg->time){
		currentTime = msg->time;
		listePourMoyenne.clear();
	}
	
	listeGPS[num]->latitude = msg->latitude;
	listeGPS[num]->longitude = msg->longitude;
	listeGPS[num]->time = msg->time;
	listeGPS[num]->hop = msg->accuracy;

	
	listePourMoyenne.push_back(listeGPS[num]);	

	ROS_INFO("il y a %lu gps dans la liste intermediaire", listePourMoyenne.size());
	/*
	for (int i=0; i<4; i++){
		if(gpsAssignes[i]!=-1){
			if(gpsAssigneFresh[i]!=-1){
				listePourMoyenne.push_back(listeGPS[i]);
				gpsAssigneFresh[i]=-1;
			}
		}
	}
	*/

	if(listePourMoyenne.size()==nombreGPS){
		int nombreGPSUtilises = positionMoyenne->moyenneIntelligente(listePourMoyenne);

  	ROS_INFO("I used %i gps to get this position : N %f , E %f",nombreGPSUtilises, positionMoyenne->latitude, positionMoyenne->longitude);
  	listePourMoyenne.clear();
	}
}



int main(int argc, char **argv)
{

  ros::init(argc, argv, "main");
  
  ros::NodeHandle n;
  
  ros::Rate loop_rate(10);
  
  gpsAssignes.reserve(4);
  
  for (int i =0; i<4;i++){
  	gpsAssignes.push_back(-1);
  }
  
  positionMoyenne = new PointGPS();
  positionMoyenne->numero = -1;
  
  listeGPS.resize(4);
	
	ros::Subscriber sub = n.subscribe("GPSavance", 1000, chatterCallback);

  while(ros::ok()){

  nombreGPS = sub.getNumPublishers();
  ros::param::set("/numberOfGPS",nombreGPS);

  ros::spinOnce();

  loop_rate.sleep(); 
  }

  return 0;

}



