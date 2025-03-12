# Set the Docker image tag (override if necessary in devcontainer.json)
ARG DOCKER_TAG=latest

# Use the image as the base image
FROM espressif/idf:${DOCKER_TAG}

# Set environment variables for language
ENV LC_ALL=C.UTF-8
ENV LANG=C.UTF-8

# Install necessary packages
RUN apk update && apk add --no-cache \
    udev \
    build-essential

# Run export of ESP-IDF
RUN echo "source /opt/esp/idf/export.sh > /dev/null 2>&1" >> ~/.bashrc

# Configure entry point
ENTRYPOINT [ "/opt/esp/entrypoint.sh" ]

# Set the entrypoint to bash
CMD ["/bin/bash", "-c"]