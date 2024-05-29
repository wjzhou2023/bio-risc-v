#! /usr/bin/bash

sudo dnf install -y git gcc make g++ autoconf automake

# clone and compile star repo
sudo dnf install -y python3.7
pip install virtualenv

virtualenv scSplitter_env -p python3.7
cd ./scSplitter_env && source ./bin/activate

pip install -r ../requirements.txt

wget https://github.com/alexdobin/STAR/archive/refs/tags/2.6.1b.zip
unzip 2.6.1b.zip
cd ./STAR-2.6.1b && make STAR
cd ..

git clone https://github.com/zzhu33/scSplitter.git
