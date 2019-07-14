#ifndef EVENT__HANDLER_H_
#define EVENT_HANDLER_H_

namespace reactor {

/**
 * \brief Reactor event handler.
 */
class EventHandler {
 public:
  /**
   * \brief Destructor of EventHandler.
   */
  virtual ~EventHandler() = default;

  /**
   * \brief Read event handler.
   *
   * \param handle A file descriptor.
   *
   * This function is called by reactor whenever the file descriptor becomes readable.
   */
  virtual void handle_read(int handle) {}

  /**
   * \brief Write event handler.
   *
   * \param handle A file descriptor.
   *
   * This function is called by reactor whenever the file descriptor becomes writable.
   */
  virtual void handle_write(int handle) {}

    /**
   * \brief Exception event handler.
   *
   * \param handle A file descriptor.
   *
   * This function is called by reactor whenever an exception condition occurs on the file descriptor.
   */
  virtual void handle_exception(int handle) {}
};

} /* namespace reactor */

#endif  // event_handler_h
