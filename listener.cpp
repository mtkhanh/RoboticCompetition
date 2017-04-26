#include <ros/ros.h>
#include "std_msgs/String.h"
#include "std_msgs/UInt16.h"
#include <geometry_msgs/Twist.h>
#include <math.h>
void moveF();
void moveTL();
void moveTR();
void turnLBIT();
int sensR();
int sensL();


int valL ;
int valR ;

ros::Publisher chatter_pub;
//Callback function for sensor 1
void sensorLCB(const std_msgs::UInt16 sensor1)
{
  valL = sensor1.data;
}

//Callback function for sensor 2
void sensorRCB(const std_msgs::UInt16 sensor2)
{
  valR = sensor2.data;
}



int main(int argc, char **argv)
{
  
  int state = 0;
  //initialize the node
  ros::init(argc, argv, "listener");


  ros::NodeHandle n;

  //initialize 5Hz rate
  ros::Rate loop_rate(10);


  //Create the subscriber
  ros::Subscriber sub_1 = n.subscribe("/groupJ/IR_sensor1", 1000, sensorLCB);
  ros::Subscriber sub_2 = n.subscribe("/groupJ/IR_sensor2", 1000, sensorRCB);

  chatter_pub = n.advertise<geometry_msgs::Twist>("/groupJ/cmd_vel", 1000);
  //led_rgb     = n.advertise<std_msgs::UInt8MultiArray>("/groupJ/RGB_led",1000);
 
  while(ros::ok()){
    int l ,r ;
    if (valR <= 13){
		r = 1;
	}
	else r = 0;
	if (valL <=10){
		l = 1;
	}
	else l = 0;
	
	if(valR <= 8){
		turnLBIT();
	}
	
    if (l == 1){
		moveTL();
		//moveF();
	} else if (r == 0){
		moveTR();
		//moveF();
	} else if (l == 0 && r == 1){
		moveF();
	}
	

	
	ROS_INFO("%d %d", l,r);
	ROS_INFO("%d %d", valL,valR);
	ros::spinOnce();
	loop_rate.sleep();
  }

  return 0;
}

void moveF(){
  geometry_msgs::Twist vel_msgs;
  vel_msgs.linear.x = 0.15;
  vel_msgs.linear.y = 0.0;
  vel_msgs.linear.z = 0.0;

  vel_msgs.angular.x = 0.0;
  vel_msgs.angular.y = 0.0;
  vel_msgs.angular.z = 0.0;

  
  ros::Rate loop_rate(10);
  chatter_pub.publish(vel_msgs);
  ros::spinOnce();
  loop_rate.sleep();
}

void moveTR(){
  geometry_msgs::Twist vel_msgs;
  vel_msgs.linear.x = 0.05;
  vel_msgs.linear.y = 0.0;
  vel_msgs.linear.z = 0.0;

  vel_msgs.angular.x = 0.0;
  vel_msgs.angular.y = 0.0;
  vel_msgs.angular.z = -0.4;  
  
  ros::Rate loop_rate(10);
  chatter_pub.publish(vel_msgs);
  ros::spinOnce();
  loop_rate.sleep();

}
void turnLBIT(){
  geometry_msgs::Twist vel_msgs;
  vel_msgs.linear.x = 0.1;
  vel_msgs.linear.y = 0.0;
  vel_msgs.linear.z = 0.0;

  vel_msgs.angular.x = 0.0;
  vel_msgs.angular.y = 0.0;
  vel_msgs.angular.z = 0.4;  
  
  ros::Rate loop_rate(10);
  chatter_pub.publish(vel_msgs);
  ros::spinOnce();
  loop_rate.sleep();
}
void moveTL(){
  geometry_msgs::Twist vel_msgs;
  vel_msgs.linear.x = 0.0;
  vel_msgs.linear.y = 0.0;
  vel_msgs.linear.z = 0.0;

  vel_msgs.angular.x = 0.0;
  vel_msgs.angular.y = 0.0;
  vel_msgs.angular.z =M_PI / 2.0 * 100.0;
  
  ros::Rate loop_rate(10);
  chatter_pub.publish(vel_msgs);
  ros::spinOnce();
  loop_rate.sleep();
}




