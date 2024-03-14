FROM ubuntu:latest 

RUN apt-get update && \
    apt-get install -y \
    build-essential \
    cmake \
    libboost-all-dev

# Create a non-root user `developer` without password
# -m option to create a home directory
RUN useradd -m developer

RUN mkdir /app && \
    chown -R developer:developer /app

WORKDIR /app

USER developer

RUN mkdir /app/build

# Set the default command to a shell
# This keeps the container running and allows you to exec into it
CMD ["/bin/bash"]
