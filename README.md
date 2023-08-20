# fr_events Library

This is an event library that is able to transmit events across
a network connection. Currently only ZMQ is supported for network
transport, but I intend to add AMQP for rabbitmq soon.

# Building

This should try to download everything it needs. You'll need a
version of cmake that supports fetchcontent. You'll also need
a C++17 compiler. This is built on a recent version of G++.
If you're using a fairly recent linux distribtion you should
be fine. Please let me know if you encounter any problems
with the build. I'd like to make sure it runs on as many
platforms as possible.

Given all that, create a build directory somewhere, IE:

mkdir /tmp/build

cd to that directory and run:

cmake /path/to/CMakelists.txt IE:

cmake ~/sandbox/fr_events

Then run make:

make

You can run the ./test_all_the_things executable if it builds,
or run an example python program in fr_events/examples/python
to demonstrate its use.

# Roadmap

 * Other event types -- I want to use this for my video processing.
 * AMQP support
 * More unit tests
 * More Python examples
 * Javascript exasmples for receiving events from C++ (ZMQ or AMQP
   or both)
