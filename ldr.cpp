#include "ros/ros.h"
#include "std_msgs/String.h"
#include "std_msgs/UInt16.h"
#include "geometry_msgs/Pose2D.h"
#include "geometry_msgs/Twist.h"
#include <stdlib.h>
#include <math.h>
#include <random>
#define Vmax 0.3
#define Wmax 4.3

void get_best();
//GLOBAL VALUE
int group_ldr[14] = {0};
int count  = 0;
float group_x[14] = {0.0};
float group_y[14] = {0.0};
float group_theta[14] = {0.0};

int global_best;
int best_read;
int current_read;

float lin, ang;
float global_x;
float global_y;
float best_pos_x;
float best_pos_y;
float current_pos_x;
float current_pos_y;
float current_pos_theta;
float past_vel_x = 0.01;
float past_vel_y = 0.5;
float current_vel_x;
float current_vel_y;
float w = 0.9,p1 = 1.2, p2 = 0.8;
float r1,r2;
ros::Publisher chatter_pub;
//Subscribe LDR
void ldrA(const std_msgs::UInt16 ldr_A){  group_ldr[0] = ldr_A.data;}
void ldrB(const std_msgs::UInt16 ldr_B){  group_ldr[1] = ldr_B.data;}
void ldrC(const std_msgs::UInt16 ldr_C){  group_ldr[2] = ldr_C.data;}
void ldrD(const std_msgs::UInt16 ldr_D){  group_ldr[3] = ldr_D.data;}
void ldrE(const std_msgs::UInt16 ldr_E){  group_ldr[4] = ldr_E.data;}
void ldrF(const std_msgs::UInt16 ldr_F){  group_ldr[5] = ldr_F.data;}
void ldrG(const std_msgs::UInt16 ldr_G){  group_ldr[6] = ldr_G.data;}
void ldrH(const std_msgs::UInt16 ldr_H){  group_ldr[7] = ldr_H.data;}
void ldrI(const std_msgs::UInt16 ldr_I){  group_ldr[8] = ldr_I.data;}
void ldrJ(const std_msgs::UInt16 ldr_J){  group_ldr[9] = ldr_J.data; current_read = ldr_J.data; }
void ldrK(const std_msgs::UInt16 ldr_K){  group_ldr[10] = ldr_K.data;}
void ldrL(const std_msgs::UInt16 ldr_L){  group_ldr[11] = ldr_L.data;}
void ldrM(const std_msgs::UInt16 ldr_M){  group_ldr[12] = ldr_M.data;}
void ldrN(const std_msgs::UInt16 ldr_N){  group_ldr[13] = ldr_N.data;}

