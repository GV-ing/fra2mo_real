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
    locales \
    build-essential \
    python3-pip \
    python3-colcon-common-extensions \
    git \
    nano \
    curl \
    iputils-ping \
    ros-humble-rmw-cyclonedds-cpp \
    ros-humble-sensor-msgs \
    ros-humble-geometry-msgs \
    ros-humble-tf2 \
    ros-humble-xacro \
    ros-humble-realsense2-description \
    ros-humble-tf2-ros \
    ros-humble-joint-state-publisher \
    && locale-gen en_US.UTF-8 \
    && update-locale LANG=en_US.UTF-8 \
    && rm -rf /var/lib/apt/lists/*


# ==========================================
# Creazione della struttura del workspace ROS2 (Il tuo codice originale)
# ==========================================
RUN mkdir -p /root/ros2_ws/src
COPY tmp_sources/ /root/ros2_ws/src/
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