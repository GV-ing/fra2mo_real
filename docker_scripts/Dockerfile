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
    ros-humble-tf2-ros \
    && locale-gen en_US.UTF-8 \
    && update-locale LANG=en_US.UTF-8 \
    && rm -rf /var/lib/apt/lists/*

# ==========================================
# 1. INSTALLAZIONE MICRO-ROS AGENT
# ==========================================
# Creiamo un workspace separato per micro-ROS per tenere pulito il tuo ros2_ws
WORKDIR /root/microros_ws
RUN git clone -b humble https://github.com/micro-ROS/micro_ros_setup.git src/micro_ros_setup && \
    rosdep init || true && \
    rosdep update && \
    rosdep install --from-paths src --ignore-src -y && \
    /bin/bash -c "source /opt/ros/humble/setup.bash && colcon build" && \
    /bin/bash -c "source /opt/ros/humble/setup.bash && source install/local_setup.bash && ros2 run micro_ros_setup create_agent_ws.sh" && \
    /bin/bash -c "source /opt/ros/humble/setup.bash && source install/local_setup.bash && ros2 run micro_ros_setup build_agent.sh" && \
    rm -rf log/ build/ src/

# ==========================================
# 2. INSTALLAZIONE ARDUINO CLI E TOOLCHAIN PICO
# ==========================================
WORKDIR /root
RUN curl -fsSL https://raw.githubusercontent.com/arduino/arduino-cli/master/install.sh | sh && \
    cp bin/arduino-cli /usr/local/bin/ && \
    arduino-cli core update-index --additional-urls https://github.com/earlephilhower/arduino-pico/releases/download/global/package_rp2040_index.json && \
    arduino-cli core install rp2040:rp2040 --additional-urls https://github.com/earlephilhower/arduino-pico/releases/download/global/package_rp2040_index.json && \
    mkdir -p /root/Arduino/libraries && \
    cd /root/Arduino/libraries && \
    git clone -b humble https://github.com/micro-ROS/micro_ros_arduino.git

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
    echo 'source /root/microros_ws/install/local_setup.bash' >> /entrypoint.sh && \
    echo 'if [ -f /root/ros2_ws/install/setup.bash ]; then source /root/ros2_ws/install/setup.bash; fi' >> /entrypoint.sh && \
    echo 'export ROS_DOMAIN_ID=97' >> /entrypoint.sh && \
    echo 'export RMW_IMPLEMENTATION=rmw_cyclonedds_cpp' >> /entrypoint.sh && \
    echo 'exec "$@"' >> /entrypoint.sh && \
    chmod +x /entrypoint.sh

RUN echo "source /opt/ros/humble/setup.bash" >> /root/.bashrc \
    && echo "source /root/microros_ws/install/local_setup.bash" >> /root/.bashrc \
    && echo "if [ -f /root/ros2_ws/install/setup.bash ]; then source /root/ros2_ws/install/setup.bash; fi" >> /root/.bashrc \
    && echo "export ROS_DOMAIN_ID=97" >> /root/.bashrc \
    && echo "export RMW_IMPLEMENTATION=rmw_cyclonedds_cpp" >> /root/.bashrc

ENTRYPOINT ["/entrypoint.sh"]
CMD ["/bin/bash", "-i"]