#include <memory>
#include <chrono>
#include <functional>
#include <cmath>

#include "rclcpp/rclcpp.hpp"
#include "geometry_msgs/msg/vector3.hpp"
#include "geometry_msgs/msg/pose.hpp"
#include "geometry_msgs/msg/twist_stamped.hpp"

using namespace std::chrono_literals;
using std::placeholders::_1;

class Projeto1_cpp : public rclcpp::Node{

    private:

        // Subscriber at the topic 'goal' and 'robot_position'
        rclcpp::Subscription<geometry_msgs::msg::Vector3>::SharedPtr goal_subscription;
        rclcpp::Subscription<geometry_msgs::msg::Pose>::SharedPtr robot_position_subscription;

        // Publisher on the topic 'cmd_vel'
        rclcpp::Publisher<geometry_msgs::msg::TwistStamped>::SharedPtr publisher_cmd_vel;  

        // Timer
        rclcpp::TimerBase::SharedPtr timer_;
        
        // Parameters 
        double goal_x_, goal_y_, goal_z_,
                x_i_, y_i_, z_i_,
                orientation_x_, orientation_y_, orientation_z_, orientation_w_;
        
        bool has_goal_ = false;

        // Recebendo parâmetros do 'goal'
        void goal_callback(const geometry_msgs::msg::Vector3::SharedPtr goal){
            
            // Avisando que o robô recebeu um 'goal' válido (diferente de (0,0,0))
            has_goal_ = true;

            // Salvando valores publicados no 'goal'
            goal_x_ = goal->x;
            goal_y_ = goal->y;
            goal_z_ = goal->z;
        };

        // Recebendo parâmetros do 'pose' (pose atual e orientação)
        void pose_callback(const geometry_msgs::msg::Pose::SharedPtr robot_position){
            x_i_ = robot_position->position.x;
            y_i_ = robot_position->position.y;
            z_i_ = robot_position->position.z;

            orientation_x_ = robot_position->orientation.x;
            orientation_y_ = robot_position->orientation.y;
            orientation_z_ = robot_position->orientation.z;
            orientation_w_ = robot_position->orientation.w;
        };

