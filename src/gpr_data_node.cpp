#include "CommunicationManager.h"
#include <std_msgs/Float64.h>
//#include <mavros_msgs/Mavlink.h>
#include <geometry_msgs/PoseWithCovarianceStamped.h>
#include <iostream>
#include <fstream>
#include <ctype.h>

#define S_TIME      1

using namespace std;

double file_w_count;   // file write count variable
//int fw_mav_dummy = 0;  // file wrtie dummy variable
double fw_rssi = 0;  //file write variables
double fw_pose_x = 0;
double fw_pose_y = 0;

// pose msg callback function
void msgCallback_pose(const geometry_msgs::PoseWithCovarianceStamped::ConstPtr& msg_pose)
{
  fw_pose_x = msg_pose->pose.pose.position.x;
  fw_pose_y = msg_pose->pose.pose.position.y;
}

// test dummy callback function
/*void msgCallback_dummy(const mavros_msgs::Mavlink::ConstPtr& msg_dummy)
{
  fw_mav_dummy = msg_dummy->header.seq;
}*/

// rssi msg callback function
void msgCallback_rssi(const std_msgs::Float64::ConstPtr& msg_rssi)
{
  fw_rssi = msg_rssi->data;
}

// --------------------------------------------------------------------//
// MAIN function
int main(int argc, char **argv)
{
  ros::init(argc, argv, "gpr_data_node");
  if (argc != 2) // input validation
  {
    ROS_INFO("usage: gpr_data_node file_number , PLEASE use the conmmand properly");
    return 1;
  }
  std::string file_num_ = argv[1]; // the input argument : file number in the file name

  ros::NodeHandle nh;

  //std::string temp_str1 = "gpr_data_";
  //std::string temp_str2 = ".txt";
  //std::string the_file_name = temp_str1 + file_num_ + temp_str2;
  // Declare file obj
  ofstream file_wrtie_gpr_obj("gpr_data_" + file_num_ + ".txt");

  // Subscriber declaration
  ros::Subscriber pose_sub = nh.subscribe("/amcl_pose", 100, msgCallback_pose);
  ros::Subscriber rssi_sub = nh.subscribe("/rssi_val", 100, msgCallback_rssi);
  //ros::Subscriber dummy_sub = nh.subscribe("/inMavlink", 100, msgCallback_dummy);
  ros::Rate loop_rate(1/S_TIME);
	while (ros::ok()){

    //ROS_INFO("inMavlink msg :  %d ", fw_mav_dummy);
    ROS_INFO("RSSI value : %f  ", fw_rssi);
    ROS_INFO("AMCL pose  : ( %f , %f ) ", fw_pose_x, fw_pose_y);

    // Writing the file
    file_wrtie_gpr_obj << file_w_count << ", " << fw_pose_x << ", " << fw_pose_y << ", " << fw_rssi << endl;

    ros::spinOnce();
    loop_rate.sleep();
    file_w_count = file_w_count + 1;
  } // end while
  file_wrtie_gpr_obj.close();  // may NOT be implemented
  return 0;
} // end MAIN
