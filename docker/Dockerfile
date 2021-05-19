# Base image
FROM amd64/debian:stable

# Copy sed-patch files into image
#COPY *.sed /tmp/
COPY checkbuild.sh /tmp/checkbuild.sh

# Package dependencies
RUN apt-get update && \
    apt-get install -y \
        build-essential \
        libreadline-dev \
        wget \
        re2c \
        vim \
        git-all

# Download EPICS, unpack, build it
RUN wget -P /tmp/ https://epics-controls.org/download/base/base-7.0.5.tar.gz && \
    mkdir -p /usr/local/epics && \
    tar xfz /tmp/base-7.0.5.tar.gz -C /usr/local/epics && \
    ln -s /usr/local/epics/base-7.0.5 /usr/local/epics/base && \
    cd /usr/local/epics/base && make


ENTRYPOINT ["/tmp/checkbuild.sh"]

