docker start TPE-Arqui
docker exec -it TPE-Arqui make clean -C /root/Toolchain
docker exec -it TPE-Arqui make clean -C /root/
docker exec -it TPE-Arqui make -C /root/Toolchain
docker exec -it TPE-Arqui make -C /root/
docker exec -it TPE-Arqui chmod 777 /root/Image/x64BareBonesImage.qcow2 
# docker stop TPE-Arqui
