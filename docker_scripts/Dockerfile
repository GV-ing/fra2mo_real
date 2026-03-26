# Usiamo ros-base invece di desktop: niente GUI, solo il core di ROS2 (molto più leggero!)
FROM ros:humble-ros-base

# Configurazione ambiente base
ENV DEBIAN_FRONTEND=noninteractive \
    LANG=en_US.UTF-8 \
    LC_ALL=en_US.UTF-8 \
    HOME=/root \
    ROS_DISTRO=humble

WORKDIR /root

# Installazione strumenti essenziali, dipendenze hardware/comunicazione e locale UTF-8

RUN apt-get update && apt-get install -y --no-install-recommends \
    python3-serial \
    locales \
    build-essential \
    python3-pip \
    python3-colcon-common-extensions \
    git \
    nano \
    curl \
    iputils-ping \
    libboost-system-dev \
    libboost-thread-dev \
    libboost-program-options-dev \
    ros-humble-rmw-cyclonedds-cpp \
    ros-humble-sensor-msgs \
    ros-humble-geometry-msgs \
    ros-humble-nav-msgs \
    ros-humble-std-msgs \
    ros-humble-tf2 \
    ros-humble-tf2-ros \
    ros-humble-tf2-geometry-msgs \
    ros-humble-xacro \
    ros-humble-realsense2-description \
    ros-humble-joint-state-publisher \
    ros-humble-rclcpp \
    ros-humble-rclpy \
    ros-humble-rosidl-default-generators \
    ros-humble-rosidl-default-runtime \
    libusb-1.0-0-dev \
    libyaml-cpp-dev \
    libsqlite3-dev \
    libboost-all-dev \
    ros-humble-navigation2 \
    ros-humble-nav2-bringup \
    ros-humble-nav2-map-server \
    ros-humble-nav2-amcl \
    ros-humble-nav2-lifecycle-manager \
    ros-humble-nav2-bt-navigator \
    ros-humble-nav2-planner \
    ros-humble-nav2-controller \
    ros-humble-nav2-waypoint-follower \
    ros-humble-slam-toolbox \
    && locale-gen en_US.UTF-8 \
    && update-locale LANG=en_US.UTF-8 \
    && rm -rf /var/lib/apt/lists/*


# ==========================================

# ==========================================
# Creazione della struttura del workspace ROS2 e aggiunta rplidar_ros
# ==========================================
RUN mkdir -p /root/ros2_ws/src
COPY tmp_sources/ /root/ros2_ws/src/
WORKDIR /root/ros2_ws/src
# Clona rplidar_ros branch ros2
RUN git clone --branch ros2 --depth 1 https://github.com/Slamtec/rplidar_ros.git
WORKDIR /root/ros2_ws

RUN /bin/bash -lc "source /opt/ros/${ROS_DISTRO}/setup.bash && colcon build --symlink-install"

# ==========================================
# Script di entrypoint e configurazione bash
# ==========================================
RUN echo '#!/bin/bash' > /entrypoint.sh && \
    echo 'source /opt/ros/humble/setup.bash' >> /entrypoint.sh && \
    echo 'if [ -f /root/ros2_ws/install/setup.bash ]; then source /root/ros2_ws/install/setup.bash; fi' >> /entrypoint.sh && \
    echo 'export ROS_DOMAIN_ID=97' >> /entrypoint.sh && \
    echo 'export RMW_IMPLEMENTATION=rmw_cyclonedds_cpp' >> /entrypoint.sh && \
    echo 'exec "$@"' >> /entrypoint.sh && \
    chmod +x /entrypoint.sh

RUN echo "source /opt/ros/humble/setup.bash" >> /root/.bashrc \
    && echo "if [ -f /root/ros2_ws/install/setup.bash ]; then source /root/ros2_ws/install/setup.bash; fi" >> /root/.bashrc \
    && echo "export ROS_DOMAIN_ID=97" >> /root/.bashrc \
    && echo "export RMW_IMPLEMENTATION=rmw_cyclonedds_cpp" >> /root/.bashrc

ENTRYPOINT ["/entrypoint.sh"]
CMD ["/bin/bash", "-i"]