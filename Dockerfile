FROM ubuntu:jammy

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && \
    apt-get install -y --no-install-recommends \
        build-essential \
        cmake \
        git \
        valgrind \
        && rm -rf /var/lib/apt/lists/*

# Add vscode user
RUN useradd -m -s /bin/bash vscode

WORKDIR /workspace
CMD ["/bin/bash"]
