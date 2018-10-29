#include "CommunicationManager.h"
#include <std_msgs/Float64.h>
//#include <mavros_msgs/Mavlink.h>
#include <geometry_msgs/PoseWithCovarianceStamped.h>
#include <iostream>
#include <fstream>
#include <ctype.h>

#define S_TIME      2

using namespace std;

double file_w_count;   // file write count variable

double fw_pose_c1x = 0;
double fw_pose_c1y = 0;
double fw_pose_c2x = 0;
double fw_pose_c2y = 0;
double fw_pose_r1x = 0;
double fw_pose_r1y = 0;

// pose msg callback function
void msgCallback_pose_c1(const geometry_msgs::PoseWithCovarianceStamped::ConstPtr& msg_pose_c1)
{
  fw_pose_c1x = msg_pose_c1->pose.pose.position.x;
  fw_pose_c1y = msg_pose_c1->pose.pose.position.y;
}
void msgCallback_pose_c2(const geometry_msgs::PoseWithCovarianceStamped::ConstPtr& msg_pose_c2)
{
  fw_pose_c2x = msg_pose_c2->pose.pose.position.x;
  fw_pose_c2y = msg_pose_c2->pose.pose.position.y;
}
void msgCallback_pose_r1(const geometry_msgs::PoseWithCovarianceStamped::ConstPtr& msg_pose_r1)
{
  fw_pose_r1x = msg_pose_r1->pose.pose.position.x;
  fw_pose_r1y = msg_pose_r1->pose.pose.position.y;
}
// test dummy callback function
/*void msgCallback_dummy(const mavros_msgs::Mavlink::ConstPtr& msg_dummy)
{
  fw_mav_dummy = msg_dummy->header.seq;
}*/

// --------------------------------------------------------------------//
// MAIN function
int main(int argc, char **argv)
{
  ros::init(argc, argv, "bagplay");
  if (argc != 2) // input validation
  {
    ROS_INFO("usage: gpr_data_node file_number + rssi_device1_number + rssi_device2_number , PLEASE use the conmmand properly");
    return 1;
  }
  std::string file_num_ = argv[1]; // the input argument : file number in the file name
  //std::string short_id1_ = argv[2]; // the input short id : target rssi device short id number (device1)
  //std::string short_id2_ = argv[3]; // the input short id : target rssi device short id number (device2)

  ros::NodeHandle nh;

  // Declare file obj
  ofstream file_wrtie_obj("bag_data_" + file_num_ + ".txt");

  // Subscriber declaration
  ros::Subscriber pose_sub_c1 = nh.subscribe("/turtlebot_2/amcl_pose", 100, msgCallback_pose_c1);
  ros::Subscriber pose_sub_c2 = nh.subscribe("/turtlebot_1/amcl_pose", 100, msgCallback_pose_c2);
  ros::Subscriber pose_sub_r1 = nh.subscribe("/amcl_pose", 100, msgCallback_pose_r1);
  //ros::Subscriber dummy_sub = nh.subscribe("/inMavlink", 100, msgCallback_dummy);

  ros::Rate loop_rate(2); //S_TIME);
	while (ros::ok()){

    //ROS_INFO("inMavlink msg :  %d ", fw_mav_dummy);
    ROS_INFO("Client1 pose  : ( %f , %f ) ", fw_pose_c1x, fw_pose_c1y);
    ROS_INFO("Client2 pose  : ( %f , %f ) ", fw_pose_c2x, fw_pose_c2y);
    ROS_INFO("Relay 1 pose  : ( %f , %f ) ", fw_pose_r1x, fw_pose_r1y);

    // Writing the file
    file_wrtie_obj << file_w_count << ", " << fw_pose_c1x << ", " << fw_pose_c1y << ", " << fw_pose_c2x << ", " << fw_pose_c2y << ", " << fw_pose_r1x << ", " << fw_pose_r1y << endl;

    ros::spinOnce();
    loop_rate.sleep();
    file_w_count = file_w_count + 1;
  } // end while
  file_wrtie_obj.close();  // may NOT be implemented
  return 0;
} // end MAIN
