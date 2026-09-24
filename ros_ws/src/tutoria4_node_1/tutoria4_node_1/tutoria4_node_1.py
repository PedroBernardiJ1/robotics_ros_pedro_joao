import rclpy
import math
from rclpy.node import Node
from tutoria4_custom_interfaces.msg import Velocidade

class tutoria4_node_1 (Node):

    def __init__(self):
        super().__init__("tutoria4_node_1")

        self.v0_ = 1.0
        self.tempo_ = 0.0
        
        # Publisher
        self.velocidade_publisher_ = self.create_publisher(Velocidade, "velocidade", 10)

        # Timers
        self.timer_publisher_ = self.create_timer(1.0, self.publisher_velocidade)    

    def publisher_velocidade(self):

        gravidade = 9.81
        theta = 10.0
        
        # Instanciando mensagem do tipo Velocidade e calculando seus valores
        vel = Velocidade()
        vel.vx = self.v0_ * math.cos(theta)
        vel.vy = self.v0_ * math.sin(theta) - gravidade * self.tempo_
        vel.t = self.tempo_

        # Incrementando tempo
        self.tempo_ += 1.0

        # Publicando
        self.velocidade_publisher_.publish(vel)

        self.get_logger().info(f"Tempo: {vel.t} | Velocidade: {vel.vx}, {vel.vy}")
        
def main():

    rclpy.init()

    node = tutoria4_node_1()
    rclpy.spin(node)

    node.destroy_node()
    rclpy.shutdown()

if __name__ == "__main__":
    main()
