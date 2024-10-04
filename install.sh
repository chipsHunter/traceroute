#!/bin/bash

# Устанавливаем необходимые зависимости (на случай отсутствия)
sudo apt update
sudo apt install -y cmake g++ make

# Клонируем репозиторий
https://github.com/chipsHunter/traceroute.git
cd traceroute

# Сборка и установка
mkdir build
cd build
cmake ..
make
sudo make install