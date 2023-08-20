/**
 * Copyright 2023 Bruce Ide
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

#include <gtest/gtest.h>

#include <fr_events/simple_transport.hpp>

using namespace fr::events;
using namespace fr::events::transport;

/**
 * We need an event handler for this test
 *
 * Funny story, I was doing the zmq_test and had named
 * this object and the one in the zmq_test to both be
 * test_handler. This test test_handler got picked up
 * instead of the one in the zmq_test file, which was
 * quite confusing for a couple hours before I noticed
 * in the debugger that the test_handler was being
 * picked up from this file. So the names should be
 * a bit more verbose, or I should put my tests in a
 * testing namespace, or both.
 */

class simple_transport_test_handler : public event_handler {
public:
  bool gotEvent=false;

  void handle(std::shared_ptr<base_event> event) override {
    gotEvent = true;
  }
  
};

TEST(SimpleTransportTest, send_receive) {
  simple_sender sender;
  simple_receiver receiver;
  simple_transport_test_handler handler;

  ASSERT_FALSE(handler.gotEvent);
  sender.subscribe(receiver);
  handler.subscribe(receiver);

  // We need an event to send
  std::shared_ptr<base_event> event = std::make_shared<base_event>();
  sender.send(event);
  ASSERT_TRUE(handler.gotEvent);
}

