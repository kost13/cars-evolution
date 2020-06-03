#!/bin/bash
# steps to run on the new, clear Linux Ubuntu

sudo add-apt-repository universe
sudo apt-get update
sudo apt-get upgrade

sudo apt-get install build-essential
sudo apt-get install cmake
sudo apt-get install libboost-test-dev
sudo apt-get install qt5-default qtdeclarative5-dev libqt5charts5-dev qtquickcontrols2-5-dev
sudo apt-get install qml-module-qtquick2 qml-module-qtquick-controls qml-module-qtquick-controls2 qml-module-qtquick-dialogs qml-module-qtcharts
