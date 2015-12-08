#!/bin/bash

git pull
make clean
sleep 1
make ARCH=target
sleep 1
scp bin/target/pi pi@169.254.253.2:
