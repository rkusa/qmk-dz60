FROM debian:buster-slim

RUN apt-get update && apt-get install -y git
RUN apt install git
RUN git clone https://github.com/qmk/qmk_firmware.git
RUN cd qmk_firmware && ./util/linux_install.sh

