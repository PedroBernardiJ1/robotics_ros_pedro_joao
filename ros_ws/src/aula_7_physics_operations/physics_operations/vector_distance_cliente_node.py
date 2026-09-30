import sys

import rclpy
from geometry_msgs.msg import Vector3
from rclpy.node import Node

from custom_interfaces.srv import VectorDistance

class VectorDistanceClient(Node):

    def __init__(self):
        super().__init__('vector_distance_cliente_node')
        self.cli = self.create_client(VectorDistance, 'vector_distance')

        while not self.cli.wait_for_service(timeout_sec=1.0):
            self.get_logger().info('Servidor não disponível, aguardando ...')
        self.request = VectorDistance.Request()

    def send_request(self,
        ax: float,
        ay: float,
        az: float,
        bx: float,
        by: float,
        bz:float) -> VectorDistance.Response:

        self.request.vector_a = Vector3(x=ax, y=ay, z=az)
        self.request.vector_b = Vector3(x=bx, y=by, z=bz)
        self.future = self.cli.call_async(self.request)

        rclpy.spin_until_future_complete(self, self.future)

        return self.future.result()

def main(args=None):

    rclpy.init()

    if len(sys.argv) != 7:
        print('Uso do cliente: ros2 run <pacote> <executavel> ax ay az bx by bz')
        return

    ax, ay, az, bx, by, bz = (float(v) for v in sys.argv[1:7])

    node = VectorDistanceClient()

    response = node.send_request(ax, ay, az, bx, by, bz)

    node.get_logger().info(f'Diferenca, {list(response.difference)}, modulo {response.magnitude:.2f}')

    node.destroy_node()
    rclpy.shutdown()

if __name__ == '__main__':
    main()