//Subscribe odom
void odomA(const geometry_msgs::Pose2D odom_A){ group_x[0] = odom_A.x ; group_y[0] = odom_A.y; group_theta[0] = odom_A.theta;}
void odomB(const geometry_msgs::Pose2D odom_B){ group_x[1] = odom_B.x ; group_y[1] = odom_B.y; group_theta[1] = odom_B.theta;}
void odomC(const geometry_msgs::Pose2D odom_C){ group_x[2] = odom_C.x ; group_y[2] = odom_C.y; group_theta[2] = odom_C.theta;}
void odomD(const geometry_msgs::Pose2D odom_D){ group_x[3] = odom_D.x ; group_y[3] = odom_D.y; group_theta[3] = odom_D.theta;}
void odomE(const geometry_msgs::Pose2D odom_E){ group_x[4] = odom_E.x ; group_y[4] = odom_E.y; group_theta[4] = odom_E.theta;}
void odomF(const geometry_msgs::Pose2D odom_F){ group_x[5] = odom_F.x ; group_y[5] = odom_F.y; group_theta[5] = odom_F.theta;}
void odomG(const geometry_msgs::Pose2D odom_G){ group_x[6] = odom_G.x ; group_y[6] = odom_G.y; group_theta[6] = odom_G.theta;}
void odomH(const geometry_msgs::Pose2D odom_H){ group_x[7] = odom_H.x ; group_y[7] = odom_H.y; group_theta[7] = odom_H.theta;}
void odomI(const geometry_msgs::Pose2D odom_I){ group_x[8] = odom_I.x ; group_y[8] = odom_I.y; group_theta[8] = odom_I.theta;}
void odomJ(const geometry_msgs::Pose2D odom_J){ group_x[9] = odom_J.x ; group_y[9] = odom_J.y; group_theta[9] = odom_J.theta; current_pos_x = odom_J.x; current_pos_y = odom_J.y; current_pos_theta = odom_J.theta;}
void odomK(const geometry_msgs::Pose2D odom_K){ group_x[10] = odom_K.x ; group_y[10] = odom_K.y; group_theta[10] = odom_K.theta;}
void odomL(const geometry_msgs::Pose2D odom_L){ group_x[11] = odom_L.x ; group_y[11] = odom_L.y; group_theta[11] = odom_L.theta;}
void odomM(const geometry_msgs::Pose2D odom_M){ group_x[12] = odom_M.x ; group_y[12] = odom_M.y; group_theta[12] = odom_M.theta;}
void odomN(const geometry_msgs::Pose2D odom_N){ group_x[13] = odom_N.x ; group_y[13] = odom_N.y; group_theta[13] = odom_N.theta;}





