/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <iostream>
#include <csignal>
#include <atomic>
#include "reactor/reactor.h"
#include "IPCServer.h"
#include "IPCClient.h"

static void setup_signal_handling() {
  sigset_t sigset;

  ::sigemptyset(&sigset);
  ::sigaddset(&sigset, SIGTERM);
  ::sigaddset(&sigset, SIGINT);

  pthread_sigmask(SIG_BLOCK, &sigset, NULL);
}

static void handle_signals() {
  sigset_t sigset;
  int signal;
  bool done;

  ::sigemptyset(&sigset);
  ::sigaddset(&sigset, SIGTERM);
  ::sigaddset(&sigset, SIGINT);

  done = false;

  while (!done) {
    sigwait(&sigset, &signal);

    std::cout << "Caught signal " << signal << '\n';

    switch (signal) {
      case SIGINT:
        done = true;
        break;
      case SIGTERM:
        done = true;
        break;
    }
  }
}

int main() {
  setup_signal_handling();

  reactor::Reactor reactor;
  std::atomic<bool> reactor_done(false);
  std::thread reactor_thread = std::thread([&reactor,&reactor_done]() {
      struct timeval timeout;
      timeout.tv_sec = 1;
      timeout.tv_usec = 0;
      while (!reactor_done) {
        reactor.handle_events(&timeout);
      }
    });

  // applications that register to the reactor
  ipcserver::IPCServer ipcserver(reactor, "./socket");
  ipcserver.start();
  // in this example the client does not register any handler to the reactor
  ipcclient::IPCClient ipcclient(reactor, "./socket");
  ipcclient.start();

  handle_signals();

  ipcserver.stop();
  ipcclient.stop();

  reactor_done = true;
  reactor.unblock();
  reactor_thread.join();

  return 0;
}
