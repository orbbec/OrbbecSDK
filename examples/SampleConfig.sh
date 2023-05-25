#!/bin/bash
#this is a Configuration script for sample of ObsensorSDK,Created by zyb 2020-12-10
function installDependency(){
echo -e "\033[31m----------------------------Now install Opencv dependency ....------------------------------\033[0m"
sudo apt-get --assume-yes install build-essential
sudo apt-get --assume-yes install libgtk-3-dev pkg-config libavcodec-dev libavformat-dev libswscale-dev
#sudo apt-get --assume-yes install libtbb2 libtbb-dev libjpeg-dev libpng-dev libtiff-dev libjasper-dev
#sudo apt-get --assume-yes install libxvidcore-dev libx264-dev python-dev python-numpy libdc1394-22-dev
echo -e "\033[31m-------------------------Finish Opencv dependency installation !!------------------------------\033[0m" 
}
echo -e "\033[31m---------------------Before Beginning to Config ,please make sure the net is OK!!-----------\033[0m"
echo -e "\033[31m------------------Maybe the Configuration process will cost your some time!!------------------\033[0m"
  if [ `id -u` -eq 0 ];then 
  	echo -e "\033[31m---------------------Begin to check your Net!!------------------\033[0m" 
    else  
        echo -e "\033[31m-----------------You are not root !! please add [sudo] commond----------\033[0m "  
	exit
    fi 
ping -c 1 114.114.114.114 > /dev/null 2>&1 
if [ $? -eq 0  ];then 
echo -e "\033[31m-------------------------The Net is ok!!----------------------------------\033[0m" 
installDependency
echo -e "\033[31m---------------------now system configuration finish,It's time to begin a happy code traveling!-----------\033[0m"
else  
echo -e "\033[31m-----------------Sorry you can not connect the Net ,please make sure the net is OK !!----------\033[0m "
fi  
