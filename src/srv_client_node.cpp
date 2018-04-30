#include "CommunicationManager.h"
//#include <string>

// MAIN function
int main(int argc, char **argv)
{
  // ROS initialization
  ros::init(argc, argv, "srv_client_node");
  // Input arguments treatment
  if (argc != 2) // input validation
  {
    ROS_INFO("usage: srv_client_node Target_short_ID , PLEASE use the conmmand properly");
    return 1;
  }
  std::string short_id_ = argv[1]; // the input short id


  ros::NodeHandle nh; // node handle declaration

  // Service client objects(trigger and get) creation
  ros::ServiceClient client_trig_ = nh.serviceClient<mavros_msgs::ParamGet>("/xbee_status");
  ros::ServiceClient client_get_ = nh.serviceClient<mavros_msgs::ParamGet>("/xbee_status");


  // Service messages creation : mavros service format
  mavros_msgs::ParamGet srv_trig;
  mavros_msgs::ParamGet srv_get;

  // Request contents
  std::string temp_str1 = "trig_rssi_api_";
  std::string temp_str2 = "get_rssi_api_";
  srv_trig.request.param_id = temp_str1 + short_id_;   //"trig_rssi_api_8"; // later, specify the node ID with the argument input
  srv_get.request.param_id =  temp_str2 + short_id_; //"get_rssi_api_8";

  ros::Rate Loop_Rate(0.5);  // 1HZ rate
  while (ros::ok())
  {
    // Call the service : rssi test link trigger
    if (client_trig_.call(srv_trig))
    {
      ROS_INFO("Client_trig_ has been successfully called!!");
    }
    else
    {
      ROS_ERROR("Failed to call service rssi_trig");
    }

    // Wait for a while; 1 second
    ros::Duration(1.0).sleep();

    // Call the serivce : rssi test link access
    float the_rssi;
    if (client_get_.call(srv_get))
    {
      ROS_INFO("Client_get_ has been successfully called!!");
      the_rssi = srv_get.response.value.real;
      printf(" RSSI: %f \n", the_rssi);
      //return the_rssi;
      //return 0;
    }
    else
    {
      ROS_ERROR("Failed to call service rssi_get");
      return 1;
    }
    ros::spinOnce();
    Loop_Rate.sleep();
  }// end while

  return 0;
} // MAIN end
