import rclpy
from rclpy.node import Node
from std_msgs.msg import Float64

class PythonTutoriaNode (Node):
    def __init__(self):
        super().__init__("python_tutoria_node")

        self.msg_vol = 0.0 # Altura do cilindro

        # Subscriber
        self.area_subscriber_ = self.create_subscription(Float64, "/publisher_node_area", self.subscriber_callback, 10)

        # Publisher
        self.volume_publisher_ = self.create_publisher(Float64, "/publisher_volume", 10)
        self.timer_publisher_ = self.create_timer(1.0, self.publisher_callback)

    def subscriber_callback(self, msg: Float64):

        # Salvamento da área como atributo
        self.msg_vol = msg.data

    def publisher_callback(self):

            # Definindo a Altura
            height = 10.0

            # Calculando o volume
            msg1 = Float64()
            msg1.data = self.msg_vol * height

            # Publicando
            self.volume_publisher_.publish(msg1)
            # self.get_logger().info("")
        
def main():

    rclpy.init()

    node = PythonTutoriaNode()
    rclpy.spin(node)

    node.destroy_node()
    rclpy.shutdown()

if __name__ == "__main__":
    main()
