import rclpy
from rclpy.node import Node
from std_msgs.msg import String
from std_msgs.msg import Int64

class SubscriberNode (Node):
    def __init__(self):
        super().__init__("subscriber_node")

        self.get_logger().info("subscriber_node started!")

        # Subscribers
        self.text_subscriber = self.create_subscription(String, "/text", self.subscriber_callback, 10)

        # Publishers
        self.count_publisher_ = self.create_publisher(Int64, "/text_count", 10)
        self.count_min_publisher_ = self.create_publisher(Int64, "/text_min_count", 10)

        # Timer
        self.one_min_timer = self.create_timer(60, self.one_min_callback)

        # Variables
        self.count: int = 0

    def subscriber_callback(self, msg: String):
        # Feedback de mensagem
        self.get_logger().info(f"Receive: {msg.data}")
        self.count += 1

        # Publicar sempre que receber uma nova mensagem
        msg_count = Int64()
        msg_count.data = self.count

        self.count_publisher_.publish(msg_count)

    def one_min_callback(self):
        
        # Publicar sempre que receber uma nova mensagem
        msg_count = Int64()
        msg_count.data = self.count
        self.count_min_publisher_.publish(msg_count)

        
def main():

    rclpy.init()

    node = SubscriberNode()
    rclpy.spin(node)

    node.destroy_node()
    rclpy.shutdown()

if __name__ == "__main__":
    main()
