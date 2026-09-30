#include <memory>
#include <cstdlib>

#include "rclcpp/rclcpp.hpp"
#include "custom_interfaces/srv/add_two_ints.hpp"

using namespace std::chrono_literals;

int main(int argc, char * argv[])
{
    rclcpp::init(argc, argv);


    if(argc != 3){

        RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "Uso do cliente: add_two_ints_client X Y");

        return 1;
    }

    std::shared_ptr<rclcpp::Node> client_node = rclcpp::Node::make_shared("add_ints_client_node");

    rclcpp::Client<custom_interfaces::srv::AddTwoInts>::SharedPtr math_operations_client =
    client_node->create_client<custom_interfaces::srv::AddTwoInts>("add_two_ints");

    std::shared_ptr<custom_interfaces::srv::AddTwoInts::Request> request =
    std::make_shared<custom_interfaces::srv::AddTwoInts::Request>();

    request->a= atoll(argv[1]);
    request->b= atoll(argv[2]);

    while(!math_operations_client->wait_for_service(1s)){

        if(!rclcpp::ok()){
            RCLCPP_ERROR(rclcpp::get_logger("rclcpp"), "Interrompido enquanto esperava o serviço");
            return 0;
        }

        RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "Servico não disponivel, aguardando...");
    }

    rclcpp::Client<custom_interfaces::srv::AddTwoInts>::FutureAndRequestId result =
    math_operations_client->async_send_request(request);

    if(rclcpp::spin_until_future_complete(client_node, result) ==
    rclcpp::FutureReturnCode::SUCCESS){

        RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "Sum: %ld",result.get()->sum);

    } else {

        RCLCPP_ERROR(rclcpp::get_logger("rclcpp"), "Falha ao chamar o servico add_two_ints");

    }

    rclcpp::shutdown();
    return 0;
}