        void output_callback(){
            
            // Avalia logo no início do callback se o 'goal' recebido é um ponto válido
            if (!has_goal_) {

                geometry_msgs::msg::TwistStamped mensagem_parado;
                mensagem_parado.header.stamp = this->get_clock()->now();
                mensagem_parado.header.frame_id = "base_link";
                
                // Zerando todos os valores
                mensagem_parado.twist.linear.y = 0.0;
                mensagem_parado.twist.linear.z = 0.0;
                mensagem_parado.twist.angular.x = 0.0;
                mensagem_parado.twist.angular.y = 0.0;
                mensagem_parado.twist.linear.x = 0.0;
                mensagem_parado.twist.angular.z = 0.0;
        
                // Publica no 'cmd_vel'
                publisher_cmd_vel->publish(mensagem_parado);

                // Printa no terminal o estado da avaliação e comandos de V e W
                RCLCPP_INFO (
                    this->get_logger(), 
                    "Aguardando objeto válido no tópico 'goal'  ---> Comando V: 0 m/s | Comando W: 0 rad/s"
                );
        
                return; // Código é abortado aqui, caso não tenha sido recebido o 'goal'
            };
            
            // Definindo V e W iniciais
            double v = 0.0;
            double w = 0.0;

            // Calculando ângulo de orientação atual
            double theta_y = 2 * ((orientation_w_ * orientation_z_) + (orientation_x_ * orientation_y_)); // Componente no eixo Y (seno)
            double theta_x = 1 - (2 * ((pow(orientation_y_, 2)) + pow(orientation_z_, 2))); // Componente no eixo X (cosseno)

            double theta = atan2(theta_y, theta_x); // Ângulo calculado (em rad)

            // Por se tratar de um problema em 2D, é necessário calcular apenas o componente de rotação em Z (yaw), dado que o robô não sairá do
            // chão e, portanto, não realizará rotações em X e Y

            // Calculando distância até o objetivo
            double distance_x_to_goal_ = goal_x_ - x_i_; // Componente de distância em X (delta X)
            double distance_y_to_goal_ = goal_y_ - y_i_; // Componente de distância em Y (delta Y)

            double distance_to_goal_ = sqrt(pow(distance_x_to_goal_, 2) + pow(distance_y_to_goal_, 2)); // Distância calculada
            
            // Definindo as tolerâncias para W e V
            double w_tol = 0.08; // tolerância de aproximadamente 5 graus para a orientação
            double d_tol = 0.05; // Tolerância de 5 centímetros para a distância final

            // Definindo o 'theta_to_goal' e seu tratamento condicional
            double theta_to_goal_;

            if (distance_to_goal_ > d_tol) { // O robô ainda não chegou ao raio de tolerância do destino

                // Calculando a orientação em linha reta até o objetivo final
                theta_to_goal_ = atan2(distance_y_to_goal_, distance_x_to_goal_);

            } else { // Robô já chegou ao destino (raio da tolerância)

                // Giro para alinhar o ângulo final exigido
                theta_to_goal_ = goal_z_;

            }

            // Calculando o erro de orientação ('orientação do objetivo' - 'orientação atual')
            double theta_error = theta_to_goal_ - theta; // em radianos
            theta_error = atan2(sin(theta_error), cos(theta_error)); // normalizando

            // Definindo as constantes do ganho proporcional
            double K_w {1.2}; // definindo o ganho como 1.2 para W
            double K_v {0.5}; // definindo o ganho como 0.5 para V
            
            // Aplica a tolerância do giro
            if (abs(theta_error) < w_tol) {

                w = 0.0; // Para de tentar alinhar o ângulo
                
            } else {

                // Aplica o controle proporcional em W angular
                w = K_w * theta_error;
                
                if (w > 1.5) {

                    w = 1.5; // Definindo 1.5 rad/s como o limite superior de velocidade angular

                } else if (w < -1.5) {

                    w = -1.5; // Definindo -1.5 rad/s como o limite inferior de velocidade angular

                };
            };
                
            // Aplica a tolerância da distância
            if (distance_to_goal_< d_tol) {

                v = 0.0; // Estaciona completamente
                
            } else {

                // Aplica o controle proporcional em V linear
                v = K_v * distance_to_goal_;

                if (v > 5) {

                    v = 5; // Definindo 5 m/s como o limite de velocidade linear

                };
            };

            // Criando o Twist Stamped
            geometry_msgs::msg::TwistStamped mensagem_movimento;

            mensagem_movimento.header.stamp = this->get_clock()->now(); // Grava o tempo exato da execução
            mensagem_movimento.header.frame_id = "base_link"; // Define o ponto de referência físico que recebe a ordem de V e W

            // Zerando os valores que não foram usados
            mensagem_movimento.twist.linear.y = 0.0;
            mensagem_movimento.twist.linear.z = 0.0;
            mensagem_movimento.twist.angular.x = 0.0;
            mensagem_movimento.twist.angular.y = 0.0;

            // Valores calculados
            mensagem_movimento.twist.linear.x = v; // Vai para frente
            mensagem_movimento.twist.angular.z = w; // Vira para a direita

            // Printa no terminal o estado da avaliação e comandos de V e W
            RCLCPP_INFO (
                this->get_logger(), 
                "Distância: '%f' m | Erro Angular: '%f' rad ---> Comando V: '%f' | Comando W: '%f'", 
                distance_to_goal_,
                theta_error,
                v, w
            );

            // Publica no 'cmd_vel'
            publisher_cmd_vel->publish(mensagem_movimento);
        };
    
    public:

        Projeto1_cpp() : Node("projeto1_publisher_subscriber_cpp") {

            // Criando o subscriber no tópico 'goal', que recebe o tipo Vector3
            goal_subscription = this->create_subscription<geometry_msgs::msg::Vector3>(
                "goal", 10,
                std::bind(&Projeto1_cpp::goal_callback, this, _1)
            );

            // Criando o subscriber no tópico 'robot_position', que recebe o tipo Pose
            robot_position_subscription = this->create_subscription<geometry_msgs::msg::Pose>(
                "robot_position", 10,
                std::bind(&Projeto1_cpp::pose_callback, this, _1)
            );

            // Criando o publisher no tópico 'cmd_vel', do tipo Twist Stamped
            publisher_cmd_vel = this->create_publisher<geometry_msgs::msg::TwistStamped>("cmd_vel", 10);

            // Criando o timer do publisher
            timer_ = this->create_wall_timer(

                100ms, // Definindo o timer para atuar em 10 Hz (a cada 100 ms)
                std::bind(&Projeto1_cpp::output_callback, this)

            );
        };
};

int main(int argc, char * argv[]){

    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<Projeto1_cpp>());
    rclcpp::shutdown();

    return 0;
}
