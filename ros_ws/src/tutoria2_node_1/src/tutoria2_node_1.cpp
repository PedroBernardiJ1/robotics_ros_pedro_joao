#include <iostream>
#include <memory>
#include <string>
#include <chrono>
#include <functional>
#include <cmath>

#include "rclcpp/rclcpp.hpp"
#include "geometry_msgs/msg/vector3.hpp"
#include "std_msgs/msg/float64.hpp"

using namespace std::chrono_literals;
using std::placeholders::_1;

class Node_cpp : public rclcpp::Node{

    private:
        // Subscriber at the topic point
        rclcpp::Subscription<geometry_msgs::msg::Vector3>::SharedPtr point_subscription_;  

        // Publisher on the topic distance and orientation 
        rclcpp::Publisher<std_msgs::msg::Float64>::SharedPtr publisher_distance;  
        rclcpp::Publisher<geometry_msgs::msg::Vector3>::SharedPtr publisher_orientation;  

        // Timer
        rclcpp::TimerBase::SharedPtr timer_;
        
        //Parameters 
        double x_, y_, z_; 

        void point_in_callback(const geometry_msgs::msg::Vector3::SharedPtr point){
            x_ = point->x;
            y_ = point->y;
            z_ = point->z;
        }

        void output_callback(){

            std_msgs::msg::Float64 distance_;
            distance_.data = sqrt((pow(x_, 2)) + (pow(y_, 2)) + (pow(z_, 2)));

            float theta_rad = atan2(y_, x_);
            float phi_rad = atan2(z_, (sqrt((pow(x_, 2)) + (pow(y_, 2)))));       

            geometry_msgs::msg::Vector3 orientation_;
            orientation_.x = theta_rad * (180/M_PI);
            orientation_.y = phi_rad * (180/M_PI);
            orientation_.z = 0.0;

            RCLCPP_INFO(
                this->get_logger(), 
                "Ponto: (x, y, z) = ('%f', '%f', '%f') | Distância: '%f' | Orientação: (x, y, z) = ('%f', '%f', '%f')", 
                x_, y_, z_, 
                distance_.data, 
                orientation_.x, 
                orientation_.y, 
                orientation_.z);
            
            publisher_distance->publish(distance_);
            publisher_orientation->publish(orientation_);
        }
    
    public:

        Node_cpp() : Node("tutoria2_node_1"), x_ {0.0} {

            point_subscription_ = this->create_subscription<geometry_msgs::msg::Vector3>(
                "point_in", 10,
                std::bind(&Node_cpp::point_in_callback, this, _1)
            );

            publisher_distance = this->create_publisher<std_msgs::msg::Float64>("/distance", 10); 
            publisher_orientation = this->create_publisher<geometry_msgs::msg::Vector3>("/orientation", 10);

            timer_ = this->create_wall_timer(
                500ms, 
                std::bind(&Node_cpp::output_callback, this)
            );
        }
};

int main(int argc, char * argv[]){

    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<Node_cpp>());
    rclcpp::shutdown();

    return 0;
}
