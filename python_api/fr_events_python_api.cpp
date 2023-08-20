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

#include <pybind11/pybind11.h>
#include <fr_events/fr_events.hpp>

PYBIND11_MODULE(fr_events_python_api, m) {

  /**
   * Base event object. This just has a name plus an optional payload holder.
   * Should be OK to construct these for testing. Currently you only have
   * access to the name but can change that at will.
   */

  pybind11::class_<fr::events::base_event, std::shared_ptr<fr::events::base_event>>(m, "base_event")
    .def(pybind11::init<>())
    .def_readwrite("name", &fr::events::base_event::name)
    ;

  pybind11::class_<fr::events::transport::receiver, std::shared_ptr<fr::events::transport::receiver>>(m, "receiver")
    .def(pybind11::init<>())
    ;
  
  pybind11::class_<fr::events::transport::zmq_sender, std::shared_ptr<fr::events::transport::zmq_sender>>(m, "zmq_sender")
    .def(pybind11::init<>())
    .def("connect_to_server", &fr::events::transport::zmq_sender::add, "Addresses in the format tcp://x.x.x.x:port")
    .def("send", &fr::events::transport::zmq_sender::send)
    ;

  pybind11::class_<fr::events::transport::zmq_receiver, std::shared_ptr<fr::events::transport::zmq_receiver>, fr::events::transport::receiver>(m, "zmq_receiver")
    .def(pybind11::init<>())
    .def("listen", &fr::events::transport::zmq_receiver::add, "Address in the format tcp://127.0.0.1:port")
    ;

  pybind11::class_<fr::events::event_buffer, std::shared_ptr<fr::events::event_buffer>>(m, "event_buffer")
    .def(pybind11::init<>())
    .def("subscribe_to", pybind11::overload_cast<std::shared_ptr<fr::events::transport::receiver>>(&fr::events::event_buffer::subscribe), "Subscribe to a receiver (like zmq_receiver)")
    .def("size", &fr::events::event_buffer::size, "Number of events in the buffer")
    .def("get", &fr::events::event_buffer::get, "Retrieve an event from the back of the buffer (Buffer is LIFO)")
    ;
  
}
