from launch import LaunchDescription
from launch_ros.actions import Node
from launch.actions import DeclareLaunchArgument
from launch.substitutions import LaunchConfiguration

def generate_launch_description():

    amplitude_imu = LaunchConfiguration('amplitude_imu')

    declare_amplitude_imu = DeclareLaunchArgument(
        'amplitude_imu',
        default_value='0.6',
        description='Amplitude do Tomps.'
    )

    tutoria3_node1 = Node(
    package='tutoria3_node1',
    executable='tutoria3_node1',
    output='screen',
    parameters=[{'amplitude_imu': amplitude_imu}]
    )

    tutoria3_node2 = Node(
    package='tutoria3_node2',
    executable='tutoria3_node2',
    output='screen',
    )

    return LaunchDescription([
        tutoria3_node1,
        tutoria3_node2
    ])