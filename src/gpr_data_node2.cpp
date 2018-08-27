#include "CommunicationManager.h"
#include <std_msgs/Float64.h>
//#include <mavros_msgs/Mavlink.h>
#include <geometry_msgs/PoseWithCovarianceStamped.h>
#include <iostream>
#include <fstream>
#include <ctype.h>

#define S_TIME      3

using namespace std;

double file_w_count;   // file write count variable
//int fw_mav_dummy = 0;  // file wrtie dummy variable
double fw_rssi1 = 0;  //file write variables
double fw_rssi2 = 0;
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
void msgCallback_rssi1(const std_msgs::Float64::ConstPtr& msg_rssi1)
{
  fw_rssi1 = msg_rssi1->data;
}

void msgCallback_rssi2(const std_msgs::Float64::ConstPtr& msg_rssi2)
{
  fw_rssi2 = msg_rssi2->data;
}

// --------------------------------------------------------------------//
// MAIN function
int main(int argc, char **argv)
{
  ros::init(argc, argv, "gpr_data_node2");
  if (argc != 4) // input validation
  {
    ROS_INFO("usage: gpr_data_node file_number + rssi_device1_number + rssi_device2_number , PLEASE use the conmmand properly");
    return 1;
  }
  std::string file_num_ = argv[1]; // the input argument : file number in the file name
  std::string short_id1_ = argv[2]; // the input short id : target rssi device short id number (device1)
  std::string short_id2_ = argv[3]; // the input short id : target rssi device short id number (device2)

  ros::NodeHandle nh;

  // Declare file obj
  ofstream file_wrtie_gpr_obj("gpr_data_" + file_num_ + ".txt");

  // Get rssi topic name
  std::string rssi_topic1 = "/rssi_val_" + short_id1_;
  std::string rssi_topic2 = "/rssi_val_" + short_id2_;

  // Subscriber declaration
  ros::Subscriber pose_sub = nh.subscribe("/amcl_pose", 100, msgCallback_pose);
  ros::Subscriber rssi_sub1 = nh.subscribe(rssi_topic1, 100, msgCallback_rssi1);
  ros::Subscriber rssi_sub2 = nh.subscribe(rssi_topic2, 100, msgCallback_rssi2);
  //ros::Subscriber dummy_sub = nh.subscribe("/inMavlink", 100, msgCallback_dummy);
  ros::Rate loop_rate(3); //S_TIME);
	while (ros::ok()){

    //ROS_INFO("inMavlink msg :  %d ", fw_mav_dummy);
    ROS_INFO("RSSI value 1 : %f  ", fw_rssi1);
    ROS_INFO("RSSI value 2 : %f  ", fw_rssi2);
    ROS_INFO("AMCL pose  : ( %f , %f ) ", fw_pose_x, fw_pose_y);

    // Writing the file
    file_wrtie_gpr_obj << file_w_count << ", " << fw_pose_x << ", " << fw_pose_y << ", " << fw_rssi1 << ", " << fw_rssi2 << endl;

    ros::spinOnce();
    loop_rate.sleep();
    file_w_count = file_w_count + 1;
  } // end while
  file_wrtie_gpr_obj.close();  // may NOT be implemented
  return 0;
} // end MAIN
