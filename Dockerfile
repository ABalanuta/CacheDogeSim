FROM ubuntu:18.04
LABEL maintainer "Artur Balanuta"
MAINTAINER "artur@cmu.edu"

RUN	apt-get update && \
	apt-get install -y htop nano vim aria2 build-essential m4 less

ENV WORK_DIR /root
WORKDIR ${WORK_DIR}

ENV PIN_VER "pin-3.7-97619-g0d0c92f4f-gcc-linux"
ENV PIN_URL "https://software.intel.com/sites/landingpage/pintool/downloads/$PIN_VER.tar.gz"
ENV PIN_HOME "/root/$PIN_VER"
RUN aria2c -x 16 --summary-interval=1 $PIN_URL && tar -xvf $PIN_VER.tar.gz && rm $PIN_VER.tar.gz
RUN cd $WORK_DIR/$PIN_VER/source/tools/ManualExamples && make all TARGET=intel64
RUN cd $WORK_DIR/$PIN_VER/source/tools/Memory && make all TARGET=intel64

ENV PARSEC_CORE "parsec-3.0-core"
ENV PARSEC_INPUTS "parsec-3.0-input-sim"
ENV PARSEC_CORE_URL "http://parsec.cs.princeton.edu/download/3.0/$PARSEC_CORE.tar.gz"
ENV PARSEC_INPUTS_URL "http://parsec.cs.princeton.edu/download/3.0/$PARSEC_INPUTS.tar.gz"
ENV PARSEC_DIR "parsec-3.0"
RUN aria2c -x 16 --summary-interval=1 $PARSEC_CORE_URL && tar -xvf $PARSEC_CORE.tar.gz && rm $PARSEC_CORE.tar.gz
RUN aria2c -x 16 --summary-interval=1 $PARSEC_INPUTS_URL && tar -xvf $PARSEC_INPUTS.tar.gz && rm $PARSEC_INPUTS.tar.gz

VOLUME /${WORK_DIR}/src