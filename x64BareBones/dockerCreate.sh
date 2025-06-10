# Run this script only the fist time then it's unnecessary
sudo docker pull agodio/itba-so:2.0
sudo docker run -d -v ${PWD}:/root --security-opt seccomp:unconfined -it --name TPE-Arqui agodio/itba-so:2.0