int main(int argc, char **argv)
{
  ros::init(argc, argv, "ldr");
  ros::NodeHandle n;

  ros::Subscriber groupA_LDR = n.subscribe("/groupA/ldr", 1000, ldrA);
  ros::Subscriber groupB_LDR = n.subscribe("/groupB/ldr", 1000, ldrB);
  ros::Subscriber groupC_LDR = n.subscribe("/groupC/ldr", 1000, ldrC);
  ros::Subscriber groupD_LDR = n.subscribe("/groupD/ldr", 1000, ldrD);
  ros::Subscriber groupE_LDR = n.subscribe("/groupE/ldr", 1000, ldrE);
  ros::Subscriber groupF_LDR = n.subscribe("/groupF/ldr", 1000, ldrF);
  ros::Subscriber groupG_LDR = n.subscribe("/groupG/ldr", 1000, ldrG);
  ros::Subscriber groupH_LDR = n.subscribe("/groupH/ldr", 1000, ldrH);
  ros::Subscriber groupI_LDR = n.subscribe("/groupI/ldr", 1000, ldrI);
  ros::Subscriber groupJ_LDR = n.subscribe("/groupJ/ldr", 1000, ldrJ);
  ros::Subscriber groupK_LDR = n.subscribe("/groupK/ldr", 1000, ldrK);
  ros::Subscriber groupL_LDR = n.subscribe("/groupL/ldr", 1000, ldrL);
  ros::Subscriber groupM_LDR = n.subscribe("/groupM/ldr", 1000, ldrM);
  ros::Subscriber groupN_LDR = n.subscribe("/groupN/ldr", 1000, ldrN);

  ros::Subscriber groupA_ODOM = n.subscribe("/groupA/odom", 1000, odomA);
  ros::Subscriber groupB_ODOM = n.subscribe("/groupB/odom", 1000, odomB);  
  ros::Subscriber groupC_ODOM = n.subscribe("/groupC/odom", 1000, odomC);
  ros::Subscriber groupD_ODOM = n.subscribe("/groupD/odom", 1000, odomD);
  ros::Subscriber groupE_ODOM = n.subscribe("/groupE/odom", 1000, odomE);
  ros::Subscriber groupF_ODOM = n.subscribe("/groupF/odom", 1000, odomF);
  ros::Subscriber groupG_ODOM = n.subscribe("/groupG/odom", 1000, odomG);
  ros::Subscriber groupH_ODOM = n.subscribe("/groupH/odom", 1000, odomH);
  ros::Subscriber groupI_ODOM = n.subscribe("/groupI/odom", 1000, odomI);
  ros::Subscriber groupJ_ODOM = n.subscribe("/groupJ/odom", 1000, odomJ);
  ros::Subscriber groupK_ODOM = n.subscribe("/groupK/odom", 1000, odomK);
  ros::Subscriber groupL_ODOM = n.subscribe("/groupL/odom", 1000, odomL);
  ros::Subscriber groupM_ODOM = n.subscribe("/groupM/odom", 1000, odomM);
  ros::Subscriber groupN_ODOM = n.subscribe("/groupN/odom", 1000, odomN);

  chatter_pub = n.advertise<geometry_msgs::Twist>("/groupJ/cmd_vel", 1000);

  ros::Rate loop_rate(10);
  std::random_device rd;
  std::mt19937 mt(rd());
  std::uniform_real_distribution<double> dist(0.0,10.0);

  while (ros::ok())
  {
    get_best();
    if (current_read >= best_read){
      best_read = current_read;
      best_pos_x = current_pos_x;
      best_pos_y = current_pos_y;
    }

    if( ++count > 20){
    	count = 0;

	    r1 = (float)(dist(mt)) / 10.0;
	    r2 = (float)(dist(mt)) / 10.0;
	    current_vel_x =  (float) (w  * past_vel_x + p1 * r1 * (best_pos_x - current_pos_x)); //+ p2 * r2 * (global_x - current_pos_x);
	    //current_pos_x += current_vel_x;
	    if(current_vel_x > 0.3){
	    	current_vel_x = 0.3;
	    }else if (current_vel_x < -0.3){
	    	current_vel_x  = -0.3;
	    }
	    r1 = (float)(dist(mt)) / 10.0;
	    r2 = (float)(dist(mt)) / 10.0;
	    current_vel_y =  (float) (w  * past_vel_y + p1 * r1 * (best_pos_y - current_pos_y)); //+ p2 * r2 * (global_y - current_pos_y);
	    //current_pos_y += current_vel_y;
	    if(current_vel_y > 0.3){
	    	current_vel_y = 0.3;
	    }else if (current_vel_y < -0.3){
	    	current_vel_y  = -0.3;
	    }
	    float theta_now = atan2(current_vel_y,current_vel_x);
	    float Vx_robot = sqrt(current_vel_x * current_vel_x + current_vel_y * current_vel_y) * cos(theta_now - current_pos_theta);
	    float Vy_robot = sqrt(current_vel_x * current_vel_x + current_vel_y * current_vel_y) * sin(theta_now - current_pos_theta);
	    
	    past_vel_x= current_vel_x;
	    past_vel_y = current_vel_y;
	    
	    //assign the linear and angular speed
	    if(fabs(Vx_robot) > Vmax){
	    	lin = Vmax;
	    } else{
	    	lin = Vx_robot;
	    }

		ang = atan2(Vy_robot,(Vx_robot))/2.0;

		if (fabs(ang) > Wmax){
			ang = (fabs(ang) / (ang+0.00001)) * Wmax;
		}
		
		ROS_INFO("%d %d",current_read,best_read);
		ROS_INFO("vx = %f; vy = %f",current_vel_x,current_vel_y);
		ROS_INFO("%f %f %f",Vx_robot,Vy_robot,theta_now);
		ROS_INFO("Best : %f %f Current : %f %f",best_pos_x,best_pos_y,current_pos_x,current_pos_y);
		geometry_msgs::Twist speed;
		speed.linear.x = lin;
		speed.angular.z = ang;
		chatter_pub.publish(speed);
	}


    ros::spinOnce();
    loop_rate.sleep();
  }

  return 0;
}	

void get_best(){
  for (int i = 0;i < 14;i++){
    if(group_ldr[i] > global_best){
      global_best = group_ldr[i];
      global_x = group_x[i];
      global_y = group_y[i];
    }
  }
}
