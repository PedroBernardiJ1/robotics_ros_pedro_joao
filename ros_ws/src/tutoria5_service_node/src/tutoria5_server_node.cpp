#include "rclcpp/rclcpp.hpp"
#include "tutoria5_custom_interfaces/srv/fisica.hpp"

#include <memory>

using namespace std::placeholders;

class Tutoria5 : public rclcpp::Node {

    private:

        rclcpp::Service<tutoria5_custom_interfaces::srv::Fisica>::SharedPtr service;

        void equacoes(const std::shared_ptr<tutoria5_custom_interfaces::srv::Fisica::Request> request,
                    std::shared_ptr<tutoria5_custom_interfaces::srv::Fisica::Response> response){

            response->x = request->x0 + request->v0 * request->t + (1.0/2.0) * (request->a/request->t);
            response->v = request->v0 + request->a * request->t;
            response->d = response->x - request->x0;
            RCLCPP_INFO(this->get_logger(), "Requisição de valores: x0 = '%lf'; v0 = '%lf'; a = '%lf'; t = '%lf'", request->x0, request->v0, request->a, request->t);
            RCLCPP_INFO(this->get_logger(), "Respostas: x = '%lf'; v = '%lf'; d = '%lf'", response->x, response->v, response->d);
        };

    public:

        Tutoria5() : Node("fisica_server"){
            service = this->create_service<tutoria5_custom_interfaces::srv::Fisica>("fisica",
                std::bind(&Tutoria5::equacoes, this, _1, _2
            ));
        }
};

int main (int argc, char **argv){

    rclcpp::init(argc, argv);
    RCLCPP_INFO(rclcpp::get_logger("palmeiras"), "Servidor iniciado, aguardando x0, v0, a, t. \n");

    rclcpp::spin(std::make_shared<Tutoria5>());
    RCLCPP_INFO(rclcpp::get_logger("palmeiras"), "Servidor finalizado \n");

    rclcpp::shutdown();
    return 0;
}
