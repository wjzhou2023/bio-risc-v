#! /bin/bash
sudo dnf install -y gcc
sudo dnf install -y python
sudo dnf install -y zlib-ng-devel
sudo dnf install -y zlib-devel
sudo dnf install -y zlib
sudo dnf install -y make
sudo dnf install -y cmake
sudo dnf install -y ninja-build
sudo dnf install -y gcc-c++
sudo dnf install -y autoconf
sudo dnf install -y automake
sudo dnf install -y python3-devel
sudo dnf install -y bzip2-devel
sudo dnf install -y xz-devel
sudo dnf install -y leveldb
sudo dnf install -y leveldb-devel
sudo dnf install -y python3-leveldb
sudo dnf install -y libjpeg-turbo.riscv64
sudo dnf install -y libjpeg-turbo-devel.riscv64
sudo dnf install -y libffi-devel
sudo dnf install -y libgomp
sudo dnf install -y libgcc
sudo dnf install -y ncurses-devel
sudo dnf install -y openssl-devel
sudo dnf install -y python3-pip
sudo dnf install -y readline-devel
sudo dnf install -y python3-setuptools
sudo dnf install -y sqlite-devel
sudo dnf install -y tk
sudo dnf install -y python3-matplotlib

pip install virtualenv
virtualenv htseq_env -p python
cd htseq_env
source ./bin/activate
pip list
pip install -r ../requirements.txt
pip list
cp -r /usr/lib64/python3.11/site-packages/matplotlib lib64/python3.11/site-packages
cp -r /usr/lib64/python3.11/site-packages/matplotlib-3.7.2.dist-info/ lib64/python3.11/site-packages
cp -r /usr/lib64/python3.11/site-packages/mpl_toolkits/ lib64/python3.11/site-packages
cp -r /usr/share/matplotlib/ share/

htseq-count -h
htseq-qa -h
