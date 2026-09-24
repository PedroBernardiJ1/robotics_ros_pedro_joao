#include <iostream>
#include <memory>
#include <string>
#include <chrono>
#include <functional>
#include <cmath>

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"
#include "std_msgs/msg/float64.hpp"

using namespace std::chrono_literals;

class tutoria_node_1 : public rclcpp::Node{

    private:

        rclcpp::Publisher<std_msgs::msg::Float64>::SharedPtr publisher_;
        rclcpp::TimerBase::SharedPtr timer_;
        size_t count;
        
        void msg(){
            std_msgs::msg::Float64 message;

            const double Pi {M_PI};
            double raio {10};
            double area {Pi * pow(raio, 2)};

            message.data = area;
            
            RCLCPP_INFO(this->get_logger(), "'%f'", message.data);

            publisher_->publish(message);
        }

    public:

        tutoria_node_1() : Node("publisher_node_area") , count {0} {
            publisher_ = this->create_publisher<std_msgs::msg::Float64>("publisher_node_area", 10); //QoS
            timer_ = this->create_wall_timer(500ms, std::bind(&tutoria_node_1::msg, this) );
        }
};

int main(int argc, char *argv[]){

    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<tutoria_node_1>());
    rclcpp::shutdown();

    return 0;
}