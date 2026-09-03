import rclpy
import math
from rclpy.node import Node
from std_msgs.msg import Float64
from geometry_msgs.msg import Vector3

class Node_python (Node):

    def __init__(self):
        super().__init__("tutoria2_node_2")

        self.distance = 0.0
            
        self.x_orientation = 0.0
        self.y_orientation = 0.0
        self.z_orientation = 0.0
        
        # Subscriber
        self.area_subscriber_ = self.create_subscription(Float64, "/distance", self.subscriber_distance, 10)
        self.area_subscriber_ = self.create_subscription(Vector3, "/orientation", self.subscriber_orientation, 10)

        # Publisher
        self.point_out_publisher_ = self.create_publisher(Vector3, "/point_out", 10)

        # Timers
        self.timer_publisher_ = self.create_timer(1.0, self.publisher_point_out)

    def subscriber_distance(self, distance: Float64):

        # Salvamento da distância como atributo
        self.distance = distance.data

    def subscriber_orientation(self, orientation: Vector3):
    
            # Salvamento de cada valor da orientação e conversão de volta para radianos
            self.x_orientation = orientation.x * (math.pi/180)
            self.y_orientation = orientation.y * (math.pi/180)
            self.z_orientation = orientation.z * (math.pi/180)     

    def publisher_point_out(self):

        # Calculando os x, Y e Z do point_out
        x_pos = (self.distance) * (math.cos(self.y_orientation)) * (math.cos(self.x_orientation))
        y_pos = (self.distance) * (math.cos(self.y_orientation)) * (math.sin(self.x_orientation))
        z_pos = (self.distance) * (math.sin(self.y_orientation))
            
        # Criando o Vector point_out e definindo seus valores
        point_out_ = Vector3()
        point_out_.x = x_pos
        point_out_.y = y_pos
        point_out_.z = z_pos

        # Publicando
        self.point_out_publisher_.publish(point_out_)
        self.get_logger().info(f"Ponto (X = {point_out_.x}, Y = {point_out_.y}, Z = {point_out_.z})")
        
def main():

    rclpy.init()

    node = Node_python()
    rclpy.spin(node)

    node.destroy_node()
    rclpy.shutdown()

if __name__ == "__main__":
    main()
