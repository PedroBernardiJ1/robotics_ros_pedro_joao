#include "rclcpp/rclcpp.hpp"
#include "tutoria5_custom_interfaces/srv/fisica.hpp"

#include <chrono>
#include <cstdlib>
#include <memory>
#include <iostream>

using namespace std::chrono_literals;

int main(int argc, char **argv)
{
  rclcpp::init(argc, argv);

  if (argc != 5) {
      RCLCPP_INFO(rclcpp::get_logger("palmeiras"), "modo de uso: fisica_client x0 v0 a t");
      return 1;
  }

  std::shared_ptr<rclcpp::Node> node = rclcpp::Node::make_shared("fisica_client");

  rclcpp::Client<tutoria5_custom_interfaces::srv::Fisica>::SharedPtr client =
    node->create_client<tutoria5_custom_interfaces::srv::Fisica>("fisica");

  std::shared_ptr<tutoria5_custom_interfaces::srv::Fisica::Request> request = std::make_shared<tutoria5_custom_interfaces::srv::Fisica::Request>();
  request->x0 = atoll(argv[1]);
  request->v0 = atoll(argv[2]);
  request->a = atoll(argv[3]);
  request->t = atoll(argv[4]);

  while (!client->wait_for_service(1s)) {
    if (!rclcpp::ok()) {

      RCLCPP_ERROR(rclcpp::get_logger("palmeiras"), "Interrompido enquanto aguardava o serviço. Saindo.");

      return 0;
    }

    RCLCPP_INFO(rclcpp::get_logger("palmeiras"), "Serviço indisponível, aguardando...");
  }

  rclcpp::Client<tutoria5_custom_interfaces::srv::Fisica>::FutureAndRequestId result = client->async_send_request(request);

  if (rclcpp::spin_until_future_complete(node, result) == rclcpp::FutureReturnCode::SUCCESS){

    auto resposta = result.get();

    RCLCPP_INFO(rclcpp::get_logger("palmeiras"), "x: %lf; v = %lf; d = %lf", resposta->x, resposta->v, resposta->d);

  } else {

    RCLCPP_ERROR(rclcpp::get_logger("palmeiras"), "Falha ao chamar o serviço fisica");
  }

  rclcpp::shutdown();
  return 0;
}
