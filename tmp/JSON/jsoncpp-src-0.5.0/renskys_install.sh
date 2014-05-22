#!/bin/bash
sudo cp libs/linux-gcc-4.8/libjson_linux-gcc-4.8_libmt.so /usr/lib
sudo mkdir /usr/include/jsoncpp
sudo cp include/json/* /usr/include/jsoncpp/
