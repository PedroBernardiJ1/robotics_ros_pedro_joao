import rclpy
from rclpy.node import Node
from std_msgs.msg import String
from robot_interfaces.msg import Obstacle, Obstacles # Adicionado na Aula 6

class FirstNode(Node):
    def __init__(self):
        super().__init__("first_node")

        self.get_logger().info("first_node started!")

        self.text_publisher_ = self.create_publisher(Obstacles, "/obstacles", 10) # Modificado Aula 6, antes era "(String, "/text", 10)"

        self.timer_publisher_ = self.create_timer(1.0, self.timer_callback)

    def timer_callback(self):
        obs = Obstacle()                # Adicionado na Aula 6
        obs.name = "obs1"               # Adicionado na Aula 6
        obs.pose.position.x = 10.0      # Adicionado na Aula 6

        obs2 = Obstacle()               # Adicionado na Aula 6
        obs2.name = "obs2"              # Adicionado na Aula 6
        obs2.pose.position.x = -10.0    # Adicionado na Aula 6

        msg = Obstacles()               # Modificado na Aula 6, antes era "msg = String()"
        msg.obs.append(obs)             # Modificado na Aula 6, antes era "msg.data = "Message in /text topic""
        msg.obs.append(obs2)            # Adicionado na Aula 6

        self.text_publisher_.publish(msg)
        self.get_logger().info("Message sent")

def main():

    rclpy.init()

    node = FirstNode()
    rclpy.spin(node)

    node.destroy_node()
    rclpy.shutdown()

if __name__ == "__main__":
    main()
