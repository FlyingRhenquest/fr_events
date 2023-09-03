#!/bin/bash
#
# Illustrate how to get and build this (Specifically in the
# docker image that accompanies this)

# Make a sandbox dir to check git stuff out to
mkdir ~/sandbox

# And a build directory to build things in
mkdir ~/build

cd ~/sandbox

# Clone fr_events
git clone https://github.com/FlyingRhenquest/fr_events.git

# Generate the makefile
mkdir ~/build/fr_events
cd ~/build/fr_events
cmake ~/sandbox/fr_events

# Build events library, python API and test (This will take a while)
make

# Now we can run test all the things
./test_all_the_things

# And the python example
python3 ~/sandbox/fr_events/examples/python/send_receive.py

echo Done -- Binaries are in ${HOME}/build/fr_events
