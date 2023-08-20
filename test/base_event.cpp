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
 *
 * Test base_event class.
 */

#include <gtest/gtest.h>
#include <fr_events/base_event.hpp>
#include <sstream>

using namespace fr::events;


/**
 * Test 1: Create, serialize, deserialize and compare events
 */

TEST(BaseEventTest, basic_functionality) {
  std::stringstream buffer;
  // There is literally no way to create a completely empty
  // base event, but we can mess with the event data to do our
  // test
  base_event event1;
  base_event event2;

  event1.name = "steve";

  // Make sure they don't match
  ASSERT_NE(event1.name, event2.name);
  
  {
    // Create an archive that uses buffer
    cereal::BinaryOutputArchive oarch(buffer);
    oarch(event1);
  }
  // Deserialize it into event2
  {
    // Input archive uses the same buffer
    cereal::BinaryInputArchive iarch(buffer);
    iarch(event2);
  }
  // Assert event2.name is now equal to event1.name
  ASSERT_EQ(event1.name, event2.name);
  // Also validate both events ancillary data is correct
  ASSERT_EQ(event1.payload_size, 0l);
  ASSERT_EQ(event2.payload_size, 0l);
  ASSERT_EQ(event1.payload, nullptr);
  ASSERT_EQ(event2.payload, nullptr);
}

/**
 * Test 2: Mess with payload_size and see if it crashes
 * If payload_size is 0, payload won't attempt to be loaded,
 * so we don't have to test payload with a garbage pointer.
 *
 * Note this will not help you if the payload_size is nonzero
 * AND the payload is non-null. But at least I'm trying to help
 * you not shoot yourself in the foot.
 */

TEST(BaseEventTest, does_payload_size_crash) {
  std::stringstream buffer;
  base_event event1;
  base_event event2;
  event1.name = "steve";
  event1.payload_size = 32l;

  {
    cereal::BinaryOutputArchive oarch(buffer);
    oarch(event1);
  }
  {
    cereal::BinaryInputArchive iarch(buffer);
    iarch(event2);
  }
  ASSERT_EQ(event2.payload_size, 0l);
  ASSERT_EQ(event2.payload, nullptr);
}


