#ifndef REACTOR_H_
#define REACTOR_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <sys/select.h>

#include <atomic> 
#include <array>
#include <map>

#include "reactor/event_handler.h"

namespace reactor {

/**
 * \brief Reactor event type.
 */
enum EventType {
  kReadEvent = 1,
  kWriteEvent = 2,
  kExceptionEvent = 3
};

/**
 * \brief Reactor.
 *
 * An implementation of the reactor design pattern.
 */
class Reactor {
 public:
  /**
   * \brief Constructor of Reactor.
   */
  Reactor();
  /**
   * \brief Destructor of Reactor.
   */
  ~Reactor();
  /**
   * \brief Registers an event handler for a file descriptor.
   *
   * \param handle A file descriptor.
   * \param event_handler A pointer to an event handler.
   * \param event_type_mask A mask of event types for which the passed event handler should be registered.
   * \param one_shot Specifies if the event handler should be automatically unregistered after the first event
   * occurrence.
   */
  void register_event_handler(int handle, EventHandler* event_handler, unsigned int event_type_mask,
                              bool one_shot = false);
  /**
   * \brief Unregisters a previously registered event handler for a file descriptor.
   *
   * \param handle A file descriptor.
   * \param event_type_mask A mask of event types for which the event handler should be unregistered.
   */
  void unregister_event_handler(int handle, unsigned int event_type_mask);
  /**
   * \brief Lets the reactor check whether some events are pending on any registered file descriptors and dispatch
   *        the corresponding event handlers.
   *
   * \param timeout A pointer to a time value to wait for events. In case of the null pointer the reactor
   *                will not block waiting for events.
   */
  void handle_events(const struct timeval* timeout = nullptr);
  /**
   * \brief Causes the reactor to return from the blocking call of the function handle_events.
   */
  void unblock();

 private:
  /* Log context */
  const std::string log = "Reactor";
  /**
   * \brief Structure for storing registered event handlers.
   */
  struct EventHandlerMapEntry {
    EventHandler* event_handler_;
    bool one_shot_;
  };
  /**
   * \brief Notifies registered event handlers for which events are available.
   */
  void dispatch_event_handlers();
  /**
   * \brief Sets up file descriptor sets required for the select system call.
   */
  int setup_fd_sets();
  /**
   * \brief Sends wakeup signal to unblocks the select system call being executed in the function handle_events.
   */
  void send_wakeup();
  /**
   * \brief Handles wakeup signal sent by the function send_wakeup.
   */
  void handle_wakeup();
  /**
   * \brief Pipe used for unblocking a blocked select call.
   */
  std::array<int, 2> wakeup_pipe_;
  /**
   * \brief Indicates whether the reactor is currently dispatching occurred events.
   */
  std::atomic_bool dispatching_;
  /**
   * \brief File descriptor sets required for the select system call.
   */
  fd_set rfds, wfds, efds;
  /**
   * \brief Event handlers for read events.
   */
  std::map<int, EventHandlerMapEntry> read_event_handlers_;
  /**
   * \brief Event handlers for write events.
   */
  std::map<int, EventHandlerMapEntry> write_event_handlers_;
  /**
   * \brief Event handlers for exception events.
   */
  std::map<int, EventHandlerMapEntry> exception_event_handlers_;
};

}  // namespace reactor

#endif  // REACTOR_H_
