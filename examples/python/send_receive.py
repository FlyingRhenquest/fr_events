#!/usr/bin/env python3
#
# This is designed to run in the directory where you
# build the api library. You shouldn't need the next two
# lines if you install the library somewhere python can
# find it
import sys, os
sys.path.append('.')

import fr_events_python_api as api
import time

receiver = api.zmq_receiver()
sender = api.zmq_sender()

# Need one of these in Python since we can't receive
# events directly
eventbuffer = api.event_buffer()

# This listens in a background thread and can support
# any communication channel ZMQ does.
receiver.listen("tcp://127.0.0.1:23456")

# Give it a moment to connect. Python is so slow that
# we may not need this, but the example should be a lot
# more reliable if we do. I'm consdiering making listen
# block until the listening thread has been established,
# which would eliminate the need for this.

time.sleep(1)

# Sender also communicates in a background thread.
# This connects it to the receiver above.
sender.connect_to_server("tcp://127.0.0.1:23456")

eventbuffer.subscribe_to(receiver)
time.sleep(1)

# Create an event and send it. Right now we only
# have base_event to work with.
event = api.base_event()
event.name = "Bingo!"

print("Items in eventbuffer before send: " + str(eventbuffer.size()))

sender.send(event);

# More than enough time for the event to be received
time.sleep(1)

print("items in event buffer after send: " + str(eventbuffer.size()))

# Retrieve event. Note: get is LIFO
event2 = eventbuffer.get()

print("Event name: " + event.name)
print("Event2 name: " + event2.name)
