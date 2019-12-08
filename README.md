# reactor-pattern-example
An implementation of a reactor pattern with the accompanying IPC server/client example use case.

## PROBLEM

On UNIX based systems the *accept()* system call is used to accept a new connection on a listening socket. If no pending connections are present in the queue, and the socket is not marked as nonblocking, accept() blocks the caller until a connection is present. If the socket is marked nonblocking and no pending connections are present in the queue, accept() fails and returns some pre-defined errors. 

## REACTOR APPROACH

In order to circumvent blocking and avoid using polling, a reactor pattern can be utilized. This approach utilizes the reactor pattern introduced by Douglas Schmidt in his [Reactor paper](https://pdfs.semanticscholar.org/3d9f/fc7669ab488ea74841181e9b1be9d10d5cea.pdf?_ga=2.259182030.574174400.1563114230-2125772795.1563114230). The Reactor approach enables event-driven handling of incoming connections, which allows a server to do something else while waiting for a new connection. When a new connection request arrives, the reactor is the entity that will notify the server about the new incoming connection. In this way, invoking *accept()* by the server will not cause any blocking, because the waiting connection queue will not be empty. 

## IPC use case

This example comes with the full implementation of the reactor pattern in C++. It is runnable only on systems that come with a *select()* system call, which allows a program to monitor multiple file descriptors, waiting until one or more of the file descriptors become "ready" for some class of I/O operation. In this particular example, the IPCServer registers a read handler within the reactor on a UNIX domain socket (socket file). The same socket file is used by the IPCClient to establish an inter-process communication with the IPCServer. Whenever the IPCClient sends some data using this socket file, *select()* will mark this file descriptor as "ready" to read, and the reactor will dispatch this information to the appropriate registered handler (*handle_read* by IPCServer). In this way, the IPCServer can do something else and switch to processing of incoming connections only when it is informed by the reactor that they are present in the waiting queue. Hence, *accept()* will never  block.

The provided example is very basic and serves only for understanding the underlying mechanisms of a reactor pattern.
