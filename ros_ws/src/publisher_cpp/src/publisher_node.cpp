#include <iostream>
#include <memory>
#include <string>
#include <chrono>
#include <functional>

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"
#include "std_msgs/msg/int32.hpp"

using namespace std::chrono_literals;

class PublisherNode : public rclcpp::Node{

    private:

        rclcpp::Publisher<std_msgs::msg::String>::SharedPtr publisher_;
        rclcpp::TimerBase::SharedPtr timer_;
        size_t count;
        
        void msg(){
            std_msgs::msg::String message;
            message.data = "Olá, Mundo: " + std::to_string(count++);
            
            RCLCPP_INFO(this->get_logger(), "Publicando: '%s'", message.data.c_str());

            publisher_->publish(message);
        }

    public:

        PublisherNode() : Node("publisher_node") , count {0} {
            publisher_ = this->create_publisher<std_msgs::msg::String>("topico_exemplo", 10); //QoS
            timer_ = this->create_wall_timer(500ms, std::bind(&PublisherNode::msg, this) );
        }
};

int main(int argc, char * argv[]){

    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<PublisherNode>());
    rclcpp::shutdown();

    return 0;
}