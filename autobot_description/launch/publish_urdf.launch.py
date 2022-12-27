import os

from ament_index_python.packages import get_package_share_directory
from launch import LaunchDescription
from launch.substitutions import Command
from launch_ros.actions import Node
from launch.actions import DeclareLaunchArgument, ExecuteProcess

def generate_launch_description():
    urdf_file = 'autobot.urdf'

    package_description = 'autobot_description'
    robot_desc_path = os.path.join(get_package_share_directory(package_description), 'urdf', urdf_file)
    with open(robot_desc_path, 'r') as infp:
        robot_desc = infp.read()

    robot_state_publisher_node = Node(
        package='robot_state_publisher',
        executable='robot_state_publisher',
        name='robot_state_publisher_node',
        parameters=[{'use sim time': True, 'robot_description': Command(['xacro ', robot_desc_path])}],
        output='screen'
    )

    return LaunchDescription(
        [
            robot_state_publisher_node,
        ]
    )