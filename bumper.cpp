#include "ros/ros.h"
#include <geometry_msgs/Twist.h>
#include <kobuki_msgs/BumperEvent.h>
#include <kobuki_msgs/CliffEvent.h>

geometry_msgs::Twist vmsg;
ros::Publisher cmd_vel_pub;
double bumper;
double state;

void publishVel(){
  while(ros::ok()){
    cmd_vel_pub.publish(vmsg);
    ros::Duration(3).sleep();
    ros::spinOnce();
  }
}

void bumperCallback(const kobuki_msgs::BumperEvent::ConstPtr& msg){
  bumper = msg->bumper;
  state = msg->state;
  ROS_INFO("bumper: %f, state: %f", bumper, state);
  while(ros::ok()){
  if(bumper == 1 && state == 1){
    vmsg.linear.x = -0.17;
    vmsg.angular.z = 10;
    cmd_vel_pub.publish(vmsg);
      ros::Duration(1).sleep();
      ros::spinOnce();
    ROS_INFO("Central bumper triggered");
  }
  else if(bumper == 0 && state == 1){
    vmsg.linear.x = -0.13;
    vmsg.angular.z = -10;
    cmd_vel_pub.publish(vmsg);
      ros::Duration(1).sleep();
      ros::spinOnce();
    ROS_INFO("Left bumper triggered");
  }
  else if(bumper == 2 && state == 1){
    vmsg.linear.x = -0.13;
    vmsg.angular.z = 10;
    cmd_vel_pub.publish(vmsg);
      ros::Duration(1).sleep();
      ros::spinOnce();
    ROS_INFO("Right bumper triggered");
  }
}
}

int main(int argc, char **argv) {
  ros::init(argc, argv, "bumper");
  ros::NodeHandle n;
  ros::Subscriber sub = n.subscribe("/mobile_base/events/bumper", 1, bumperCallback);
  cmd_vel_pub = n.advertise<geometry_msgs::Twist>("/cmd_vel_mux/input/navi", 1);
  vmsg.linear.x = 0.17;
  vmsg.angular.z = 0.0;
  publishVel();

  return 0;
}
