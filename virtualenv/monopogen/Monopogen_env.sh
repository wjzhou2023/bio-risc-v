#! /usr/bin/bash

sudo dnf install -y git make cmake gcc g++ automake autoconf
sudo dnf install -y python3.11
sudo dnf install -y python3-scipy
sudo dnf install -y python3-matplotlib
sudo dnf install -y htslib-devel
sudo dnf install -y htslib
sudo dnf install -y java-17-openjdk
sudo dnf install -y R-data.table
sudo dnf install -y R-ggplot2
sudo dnf install -y automake
sudo dnf install -y autoconf
sudo dnf install -y bcftools
sudo dnf install -y libdeflate-devel
sudo dnf install -y libdeflate
sudo dnf install -y ncurses-devel
sudo dnf install -y libjpeg-turbo-devel
sudo dnf install -y libcurl-devel
sudo dnf install -y openssl-devel
sudo dnf install -y curl
sudo dnf install -y R

R --quiet -e 'install.packages("e1071",repos="https://mirrors.bfsu.edu.cn/CRAN/")'

# install pip
sudo dnf -y install wget
wget https://bootstrap.pypa.io/get-pip.py
python3.11 get-pip.py
rm -rf get-pip.py

pip3.11 install virtualenv
virtualenv Monopogen_env -p python3.11
cd ./Monopogen_env
source ./bin/activate

dst="./lib64/python3.11/site-packages"
# cp system lib
cp -r /usr/lib64/python3.11/site-packages/scipy $dst
cp -r /usr/lib64/python3.11/site-packages/SciPy-1.10.1-py3.11.egg-info $dst
cp -r /usr/lib64/python3.11/site-packages/matplotlib $dst

# clone samtools
git clone -b 1.4 https://github.com/samtools/samtools.git
cd ./samtools
autoconf -Wno-syntax
./configure
make
cd ..

git clone --recursive https://github.com/samtools/htslib.git
cd ./htslib
autoreconf -i
./configure
make
cd ..

