import rclpy
from rclpy.node import Node
from sensor_msgs.msg import Imu
from nav_msgs.msg import Odometry

class Node_python (Node):

    def __init__(self):
        super().__init__("tutoria3_node_2")

        self.imu_covariance_z = 1.0
        self.odometry_covariance_z = 1.0
        
        # Subscriber
        self.imu_subscriber_ = self.create_subscription(Imu, "imu", self.subscriber_imu, 10)
        self.odometry_subscriber_ = self.create_subscription(Odometry, "odom", self.subscriber_odometry, 10)

        # Publisher
        self.imu_fundido_publisher_ = self.create_publisher(Imu, "imu_fundido", 10)

        # Timers
        self.timer_publisher_ = self.create_timer(1.0, self.publisher_imu_fundido)

    def subscriber_imu(self, imu: Imu):

        # Salvamento da covariância do IMU como atributo
        self.imu_covariance_z = imu.orientation_covariance[7]

    def subscriber_odometry(self, odometry: Odometry):
    
        # Salvamento da covariância da Odometria como atributo
        self.odometry_covariance_z = odometry.twist.twist[35]     

    def publisher_imu_fundido(self):

        # Calculando os pesos do IMU e da Odometria
        imu_peso = 1 / (self.imu_covariance_z)
        odom_peso = 1 / (self.odometry_covariance_z)
            
        # Calculando Sigma Z do IMU Fundido
        imu_fundido_covariance_z = 1 / (imu_peso + odom_peso)

        # Criando o IMU imu_fundido e definindo seus valores
        imu_fundido_ = Imu()

        # Covariância em Z
        imu_fundido_.orientation_covariance [7] = imu_fundido_covariance_z

        # Velocidade Angular em Z
        imu_fundido_.angular_velocity.z = ((self.imu_covariance_z * imu_peso) + (self.odometry_covariance_z * odom_peso)) / (imu_peso + odom_peso)

        # Publicando
        self.imu_fundido_publisher_.publish(imu_fundido_)

        self.get_logger().info(f"peso_imu = {imu_peso}, peso_odom = {odom_peso}, sigma_z = {imu_fundido_covariance_z}, Wz = {imu_fundido_.angular_velocity.z}")
        
def main():

    rclpy.init()

    node = Node_python()
    rclpy.spin(node)

    node.destroy_node()
    rclpy.shutdown()

if __name__ == "__main__":
    main()
