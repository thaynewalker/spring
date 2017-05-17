#!/bin/bash
if [ "$EUID" -eq 0 ]
  then echo "Please do not run as root"
  exit
fi

cp /usr/local/bin/spring-headless ./
cp -r /usr/local/share/games/spring ./
mkdir -p .spring/games .spring/maps
rsync -av --ignore-existing  /home/thayne/.spring/games/springtutorialgame.sdd ./.spring/games/
rsync -av --ignore-existing  /home/thayne/.spring/maps/DeltaSiegeDry.sd7 ./.spring/maps/
docker build -t spring .
docker save spring > spring.tar
scp spring.tar montrose:
