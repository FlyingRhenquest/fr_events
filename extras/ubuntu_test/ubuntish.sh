#!/bin/bash
#
# Setup script for dev environment
#
# This should work for most debian-based deratives (Ubuntu,
# mint, et al.)
#
# This will call sudo to install a bunch of stuff on your
# system, so please review it and make sure it's not doing
# something you don't want it to.
#
# This assumes you have a base system with pretty much
# nothing installed, but if you already have some things
# apt will skip over those things
#
# This does not necessarily guarantee python will work
# correctly for you, especially if you already have
# some other version installed or one of the pyenv type
# programs running.

# Make sure apt is up to date
sudo apt update

# install git, build-essential, python3 and cmake
sudo apt install -y git build-essential python3 python3-dev cmake

# Need to install boost serialization separately for some reason
sudo apt install -y libboost-serialization-dev

# And boost-dev (For cereal et al.)
sudo apt install -y libboost-dev

# Also pkgconfig for cmake
sudo apt install -y pkg-config
