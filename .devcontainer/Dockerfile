FROM mcr.microsoft.com/devcontainers/base:ubuntu

RUN apt-get update \ 
&& apt-get install -y --no-install-recommends \
clang \
clangd \
cmake \
lldb \
cmake \
ninja-build \
build-essential \
&& apt-get clean \
&& rm -rf /var/lib/apt/lists*

# make clang the default cpp compiler in the terminal
ENV CC=clang /
CXX=clangd++