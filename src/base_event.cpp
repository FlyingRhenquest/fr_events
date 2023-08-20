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

#include <fr_events/base_event.hpp>
#include <string.h>

namespace fr::events {

  base_event::base_event() : name("base_event"), payload_size(0l), payload(nullptr) {}

  base_event::base_event(const base_event& copy) :
    name(copy.name), payload_size(copy.payload_size) {
    if (payload_size) {
      if (!payload) {
	// Let's not crash
	payload_size = 0;
	payload = nullptr;
      } else {
	payload = malloc(payload_size);
	memcpy(payload, copy.payload, payload_size);
      }
    }
  }
			   
  base_event::~base_event() {}
  
}
