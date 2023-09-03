FROM ubuntu:22.04
RUN DEBIAN_FRONTEND=noninteractive \
    apt-get update \
    && apt-get upgrade \
    && apt-get install sudo
RUN useradd -ms /bin/bash testuser
RUN usermod -a -G sudo testuser
RUN echo "testuser ALL=(ALL) NOPASSWD:ALL" > /etc/sudoers
COPY ubuntish.sh /tmp/ubuntish.sh
COPY build_events.sh /tmp/build_events.sh
RUN chown testuser.testuser /tmp/ubuntish.sh
RUN chown testuser.testuser /tmp/build_events.sh
RUN chmod 755 /tmp/ubuntish.sh /tmp/build_events.sh
USER testuser