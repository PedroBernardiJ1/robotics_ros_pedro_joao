from launch import LaunchDescription
from launch_ros.actions import Node

def generate_launch_description():

    tutoria4_node_1 = Node(
    package='tutoria4_node_1',
    executable='tutoria4_node_1',
    output='screen'
    )

    tutoria4_node_2 = Node(
    package='tutoria4_node_2',
    executable='tutoria4_node_2',
    output='screen'
    )

    return LaunchDescription([
        tutoria4_node_1,
        tutoria4_node_2
    ])