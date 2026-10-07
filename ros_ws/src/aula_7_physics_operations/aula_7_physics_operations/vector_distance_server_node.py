import math

import rclpy
from rclpy.node import Node

from aula_7_custom_interfaces.srv import VectorDistance

class VectorDistanceServer(Node):

    def __init__(self):
        super().__init__('vector_distance_server_node')
        self.srv = self.create_service(
        VectorDistance,
        'vector_distance',
        self.msg_server_callback
    )

    def msg_server_callback(self, request: VectorDistance.Request, response: VectorDistance.Response) -> VectorDistance.Response:

        dx = request.vector_a.x - request.vector_b.x
        dy = request.vector_a.y - request.vector_b.y
        dz = request.vector_a.z - request.vector_b.z

        response.difference = [dx, dy, dz]
        response.magnitude = math.sqrt(dx**2 + dy**2 + dz**2)

        self.get_logger().info(
            f'vector_a = ({request.vector_a.x:.2f}, {request.vector_a.y:.2f}, {request.vector_a.z:.2f}), '
            f'vector_b = ({request.vector_b.x:.2f}, {request.vector_b.y:.2f}, {request.vector_b.z:.2f}), '
        )

        self.get_logger().info(
            f'diferenca = [{dx:.2f}, {dy:.2f}, {dz:.2f}], modulo = {response.magnitude:.2f}'
        )

        return response

def main(args=None):

    rclpy.init(args=args)

    node = VectorDistanceServer()

    node.get_logger().info('Servidor iniciado, esperando requisição')
    rclpy.spin(node)

    node.get_logger().info('Servidor Finalizado')
    node.destroy_node()
    rclpy.shutdown()

if __name__ == '__main__':
    main()
