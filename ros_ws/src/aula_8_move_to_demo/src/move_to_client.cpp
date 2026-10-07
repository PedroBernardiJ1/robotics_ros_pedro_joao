// Bibliotecas do Sistema
#include <chrono>
#include <functional>
#include <memory>

// Bibliotecas do ROS
#include "rclcpp/rclcpp.hpp"
#include "rclcpp_action/rclcpp_action.hpp"

// Bibliotecas Próprias/Customizadas
#include "aula_8_robot_interfaces/action/move_to.hpp"

// Namespaces
using namespace std::chrono_literals;                                                   // Gerir o timer/tempo
using std::placeholders::_1;                                                            // Necessário para uso do Bind
using std::placeholders::_2;                                                            // Necessário para uso do Bind

class MoveToClient : public rclcpp::Node{                                               // Criação da Classe

    public:

        using MoveTo = aula_8_robot_interfaces::action::MoveTo;                                // Instanciansse tipo
        using GoalHandle = rclcpp_action::ClientGoalHandle<MoveTo>;                     // Instanciansse tipo

        MoveToClient(): Node("move_to_client") {                                        // Construtora da Classe, herdando a construtora de Node

            client_ = rclcpp_action::create_client<MoveTo>(                             // Criação do Client
                this,                                                                   // Nó que hospeda o action client
                "move_to"                                                               // Nome da action, o mesmo usado pelo servidor
            );

            // O goal enviado uma unica vez, logo depois que o nó sobe
            timer_ = this->create_wall_timer(
                500ms,                                                          // Intervalo entre pulsos do Timer (aqui precisamos do Chrono_Literals)
                std::bind(&MoveToClient::send_goal, this)             // Callback do 'send_goal'
            );
        }

    private:

        // Parâmetros de Classe
        rclcpp_action::Client<MoveTo>::SharedPtr client_;
        rclcpp::TimerBase::SharedPtr timer_;

        // Função para enviar o objetivo
        void send_goal() {

            timer_->cancel();                                                           // Garante que o goal seja enviado só uma vez

            if (!client_->wait_for_action_server(5s)) {                         // Aborta caso o server demore para responder / ficar ativo

                RCLCPP_ERROR(this->get_logger(), "Action server não disponível");       // Server demorou para responder (mais de 5 segundos)
                rclcpp::shutdown();

                return;
            }

            auto goal = MoveTo::Goal();
            goal.goal_pose.position.x = 5.0;                                            // Definindo valores para o objetivo
            goal.goal_pose.position.y = 3.0;                                            // Definindo valores para o objetivo
            goal.goal_pose.position.z = 0.0;                                            // Definindo valores para o objetivo

            goal.goal_pose.orientation.w = 1.0;                                         // Quaternion identidade
            goal.tolerance = 0.1;                                                       // Tolerância

            // Callbacks passados dentro de uma struct 'opções' --> callbacks do serviço estão sendo linkados aos do Cliente
            auto options = rclcpp_action::Client<MoveTo>::SendGoalOptions();

            options.goal_response_callback = std::bind(&MoveToClient::goal_response_callback, this, _1);    // Callback da Resposta do servidor --> note que o número de placholders é o número de argumentos que são recebidos
            options.feedback_callback = std::bind(&MoveToClient::feedback_callback, this, _1, _2);          // Callback do Feedback do servidor --> note que o número de placholders é o número de argumentos que são recebidos
            options.result_callback = std::bind(&MoveToClient::result_callback, this, _1);                  // Callback do Result do servidor --> note que o número de placholders é o número de argumentos que são recebidos

            RCLCPP_INFO(                                                                // Printagem no Terminal
                    this->get_logger(),
                    "Enviando goal: x: %.2f, y: %.2f",
                    goal.goal_pose.position.x,
                    goal.goal_pose.position.y
                );

            client_->async_send_goal(goal, options);                                    // Envia o Goal pro Servidor
        }

        // Chamado quando o servidor aceita ou recusa a goal
        void goal_response_callback(const GoalHandle::SharedPtr & goal_handle) {        // Note um único argumento sendo passado (placeholder anterior)

            if (!goal_handle) {                                                         // Condição de Desligamento caso o Servidor recuse o Goal

                RCLCPP_ERROR(this->get_logger(), "Goal recusada pelo servidor");
                rclcpp::shutdown();

                return;
            }

            RCLCPP_INFO(this->get_logger(), "Goal aceita, aguardando resultado...");    // Printagem no terminal
        }

        // Chamado a cada mensagem de feedback publicada pelo servidor
        void feedback_callback(
                            GoalHandle::SharedPtr,
                            const std::shared_ptr<const MoveTo::Feedback> feedback      // Shared_Ptr do Feedcback
                            ) {

                const auto & position = feedback->current_pose.position;                // Desreferenciação de Ponteiro

                RCLCPP_INFO(                                                            // Printagem no terminal
                    this->get_logger(),
                    "Faltam %.2f m (agora em x: %.2f, y: %.2f)",
                    feedback->distance_remaining,
                    position.x, position.y
                );
            }

        // Chamado uma unica vez, quando a goal chega a um estado final
        void result_callback(const GoalHandle::WrappedResult & result) {

            switch (result.code) {                                                      // Avalia como foi o Result e faz a printagem no Terminal, cada uma para um resultado

                case rclcpp_action::ResultCode::SUCCEEDED:

                    RCLCPP_INFO(
                        this->get_logger(),
                        "Chegamos! Erro final: %.3f m em %.1f s",
                        result.result->final_distance,
                        result.result->elapsed_time
                    );

                    break;

                case rclcpp_action::ResultCode::ABORTED:

                    RCLCPP_ERROR(this->get_logger(), "Goal abortada pelo servidor");

                    break;

                case rclcpp_action::ResultCode::CANCELED:

                    RCLCPP_WARN(this->get_logger(), "Goal cancelada");
                    break;

                default:
                    RCLCPP_ERROR(this->get_logger(), "Resultado desconhecido");
                    break;
            }

            rclcpp::shutdown();
        }
};

int main(int argc, char ** argv) {                                                      // Main padrão de sempre

    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<MoveToClient>());
    rclcpp::shutdown();

    return 0;
}
