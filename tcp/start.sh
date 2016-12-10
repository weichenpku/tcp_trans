#!/bin/bash

interface_server="enp0s31f6"
interface_router1="enp0s31f6"
interface_router2="enp0s31f6"
path="../etc/Resolve.conf"
target_file="testfile"
my_file="myfile"

file1="					
sudo ifconfig ${interface_router1} 192.168.1.2 up; 		
./ftp_client 192.168.1.1 ${target_file} ${my_file};
"


file2="
sudo ifconfig ${interface_router2} 192.168.1.3 up;
./ftp_client 192.168.1.1 ${target_file} ${my_file};
"

file3="
sudo ifconfig ${interface_server} 192.168.1.1 up;
./ftp_server;
"

file4="
hello world
bye
"

file5="
rm ftp_client ftp_server name* router* ${target_file} ${my_file}
"

echo ${file1} >> router1.sh
echo ${file2} >> router2.sh
echo ${file3} >> nameserver.sh
echo ${file4} >> ${target_file}
echo ${file5} >> stop.sh

chmod 711 *.sh
gcc -o ftp_server ftp_server.c
gcc -o ftp_client ftp_client.c
#eof
