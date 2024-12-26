# Use Ubuntu as base image
FROM ubuntu:22.04

# Install build essentials and CMake
RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    gcc \
    g++ \
    make \
    && rm -rf /var/lib/apt/lists/*

# Set working directory
WORKDIR /app

# The source files will be mounted at runtime