#include <iostream>
#include <memory>
#include <chrono>
#include <cmath>

#include "rclcpp/rclcpp.hpp"
#include "tutoria4_custom_interfaces/msg/velocidade.hpp"
#include "tutoria4_custom_interfaces/msg/diagnostico.hpp"

using namespace std::chrono_literals;
using std::placeholders::_1;

class tutoria4_node_2 : public rclcpp::Node{

    private:
        
        // Subscriber at the topic /velocidade
        rclcpp::Subscription<tutoria4_custom_interfaces::msg::Velocidade>::SharedPtr velocidade_subscription_;
        
        // Publisher on the topic /diagnostico
        rclcpp::Publisher<tutoria4_custom_interfaces::msg::Diagnostico>::SharedPtr publisher_diagnostico;  

        // Timer
        rclcpp::TimerBase::SharedPtr timer_;

        //Parameters 
        double vx_ = 0.0;
        double vy_ = 0.0;
        double t_ = 0.0;
        double ope_ = 0.0;
        int i = 0;

        void point_in_callback(const tutoria4_custom_interfaces::msg::Velocidade vel_){
            vx_ = vel_.vx;
            vy_ = vel_.vy;
            t_ = vel_.t;
        }

        void output_callback(){

            double massa = 10.0;

            tutoria4_custom_interfaces::msg::Diagnostico diagnostico_;
            diagnostico_.energia.x = 0.5 * massa * pow(vy_, 2);
            diagnostico_.energia.y = 0.5 * massa * pow(vy_, 2);
            diagnostico_.energia.z = diagnostico_.energia.x + diagnostico_.energia.y;

            diagnostico_.ang_velocidade = atan2(vy_, vx_);

            ope_ = sqrt(pow(vx_, 2) + pow(vy_, 2));
            diagnostico_.historico_velocidade.push_back(ope_);

            publisher_diagnostico->publish(diagnostico_);

            RCLCPP_INFO(
                this->get_logger(), 
                "Tempo: '%lf' | Energia: '%lf', '%lf', '%lf' | Ângulo Velocidade: '%lf' | Histórico: '%lf'", 
                t_,
                diagnostico_.energia.x,
                diagnostico_.energia.y,
                diagnostico_.energia.z,
                diagnostico_.ang_velocidade,
                diagnostico_.historico_velocidade.back()
            );    
        }
    
    public:

        tutoria4_node_2() : Node("tutoria4_node2") {

            publisher_diagnostico = this->create_publisher<tutoria4_custom_interfaces::msg::Diagnostico>("diagnostico", 10);

            velocidade_subscription_ = this->create_subscription<tutoria4_custom_interfaces::msg::Velocidade>(
                "velocidade", 
                10,
                std::bind(&tutoria4_node_2::point_in_callback, this, _1)
            );

            timer_ = this->create_wall_timer(
                500ms, 
                std::bind(&tutoria4_node_2::output_callback, this)
            );
        }
};

int main(int argc, char * argv[]){

    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<tutoria4_node_2>());
    rclcpp::shutdown();

    return 0;
}