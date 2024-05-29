#! /usr/bin/bash

sudo dnf -y install cmake ninja-build gcc g++
sudo dnf -y install autoconf automake git
sudo dnf -y install zlib-devel
sudo dnf -y install bzip2-devel
sudo dnf -y install xz-devel
sudo dnf -y install libjpeg-turbo-devel
sudo dnf -y install python3.11
sudo dnf -y install python3-scipy
sudo dnf -y install python3-matplotlib

# install pip
sudo dnf -y install wget
wget https://bootstrap.pypa.io/get-pip.py
python3.11 get-pip.py
rm -rf get-pip.py

pip3.11 install virtualenv

virtualenv deepTools_env -p python3.11
cd ./deepTools_env
source ./bin/activate

dst="lib64/python3.11/site-packages"
cp -r /usr/lib64/python3.11/site-packages/scipy $dst
cp -r /usr/lib64/python3.11/site-packages/matplotlib $dst
cp -r /usr/lib64/python3.11/site-packages/SciPy-1.10.1-py3.11.egg-info $dst

git clone https://github.com/deeptools/deepTools.git
cd ./deepTools
pip install -e .
