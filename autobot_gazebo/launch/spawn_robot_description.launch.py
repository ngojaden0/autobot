import random
from launch_ros.actions import Node
from launch import LaunchDescription
import os
from ament_index_python.packages import get_package_share_directory
from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument
from launch.actions import IncludeLaunchDescription
from launch.launch_description_sources import PythonLaunchDescriptionSource
from ament_index_python.packages import get_package_prefix
from launch.conditions import IfCondition
from launch.substitutions import LaunchConfiguration

def generate_launch_description():
    position = [0.0, 0.0, 0.055]
    orientation = [0.0, 0.0, 0.0]
    robot_base_name = 'autobot'
    pkg_autobot_description = get_package_share_directory('autobot_description')

    entity_name = robot_base_name+str(int(random.random()*1000))

    spawn_robot = Node(
        package='gazebo_ros',
        executable='spawn_entity.py',
        name='spawn_entity',
        output='screen',
        arguments=['-entity',entity_name,'-x',str(position[0]),'-y',str(position[1]),'-z',str(position[2])
                    ,'-R',str(orientation[0]),'-P',str(orientation[1]),'-Y',str(orientation[2])
                    ,'-topic','/robot_description']
    )

    tf_lidar_base = Node(
        package='tf2_ros',
        executable='static_transform_publisher',
        name='tf_lidar_base',
        output='screen',
        arguments=["0.0","0.0","0.0","0.0",
                    "0.0","0.0","base_link","lidar_link"]
    )

    rviz = Node(
        package='rviz2',
        executable='rviz2',
        arguments=['-d', os.path.join(pkg_autobot_description, 'rviz', 'autobot.rviz')],
    )

    encoder_information = Node(
        package="autobot_gazebo",
        executable="encoder_information",
        name="encoder_information"
    )

    return LaunchDescription(
        [
            spawn_robot,
            tf_lidar_base,
            rviz,
            encoder_information
        ]
    )