FROM ubuntu:16.04

RUN apt-get -y update && apt-get -y install libdevil-dev libboost-regex-dev libunwind8-dev libcurl4-gnutls-dev

COPY spring-headless /spring-headless

EXPOSE 9000-9999/udp

ENV PORT 9000

RUN apt-get clean

CMD /spring-headless $PORT
