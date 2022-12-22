#include <memory>
#include "rclcpp/rclcpp.hpp"
#include "sensor_msgs/msg/joint_state.hpp"

using std::placeholders::_1;
/*
    Purpose: Receive wheel position information to calculate  
*/
class EncoderInformation : public rclcpp::Node
{
    public:
        EncoderInformation()
        : Node("encoder_information")
        {
           sub_ = this->create_subscription<sensor_msgs::msg::JointState>(
           "joint_state", 10, std::bind(&EncoderInformation::topic_callback, this, _1));   
        }

    private:
        void topic_callback(const sensor_msgs::msg::JointState & msg) const
        {
            RCLCPP_INFO(this->get_logger(), "I heard '%s'", msg.data.c_str());
        }
        rclcpp::Subscription<sensor_msgs::msg::JointState>::SharedPtr sub_;
};

int main(int argc, char * argv[])
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<EncoderInformation>());
    rclcpp::shutdown();
    return 0;
}