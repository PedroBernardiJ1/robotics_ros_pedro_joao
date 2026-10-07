// NESSE NÓ VEMOS COMO A DOCUMENTAÇÃO DO ROS MOSTRA A CRIAÇÃO DO SERVIÇO, SEM USO DE CLASSES
// NO CASO DO SERVER, É FÁCIL ADAPTAR PARA A CRIAÇÃO DE UMA CLASSE, COMO FIZEMOS PARA A CRIAÇÃO
// DO CLIENTE, UTILIZAREMOS A MANEIRA MOSTRADA NA DOCUMENTAÇÃO, POIS NÃO EXISTE ADAPTAÇÃO FÁCIL
// PARA IMPLEMENTAR COM CLASSE

#include "rclcpp/rclcpp.hpp"
#include "aula_7_custom_interfaces/srv/add_two_ints.hpp"

#include <memory>

void msg_server_callback(const std::shared_ptr<aula_7_custom_interfaces::srv::AddTwoInts::Request> request,
                               std::shared_ptr<aula_7_custom_interfaces::srv::AddTwoInts::Response> response)
{
    response->sum = request->a + request->b;
    RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "Requisicao de soma: a = '%ld' e b = '%ld'",
                request->a, request->b);
    RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "Resposta da soma: '%ld'", response->sum);
}

int main(int argc, char **argv)
{
    rclcpp::init(argc, argv);

    std::shared_ptr<rclcpp::Node> node = rclcpp::Node::make_shared("add_ints_server_node");

    rclcpp::Service<aula_7_custom_interfaces::srv::AddTwoInts>::SharedPtr math_operations_server =
        node->create_service<aula_7_custom_interfaces::srv::AddTwoInts>("add_two_ints", &msg_server_callback);

    RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "Servico add_two_ints pronto!");

    rclcpp::spin(node);
    rclcpp::shutdown();
}
