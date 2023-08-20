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
 */

#pragma once

#include <cereal/archives/binary.hpp>
#include <cereal/archives/json.hpp>
#include <cereal/archives/portable_binary.hpp>
#include <cereal/types/memory.hpp>
#include <cereal/types/polymorphic.hpp>
#include <memory>
#include <string>

namespace fr::events {

  /**
   * This is a base event. Event requirements:
   *
   * * Event must be serializable (To at least Binary and JSON)
   *   (Cereal is used for this.)
   * * Events have a name, which should generally be immutable
   *   for any given class of events.
   * * All events have a payload and a payload size.
   * * Payload size can be 0
   * * If payload size is 0, payload will be assumed to be nullptr.
   * * Base events should never actually be used, unless you really
   *   want to. Specialize a different event type instead.
   */
  
  class base_event {
  public:
    std::string name;
    size_t payload_size;
    void* payload;

    /**
     * Creates a base event with a name of base_event, a
     * payload size of 0 and a null payload.
     */
    
    base_event();

    /**
     * Copy constructor. This will alloc new memory for the payload
     * if payload size is greater than 0.
     */

    base_event(const base_event& copy);
    
    /**
     * Virtual destructor for Polymorphic events. Base class
     * destructor does nothing.
     */
    virtual ~base_event();

    /**
     * Return a shared ptr to a base event
     */

    static std::shared_ptr<base_event> create() {
      return std::make_shared<base_event>();
    }
    
    /**
     * Serialzie/Deserialize via cereal archives
     */
    
    template<class Archive>
    void save(Archive& ar) const {
      bool save_payload = true;
      ar(name);
      if (payload_size && !payload) {
	// Let's not crash
	ar(0l);
	save_payload = false;
      } else {
	ar(payload_size);
      }
      if (save_payload) {
	ar.saveBinary(payload, payload_size);
      }
    }

    template<class Archive>
    void load(Archive& ar) {
      ar(name, payload_size);
      if (payload_size) {
	ar.loadBinary(payload, payload_size);
      }
    }
    
  };
  
}
