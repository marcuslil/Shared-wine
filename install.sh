#!/bin/sh

sudo apt-get install libc6-dev-i386 wine

sudo adduser --system --group sharedwine --disabled-password --home /sharedwine --shell /bin/bash
sudo install -d /sharedwine -o sharedwine -g sharedwine -m u=rwx,g=rx,o=
sudo addgroup --system sharedwineadm

#sudo install path.reg /sharedwine -o sharedwine -g sharedwine -m u=rwx,g=rx,o=

gcc -m32 wine.c -o wine

A=`dpkg-divert --list /usr/bin/wine.orig`

echo $A

if [ "$A" = "" ]; then
  echo dirvting wine
  sudo dpkg-divert --rename --divert /usr/bin/wine.orig --add /usr/bin/wine
else
  echo wine alread diverted
fi

echo sudo add
sudo sh -c "echo >/root/sharedwine %sharedwineadm    ALL =  \(sharedwine\)  ALL"
sudo chmod 0440  /root/sharedwine
sudo mv /root/sharedwine /etc/sudoers.d/sharedwine

sudo install sharedwine.sh /usr/local/bin
sudo install wine /usr/bin 
sudo install 99sharedwine  /etc/X11/Xsession.d/
sudo install -d /usr/local/share/applications/
sudo install sharedwine.desktop /usr/local/share/applications/
rm wine

