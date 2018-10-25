#include "CommunicationManager.h"
#include <std_msgs/Float64.h>


// MAIN function
int main(int argc, char **argv)
{
  // ROS initialization
  ros::init(argc, argv, "rssi_node");
  // Input arguments treatment
  if (argc != 3) // input validation
  {
    ROS_INFO("usage: rssi_node + source_id + target_id // PLEASE use the conmmand properly");
    return 1;
  }
  std::string source_id_ = argv[1]; // the short id of the xbee source device
  std::string target_id_ = argv[2]; // the short id of the xbee target device


  ros::NodeHandle nh; // node handle declaration

  // Service client objects(trigger and get) creation
  ros::ServiceClient client_trig_ = nh.serviceClient<mavros_msgs::ParamGet>("/xbee_status");
  ros::ServiceClient client_get_ = nh.serviceClient<mavros_msgs::ParamGet>("/xbee_status");
  // Publisher cliend obj
  std::string topic_name = "rssi" + source_id_ + target_id_;
  ros::Publisher pub_rssi = nh.advertise<std_msgs::Float64>(topic_name,100);

  // Service messages creation : mavros service format
  mavros_msgs::ParamGet srv_trig;
  mavros_msgs::ParamGet srv_get;
  // Topic message creation : std_msgs UInt8/Float64 format
  std_msgs::Float64 msg;

  // Request contents
  std::string srv_name_str1 = "trig_rssi_api_";
  std::string srv_name_str2 = "get_rssi_api_";
  srv_trig.request.param_id = srv_name_str1 + target_id_;   //e.g. "trig_rssi_api_8";
  srv_get.request.param_id =  srv_name_str2 + target_id_;   //e.g. "get_rssi_api_8";

  double Loop_Hz = 2.0; // plz put your number in double format 2(x)->2.0(o)
  double pause_time = (1/Loop_Hz)*0.5;  // 40% of the loop_rate is waited
  ros::Rate Loop_Rate(Loop_Hz);  // (Loop_Hz)[HZ] = **[s] , rate
  while (ros::ok())
  {
    // Call the service : rssi test link trigger
    if (client_trig_.call(srv_trig))
    {
      ROS_INFO("srvClient_trig_ has been successfully called!!");
    }
    else
    {
      ROS_ERROR("Failed to call service rssi_trig");
    }

    // Wait for a while;
    ros::Duration(pause_time).sleep();

    // Call the serivce : rssi test link access
    if (client_get_.call(srv_get))
    {
      msg.data = srv_get.response.value.real;
      if (msg.data > 18.0)
      {
        ROS_INFO("srvClient_get_ has been successfully called!!");
        //ROS_INFO("RSSI : %.1f",msg.data);
        printf("     RSSI: %.1f \n", msg.data);
        pub_rssi.publish(msg);
      }
      else  // when the value is too small
      {
        msg.data = -1.0;
        pub_rssi.publish(msg);
        ROS_ERROR("The value of the rssi seems to be wrong");
      }

    }
    else
    {
      msg.data = -2.0;
      pub_rssi.publish(msg);
      ROS_ERROR("Failed to call service rssi_get");
      //return 1; Do Not terminate this node although there's no rssi input
      //should keep runing until ctrl C is cominig in
    }
    ros::spinOnce();
    Loop_Rate.sleep();
  }// end while

  return 0;
} // MAIN end
