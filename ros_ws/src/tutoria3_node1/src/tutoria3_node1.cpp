#include <iostream>
#include <memory>
#include <chrono>
#include <cmath>

#include "rclcpp/rclcpp.hpp"
#include "nav_msgs/msg/odometry.hpp"
#include "sensor_msgs/msg/imu.hpp"

using namespace std::chrono_literals;
using std::placeholders::_1;

class tutoria3_node1 : public rclcpp::Node{

    private:  

        // Publisher on the topic /imu and /odom 
        rclcpp::Publisher<nav_msgs::msg::Odometry>::SharedPtr publisher_odom;  
        rclcpp::Publisher<sensor_msgs::msg::Imu>::SharedPtr publisher_imu;  

        // Timer
        rclcpp::TimerBase::SharedPtr timer_;
        
        //Parameters 
        double tempo_ = 0.0;

        void output_callback(){

            sensor_msgs::msg::Imu imu_;
            imu_.orientation_covariance [0] = 0.01;
            imu_.orientation_covariance [4] = 0.01;
            imu_.orientation_covariance [7] = 0.02;

            double frequencia_imu = 0.3;
            double amplitude_imu = this->get_parameter("amplitude_imu").as_double();

            imu_.angular_velocity.z = amplitude_imu * sin(frequencia_imu * tempo_);
            

            nav_msgs::msg::Odometry odometry_;
            double amplitude_odom = 0.6;
            double frequencia_odom = 0.3;
            double fase_odom = 0.2;

            odometry_.twist.twist.angular.z = amplitude_odom * sin((frequencia_odom * tempo_) + fase_odom);

            odometry_.twist.covariance[35] = 0.05;

            tempo_ += 1.0;

            RCLCPP_INFO(
                this->get_logger(), 
                "sigma (x,y,z) = ('%f', '%f', '%f') | f_imu = '%f' | amp_imu = '%f' | Wz = '%f' | amp_odom = '%f' | f_odom = '%f' | fase_odom = '%f' | Wz_odom = '%f' | sigma_z_odom = '%f'", 
                imu_.orientation_covariance [0],
                imu_.orientation_covariance [4],
                imu_.orientation_covariance [7],
                frequencia_imu,
                amplitude_imu,
                imu_.angular_velocity.z, 
                amplitude_odom,
                frequencia_odom,
                fase_odom,
                odometry_.twist.twist.angular.z,
                odometry_.twist.covariance[35]);
            
            publisher_imu->publish(imu_);
            publisher_odom->publish(odometry_);
        }
    
    public:

        tutoria3_node1() : Node("tutoria3_node1") {

            this->declare_parameter<double>("amplitude_imu", 0.6);

            publisher_imu = this->create_publisher<sensor_msgs::msg::Imu>("odom", 10); 
            publisher_odom = this->create_publisher<nav_msgs::msg::Odometry>("imu", 10);

            timer_ = this->create_wall_timer(
                500ms, 
                std::bind(&tutoria3_node1::output_callback, this)
            );
        }
};

int main(int argc, char * argv[]){

    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<tutoria3_node1>());
    rclcpp::shutdown();

    return 0;
}
