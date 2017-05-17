FROM ubuntu:16.04

ENV http_proxy http://204.68.140.236:80
ENV https_proxy http://204.68.140.236:80

RUN apt-get -y update
RUN apt-get -y install libdevil-dev libboost-regex-dev libunwind8-dev libcurl4-gnutls-dev
RUN apt-get -y install minizip libcurl3

#COPY .config /root/.config
COPY .spring /root/.spring
COPY spring-headless /spring-headless
COPY spring /usr/local/share/games/spring

RUN apt-get clean

CMD /spring-headless $PORT
