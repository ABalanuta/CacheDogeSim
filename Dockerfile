FROM ubuntu:18.04
LABEL maintainer "Artur Balanuta"
MAINTAINER "artur@cmu.edu"

ENV PIN_VER "pin-3.7-97619-g0d0c92f4f-gcc-linux"
ENV PIN_URL "https://software.intel.com/sites/landingpage/pintool/downloads/$PIN_VER.tar.gz"
ENV WORK_DIR /root
WORKDIR ${WORK_DIR}

RUN	apt-get update && \
	apt-get install -y htop nano vim wget build-essential

RUN wget $PIN_URL && tar -xf $PIN_VER.tar.gz && rm $PIN_VER.tar.gz


VOLUME /${WORK_DIR}/src