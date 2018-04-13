#!/bin/bash

set -e

date
ps axjf

#################################################################
# Update Ubuntu and install prerequisites for running Impleum   #
#################################################################
sudo apt-get update
#################################################################
# Build Impleum from source                                     #
#################################################################
NPROC=$(nproc)
echo "nproc: $NPROC"
#################################################################
# Install all necessary packages for building Impleum           #
#################################################################
sudo apt-get install -y qt4-qmake libqt4-dev libminiupnpc-dev libdb++-dev libdb-dev libcrypto++-dev libqrencode-dev libboost-all-dev build-essential libboost-system-dev libboost-filesystem-dev libboost-program-options-dev libboost-thread-dev libboost-filesystem-dev libboost-program-options-dev libboost-thread-dev libssl-dev libdb++-dev libssl-dev ufw git
sudo add-apt-repository -y ppa:bitcoin/bitcoin
sudo apt-get update
sudo apt-get install -y libdb4.8-dev libdb4.8++-dev

cd /usr/local
file=/usr/local/impleumQT
if [ ! -e "$file" ]
then
        sudo git clone https://github.com/impleum/ImpleumQt.git
fi

cd /usr/local/impleumQT/src
file=/usr/local/impleumQT/src/impleumd
if [ ! -e "$file" ]
then
        sudo make -j$NPROC -f makefile.unix
fi

sudo cp /usr/local/impleumQT/src/impleumd /usr/bin/impleumd

################################################################
# Configure to auto start at boot                                      #
################################################################
file=$HOME/.impleum
if [ ! -e "$file" ]
then
        sudo mkdir $HOME/.impleum
fi
printf '%s\n%s\n%s\n%s\n' 'daemon=1' 'server=1' 'rpcuser=u' 'rpcpassword=p' | sudo tee $HOME/.impleum/impleum.conf
file=/etc/init.d/impleum
if [ ! -e "$file" ]
then
        printf '%s\n%s\n' '#!/bin/sh' 'sudo impleumd' | sudo tee /etc/init.d/impleum
        sudo chmod +x /etc/init.d/impleum
        sudo update-rc.d impleum defaults
fi

/usr/bin/impleumd
echo "Impleum has been setup successfully and is running..."
exit 0

