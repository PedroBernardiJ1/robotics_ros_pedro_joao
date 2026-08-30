#include <memory>
#include <string>
#include <functional>

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"

using std::placeholders::_1;

class SubscriberNode : public rclcpp::Node{

    private:

        rclcpp::Subscription<std_msgs::msg::String>::SharedPtr subscription_;
        std_msgs::msg::String::SharedPtr ptr;

            void topic_callback(const std_msgs::msg::String::SharedPtr msg){
                ptr = msg;
                std::string var = msg->data;
                RCLCPP_INFO(this->get_logger(), "Subscription: '%s'", msg->data.c_str());
            }
    
    public:

        SubscriberNode() : Node("subscriber_node") {
            subscription_ = this->create_subscription<std_msgs::msg::String>(
                "topico_exemplo", 10,
                std::bind(&SubscriberNode::topic_callback, this, _1)
            );
        }
};

int main(int argc, char * argv[]){

    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<SubscriberNode>());
    rclcpp::shutdown();

    return 0;
}
