# Asynchronous C++ Server
This is my simple server written in C++ using epoll events  
It supports SSL

## Requirements
For ssl support, you will need to install wolfssl libraries  
You can install it simply running this command on debian sid/bullseye  
```
sudo apt-get install libwolfssl-dev
```
You also can install it manually cloning it from official repo  

## Setup
For setup you need to execute `make` or `USE_SSL=1 make` (if you want add ssl support)
