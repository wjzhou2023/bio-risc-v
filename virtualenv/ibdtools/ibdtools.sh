#! /usr/bin/bash

sudo dnf install -y git
sudo dnf install -y gcc make cmake g++
sudo dnf install -y meson
sudo dnf install -y ninja-build
sudo dnf install -y htslib-devel
sudo dnf install -y htslib
sudo dnf install -y fmt
sudo dnf install -y fmt-devel
sudo dnf install -y gtest-devel
sudo dnf install -y gtest
sudo dnf install -y python3.8
sudo dnf install -y java-17-openjdk
sudo dnf install -y gsl-devel

curl --proto '=https' --tlsv1.2 -sSf https://rsproxy.cn/rustup-init.sh -o init.sh
sudo chmod +x ./init.sh
sh -c "./init.sh -y"
rm -rf init.sh

# install pip
sudo dnf -y install wget
wget https://bootstrap.pypa.io/get-pip.py
python3.8 get-pip.py
rm -rf get-pip.py

# create virtual env
pip3.8 install virtualenv
virtualenv ibdtools_env -p python3.8
cd ./ibdtools_env
echo "export PATH=$PATH:$HOME/.cargo/bin" >> ./bin/activate
source ./bin/activate

git clone --recursive https://github.com/umb-oconnorgroup/ibdtools.git
cd ./ibdtools
meson build
ninja -C build


pip install msprime
./build/ibdtools -h
