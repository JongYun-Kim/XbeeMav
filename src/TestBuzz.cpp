/* RosBuzz.cpp -- Basic ROS Buzz node --               */
/* ------------------------------------------------------------------------- */
/* September 20, 2016 -- @Copyright Aymen Soussia. All rights reserved.      */
/*                                  (aymen.soussia@gmail.com)                */


#include <climits>
#include <stdlib.h> 
#include <time.h> 

#include <mavros_msgs/Mavlink.h>
#include <ros/ros.h>



//*****************************************************************************
void Receive_Payload_Callback(const mavros_msgs::Mavlink::ConstPtr& mavlink_msg)
{
	std::cout << "Received Mavlink Message:" << std::endl;
	for (unsigned short i = 0; i < mavlink_msg->payload64.size(); i++)
		std::cout << mavlink_msg->payload64.at(i) << std::endl;
	std::cout << std::endl;
}


//*****************************************************************************
unsigned int Get_Random_Size(unsigned int min, unsigned int max)
{
	return rand() % (max - min) + min;
}


//*****************************************************************************
unsigned long long Get_Random_Int64(unsigned long long min, unsigned long long max)
{
	return rand() % (max - min) + min;
}


//*****************************************************************************
void Init_Random_Seed()
{
	srand (time(NULL));
}


//*****************************************************************************
int main(int argc, char **argv)
{
  const unsigned int MIN_PAYLOAD_SIZE = 750;
  const unsigned int MAX_PAYLOAD_SIZE = 752;
  
  ros::init(argc, argv, "test_buzz");

  ros::NodeHandle node_handle;
  ros::Publisher mavlink_pub = node_handle.advertise<mavros_msgs::Mavlink>("outMavlink", 1000);
  ros::Subscriber mavlink_sub = node_handle.subscribe("inMavlink", 1000, Receive_Payload_Callback);

  Init_Random_Seed();

  int count = 0;
  const std::size_t MAX_BUFFER_SIZE = 64;
  char line[MAX_BUFFER_SIZE];
  
  std::cout << "Press Enter to Send New Mavlink Message..." << std::endl;
  
  while (ros::ok() && std::cin.getline(line, MAX_BUFFER_SIZE))
  {
    mavros_msgs::Mavlink mavlink_msg_;
    mavlink_msg_.sysid = 2;
    mavlink_msg_.msgid = 1;

    unsigned int payload_size = Get_Random_Size(MIN_PAYLOAD_SIZE, MAX_PAYLOAD_SIZE);

    for (unsigned int i = 0; i < payload_size; i++)
    {
      mavlink_msg_.payload64.push_back(Get_Random_Int64(1, ULLONG_MAX));
    }

    std::cout << "Sent Mavlink Message  " << count << std::endl;

    for (unsigned int i = 0; i < payload_size; i++)
    {
      std::cout << mavlink_msg_.payload64.at(i) << std::endl;
    }

    std::cout << std::endl;

    mavlink_pub.publish(mavlink_msg_);

    ros::spinOnce();

    count++;
    std::cout << "Press Enter to Send New Mavlink Message..." << std::endl;
  }

  return 0;
}
