from launch import LaunchDescription
from launch.actions import GroupAction, IncludeLaunchDescription
from launch.actions import DeclareLaunchArgument
from launch.launch_description_sources import PythonLaunchDescriptionSource
from launch_ros.actions import SetRemap
from ament_index_python.packages import get_package_share_directory
import os

def generate_launch_description():
    nav2_pkg = get_package_share_directory('nav2_bringup')
    nav2_launch = os.path.join(nav2_pkg, 'launch', 'navigation_launch.py')

    params_file = os.path.join(
        get_package_share_directory('fra2mo_bringup'),
        'config',
        'copy_nav2_config.yaml'
    )

    return LaunchDescription([
        GroupAction([
            # Remap cmd_vel here
            SetRemap('/cmd_vel', '/fra2mo/cmd_vel'),

            IncludeLaunchDescription(
                PythonLaunchDescriptionSource(nav2_launch),
                launch_arguments={
                    'params_file': params_file
                }.items()
            )
        ])
    ])
