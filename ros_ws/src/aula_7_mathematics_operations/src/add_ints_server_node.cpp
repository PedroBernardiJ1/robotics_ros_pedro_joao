#include <memory>

#include "rclcpp/rclcpp.hpp"
#include "custom_interfaces/srv/add_two_ints.hpp"

using namespace std::placeholders;

class MathematicsOperations : public rclcpp::Node{

    private:

        rclcpp::Service<custom_interfaces::srv::AddTwoInts>::SharedPtr math_operations_server;

        void msg_server_callback(const std::shared_ptr<custom_interfaces::srv::AddTwoInts::Request> request,
                                std::shared_ptr<custom_interfaces::srv::AddTwoInts::Response> response){

            response->sum = request->a + request->b;
            RCLCPP_INFO(this->get_logger(), "Requisição de soma: a = '%ld' e b = '%ld'", request->a, request->b); // '%ld' --> long decimal, int64 bits
            RCLCPP_INFO(this->get_logger(), "Resposta da Soma: '%ld'", response->sum);
        }

    public:

        MathematicsOperations(): Node("add_ints_server_node"){
            math_operations_server = this->create_service<custom_interfaces::srv::AddTwoInts>(
            "add_two_ints",
            std::bind(&MathematicsOperations::msg_server_callback, this,_1, _2)
            );
        }
};

int main(int argc, char * argv[]) {

    rclcpp::init(argc, argv);
    RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "Iniciando o servidor, esquerando requisição. \n");
    rclcpp::spin(std::make_shared<MathematicsOperations>());
    RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "Servidor finalizado. \n");

    rclcpp::shutdown();
    return 0;
}
