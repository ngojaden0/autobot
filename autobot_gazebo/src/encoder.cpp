#include <memory>
#include <chrono>
#include <functional>
#include <memory>
#include <string>
#include <math.h>

#include "rclcpp/rclcpp.hpp"
#include "sensor_msgs/msg/joint_state.hpp"
#include "geometry_msgs/msg/transform_stamped.hpp"
#include "tf2/LinearMath/Quaternion.h"
#include "tf2_ros/transform_broadcaster.h"
#include "autobot_information.hpp"

using std::placeholders::_1;

/*
    Purpose: Receive wheel encoder information to calculate pose and transform 
    Subscribe: /joint_states
    Broadcast: /tf 
*/

class EncoderInformation : public rclcpp::Node //initialization list (inherit Node)
{
    public:
        EncoderInformation() //constructor calling Node class with arg name
        : Node("encoder_information") 
        {
            //assign variable to subscribe to /joint_states        
            sub_ = this->create_subscription<sensor_msgs::msg::JointState>(
            "/joint_states", 10, std::bind(&EncoderInformation::handle_autobot_pose, this, _1));  
            tf_broadcaster_ = std::make_unique<tf2_ros::TransformBroadcaster>(*this);
        }

    private:
        void handle_autobot_pose(const sensor_msgs::msg::JointState & msg)
        {

            curr_left_back_pos = msg.position[0];
            curr_right_back_pos = msg.position[1];
            curr_left_front_pos = msg.position[2];
            curr_right_front_pos = msg.position[3];

            left_back_vel = ((curr_left_back_pos - prev_left_back_pos)/dt)*WHEEL_DIAMETER/2;
            right_back_vel = ((curr_right_back_pos - prev_right_back_pos)/dt)*WHEEL_DIAMETER/2;
            left_front_vel = ((curr_left_front_pos - prev_left_front_pos)/dt)*WHEEL_DIAMETER/2;
            right_front_vel = ((curr_right_front_pos - prev_right_front_pos)/dt)*WHEEL_DIAMETER/2;

            left_vel = (left_back_vel + left_front_vel)/2;
            right_vel = (right_back_vel + right_front_vel)/2;
            avg_vel = (left_vel + right_vel)/2;
            angular_vel = (right_vel - left_vel)/AUTOBOT_LENGTH;

            dth = angular_vel*dt;
            dxy = avg_vel*dt;

            dx = dxy*cos(dth);
            dy = dxy*sin(dth);

            x += (cos(th)*dx-sin(th)*dy);
            y += (sin(th)*dx+cos(th)*dy);
            th += dth;

            prev_left_back_pos = curr_left_back_pos;
            prev_right_back_pos = curr_right_back_pos;
            prev_left_front_pos = curr_left_front_pos;
            prev_right_front_pos = curr_right_front_pos;

            if(th >= 2*M_PI)
                th -= 2*M_PI;    
            if(th <= -2*M_PI)
                th += 2*M_PI;    
            
            t.header.stamp = this->get_clock()->now();
            t.header.frame_id = "odom";
            t.child_frame_id = "encoder";

            t.transform.translation.x = x;
            t.transform.translation.y = y;
            t.transform.translation.z = 0.0;

            tf2::Quaternion q;
            q.setRPY(0, 0, th);
            t.transform.rotation.x = q.x();
            t.transform.rotation.y = q.y();
            t.transform.rotation.z = q.z();
            t.transform.rotation.w = q.w();

            tf_broadcaster_->sendTransform(t);
        }

        geometry_msgs::msg::TransformStamped t;
        rclcpp::Subscription<sensor_msgs::msg::JointState>::SharedPtr sub_;
        std::unique_ptr<tf2_ros::TransformBroadcaster> tf_broadcaster_;
        float curr_left_back_pos, curr_right_back_pos, curr_left_front_pos, curr_right_front_pos;
        float prev_left_back_pos = 0, prev_right_back_pos = 0, prev_left_front_pos = 0, prev_right_front_pos = 0;
        float left_back_vel, right_back_vel, left_front_vel, right_front_vel;
        float left_vel, right_vel, avg_vel;
        float angular_vel;
        float dt = 0.1, dxy, dth, dx, dy;
        float x = 0, y = 0, th = 0; 
};

int main(int argc, char * argv[])
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<EncoderInformation>());
    rclcpp::shutdown();
    return 0;
}