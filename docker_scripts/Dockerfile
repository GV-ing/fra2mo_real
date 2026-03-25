# Usiamo ros-base invece di desktop: niente GUI, solo il core di ROS2 (molto più leggero!)
FROM ros:humble-ros-base

# Configurazione ambiente base
ENV DEBIAN_FRONTEND=noninteractive \
    LANG=en_US.UTF-8 \
    LC_ALL=en_US.UTF-8 \
    HOME=/root \
    ROS_DISTRO=humble

WORKDIR /root

# Installazione strumenti essenziali e dipendenze hardware/comunicazione
RUN apt-get update && apt-get install -y --no-install-recommends \
    build-essential \
    python3-pip \
    python3-colcon-common-extensions \
    git \
    nano \
    iputils-ping \
    ros-humble-rmw-cyclonedds-cpp \
    ros-humble-sensor-msgs \
    ros-humble-geometry-msgs \
    ros-humble-tf2 \
    ros-humble-tf2-ros \
    && rm -rf /var/lib/apt/lists/*

# Creazione della struttura del workspace ROS2
RUN mkdir -p /root/ros2_ws/src
WORKDIR /root/ros2_ws

# Script di entrypoint per configurare la rete e l'ambiente al lancio
RUN echo '#!/bin/bash' > /entrypoint.sh && \
    echo 'source /opt/ros/humble/setup.bash' >> /entrypoint.sh && \
    # Il file setup.bash del workspace verrà "sourced" solo se esiste (cioè dopo la prima compilazione)
    echo 'if [ -f /root/ros2_ws/install/setup.bash ]; then source /root/ros2_ws/install/setup.bash; fi' >> /entrypoint.sh && \
    echo 'export ROS_DOMAIN_ID=97' >> /entrypoint.sh && \
    echo 'export RMW_IMPLEMENTATION=rmw_cyclonedds_cpp' >> /entrypoint.sh && \
    echo 'exec "$@"' >> /entrypoint.sh && \
    chmod +x /entrypoint.sh

# Configurazione del .bashrc per l'uso interattivo da terminale
RUN echo "source /opt/ros/humble/setup.bash" >> /root/.bashrc \
    && echo "if [ -f /root/ros2_ws/install/setup.bash ]; then source /root/ros2_ws/install/setup.bash; fi" >> /root/.bashrc \
    && echo "export ROS_DOMAIN_ID=97" >> /root/.bashrc \
    && echo "export RMW_IMPLEMENTATION=rmw_cyclonedds_cpp" >> /root/.bashrc

ENTRYPOINT ["/entrypoint.sh"]
CMD ["/bin/bash", "-i"]