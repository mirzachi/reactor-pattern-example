/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "reactor/reactor.h"

#include <unistd.h>
#include <iostream>
#include <stdexcept>

namespace reactor {

Reactor::Reactor() : dispatching_(false) {
  /* Create a pipe for unblocking the select system call */
  int ret = pipe(wakeup_pipe_.data());
  if (ret != 0) {
    std::runtime_error("pipe");
  }
}

Reactor::~Reactor() {
  unblock();
  // TODO: Verify that no method of Reactor is still running when the object is destroyed.
  ::close(wakeup_pipe_[0]);
  ::close(wakeup_pipe_[1]);
}

void Reactor::register_event_handler(int handle, EventHandler* event_handler, unsigned int event_type_mask,
                                     bool one_shot) {
  if (event_type_mask & EventType::kReadEvent) {
    read_event_handlers_.insert(std::pair<int, EventHandlerMapEntry>(handle, {event_handler, one_shot}));
  }
  if (event_type_mask & EventType::kWriteEvent) {
    write_event_handlers_.insert(std::pair<int, EventHandlerMapEntry>(handle, {event_handler, one_shot}));
  }
  if (event_type_mask & EventType::kExceptionEvent) {
    exception_event_handlers_.insert(std::pair<int, EventHandlerMapEntry>(handle, {event_handler, one_shot}));
  }
  if (!dispatching_) {
    send_wakeup();
  }
}

void Reactor::unregister_event_handler(int handle, unsigned int event_type_mask) {
  size_t count = 0;
  if (event_type_mask & EventType::kReadEvent) {
    count += read_event_handlers_.erase(handle);
  }
  if (event_type_mask & EventType::kWriteEvent) {
    count += write_event_handlers_.erase(handle);
  }
  if (event_type_mask & EventType::kExceptionEvent) {
    count += exception_event_handlers_.erase(handle);
  }
  if (count > 0 && !dispatching_) {
    send_wakeup();
  }
}

void Reactor::handle_events(const struct timeval* timeout) {
  struct timeval tv;
  struct timeval* tv_ptr = nullptr;
  if (timeout != nullptr) {
    // select() on Linux modifies the object under tv_ptr, therefore we copy the passed data.
    tv = *timeout;
    tv_ptr = &tv;
  }

  int maxfd = setup_fd_sets();

  int count = select(maxfd + 1, &rfds, &wfds, &efds, tv_ptr);
  if (count > 0) {
    dispatch_event_handlers();
  }
}

void Reactor::unblock() { send_wakeup(); }

int Reactor::setup_fd_sets() {
  FD_ZERO(&rfds);
  FD_ZERO(&wfds);
  FD_ZERO(&efds);

  FD_SET(wakeup_pipe_[0], &rfds);
  int maxfd = wakeup_pipe_[0];

  for (const auto& p : read_event_handlers_) {
    FD_SET(p.first, &rfds);
    maxfd = std::max(maxfd, p.first);
  }

  for (const auto& p : write_event_handlers_) {
    FD_SET(p.first, &wfds);
    maxfd = std::max(maxfd, p.first);
  }

  for (const auto& p : exception_event_handlers_) {
    FD_SET(p.first, &efds);
    maxfd = std::max(maxfd, p.first);
  }

  return maxfd;
}

void Reactor::dispatch_event_handlers() {
  dispatching_ = true;

  if (FD_ISSET(wakeup_pipe_[0], &rfds)) {
    handle_wakeup();
  }

  for (auto it = read_event_handlers_.begin(); it != read_event_handlers_.end();) {
    int handle = it->first;
    auto entry = it->second;
    if (FD_ISSET(handle, &rfds)) {
      std::cout << log << ": " << "Dispatching read event" << std::endl;
      if (entry.one_shot_) {
        it = read_event_handlers_.erase(it);
      } else {
        ++it;
      }
      entry.event_handler_->handle_read(handle);
    } else {
      ++it;
    }
  }

  for (auto it = write_event_handlers_.begin(); it != write_event_handlers_.end();) {
    int handle = it->first;
    auto entry = it->second;
    if (FD_ISSET(handle, &wfds)) {
      std::cout << log << ": " << "Dispatching write event" << std::endl;
      if (entry.one_shot_) {
        it = write_event_handlers_.erase(it);
      } else {
        ++it;
      }
      entry.event_handler_->handle_write(handle);
    } else {
      ++it;
    }
  }

  for (auto it = exception_event_handlers_.begin(); it != exception_event_handlers_.end();) {
    int handle = it->first;
    auto entry = it->second;
    if (FD_ISSET(handle, &efds)) {
      std::cout << log << ": " << "Dispatching exception event" << std::endl;
      if (entry.one_shot_) {
        it = exception_event_handlers_.erase(it);
      } else {
        ++it;
      }
      entry.event_handler_->handle_exception(handle);
    } else {
      ++it;
    }
  }

  dispatching_ = false;
}

void Reactor::send_wakeup() {
  int ret;
  char dummy;

  ret = ::write(wakeup_pipe_[1], &dummy, sizeof(dummy));
  if (ret != sizeof(dummy)) {
    std::runtime_error("write");
  }
}

void Reactor::handle_wakeup() {
  int ret;
  char dummy;

  ret = ::read(wakeup_pipe_[0], &dummy, sizeof(dummy));
  if (ret != sizeof(dummy)) {
    std::runtime_error("read");
  }
}

}  // namespace reactor
