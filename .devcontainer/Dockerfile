# Set the Docker image tag (override if necessary in devcontainer.json)
ARG DOCKER_TAG=latest

# Use the image as the base image
FROM alpine:${DOCKER_TAG}

# Set environment variables for language
ENV LC_ALL=C.UTF-8
ENV LANG=C.UTF-8

# Install necessary packages
RUN apk update && apk add --no-cache \
    build-base \
    cmake \
    gcc \
    g++ \
    gdb \
    ninja-build \
    lcov \
    git \
    gzip \
    perl \
    bash

# Set up the working directory
WORKDIR /workspace

# Set the entrypoint to bash
ENTRYPOINT ["/bin/bash"]