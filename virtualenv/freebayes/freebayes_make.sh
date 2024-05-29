#! /usr/bin/bash

sudo dnf install -y perl-Test2-Harness
sudo dnf install -y patch
sudo dnf install -y bzip2-devel
sudo dnf install -y zlib-devel
sudo dnf install parallel -y
sudo dnf install htslib-devel -y
sudo dnf install zlib-devel -y
sudo dnf install samtools -y

git clone --recursive -b v1.3.2 https://github.com/freebayes/freebayes.git
cd ./freebayes/SeqLib
patch -p1 < ../../seqlib.patch
git submodule sync
cd ./bwa && git checkout master
cd ../fermi-lite && git checkout master
cd ../../
cd ./vcflib
patch -p1 < ../../vcflib.patch
cd ..
make
