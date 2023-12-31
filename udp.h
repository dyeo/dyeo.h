/*
# udp.h

## Description

This header provides a simple interface for UDP socket operations. The code uses
preprocessor directives to differentiate between Windows and non-Windows
platforms.

## API Documentation

### Structures

  - `udp_t`: Represents a UDP socket with fields for IP, port, and socket
handle.

### Macros

  - `UDP_BUFFER_LENGTH`: Default length of the buffer used to read from a UDP
socket.
  - `BYTE`: Defines a byte as an unsigned char.
  - `_UDP_C`: Guards the implementation.
  - `_WINSOCK_DEPRECATED_NO_WARNINGS`: For suppressing deprecated winsock
warnings on Windows.
  - `UDP_INVALID`, `UDP_ERROR`, `UDP_PROTOOL`: Platform-specific error constants
and protocol number.

### Function Prototypes

  - `udp_init()`: Initializes necessary data structures or libraries for UDP
operations.
  - `udp_cleanup()`: Cleans up any initialized data or libraries.
  - `udp_open()`: Opens and initializes a UDP socket.
  - `udp_send()`: Sends data over a given UDP socket.
  - `udp_bind()`: Binds a given UDP socket for listening.
  - `udp_recv()`: Receives data from a given UDP socket.
  - `udp_close()`: Closes a UDP socket.

### Initialization & Cleanup

  - `udp_init()`: For Windows, it initializes Winsock. For non-Windows, it
simply allocates a structure.
  - `udp_cleanup()`: Cleans up the previously initialized data. For Windows, it
also cleans up the Winsock library.

### Socket Operations

  - `udp_open()`: Initializes a UDP socket and binds it to the given IP and
port.
  - `udp_close()`: Closes a UDP socket and frees the associated memory.
  - `udp_send()`: Sends data to the specified IP and port using the socket. Uses
`sendto()` which is a standard socket API function for sending data over a UDP
socket.
  - `udp_recv()`: Receives data using the socket. Uses `recvfrom()`, a standard
socket API function for receiving data from a UDP socket.

## Note

- The code is cross-platform with differentiation for Windows using the `_WIN32`
preprocessor directive.
- Error handling is done using standard library functions and some error
checking, e.g., for memory allocation failures.
- The code uses `inet_pton()` for address resolution which is a more modern
function than the older `inet_addr()`.
*/
#ifndef _UDP_H
#define _UDP_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>

#if defined(_WIN32) || defined(_WIN64)
#include <winsock2.h>
#endif

#ifndef UDP_BUFFER_LENGTH
#define UDP_BUFFER_LENGTH 1024
#endif

typedef struct udp_t
{
  const char *ip;
  unsigned short port;
#ifdef _WIN32
  SOCKET handle;
#else
  int handle;
#endif
} *udp_t;

#ifndef BYTE
#define BYTE unsigned char
#endif

extern bool udp_init();
extern bool udp_cleanup();
extern udp_t udp_open(const char *ip, unsigned short port);
#define udp_bind(sock, ip, port) ((sock) = udp_bindf(sock, ip, port))
extern bool udp_send(udp_t sock, const BYTE *data, const size_t length);
extern bool udp_recv(udp_t sock, BYTE **outData, size_t *outLength);
extern bool udp_close(udp_t sock);

extern udp_t udp_bindf(udp_t sock, const char *ip, unsigned short port);

#ifdef __cplusplus
}
#endif

#endif

#ifndef UDP_IMPLEMENTATION
#ifndef _UDP_C
#define _UDP_C

#ifdef _WIN32
#define _WINSOCK_DEPRECATED_NO_WARNINGS 1
#include <winsock2.h>
#include <ws2tcpip.h>
//
#define UDP_INVALID ((SOCKET) ~0)
#define UDP_ERROR -1
#define UDP_PROTOOL IPPROTO_UDP
//
#define strdup _strdup
//
static WSADATA *_udp_handle;
#else
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
//
#define UDP_INVALID -1
#define UDP_ERROR -1
#define UDP_PROTOOL 0
//
#define closesocket close
typedef struct
{
  void *_;
} UDPDATA;
static UDPDATA *_udp_handle;
#endif

#include <stdio.h>
#include <string.h>

bool udp_init()
{
  if (_udp_handle == NULL)
  {
#ifdef _WIN32
    _udp_handle = (WSADATA *) malloc(sizeof(WSADATA));
    return WSAStartup(MAKEWORD(2, 2), _udp_handle) == 0;
#else
    _udp_handle = (UDPDATA *) malloc(sizeof(UDPDATA));
    return true;
#endif
  }
  return false;
}

bool udp_cleanup()
{
  if (_udp_handle != NULL)
  {
    bool result = true;
#ifdef _WIN32
    result = WSACleanup() == 0;
#endif
    free(_udp_handle);
    return result;
  }
  return false;
}

udp_t udp_open(const char *ip, unsigned short port)
{
  udp_t sock = (udp_t) malloc(sizeof(udp_t));
  if (!sock)
  {
    fprintf(stderr, "ERROR: Memory allocation failure udp_t");
    return NULL;
  }
  sock->ip     = strdup(ip);
  sock->port   = port;
  sock->handle = socket(AF_INET, SOCK_DGRAM, UDP_PROTOOL);
  if (sock->handle == UDP_INVALID)
  {
    free(sock);
    return NULL;
  }
  return sock;
}

udp_t udp_bindf(udp_t sock, const char *ip, unsigned short port)
{
  if (sock == NULL)
  {
    sock = udp_open(ip, port);
  }
  struct sockaddr_in addr;

  memset(&addr, 0, sizeof(addr));
  if (inet_pton(AF_INET, ip, &addr.sin_addr) != 1)
  {
    fprintf(stderr, "ERROR: Could not resolve address");
    free(sock);
    return NULL;
  }

  if (bind(sock->handle, (struct sockaddr *) &addr, sizeof(addr)) == UDP_ERROR)
  {
    fprintf(stderr, "ERROR: Could not bind socket");
    free(sock);
    return NULL;
  }

  return sock;
}

bool udp_close(udp_t sock)
{
  if (!sock)
  {
    return false;
  }
  closesocket(sock->handle);
  free((char *) sock->ip);
  free(sock);
  return true;
}

bool udp_send(udp_t sock, const BYTE *data, const size_t length)
{
  struct sockaddr_in dest;
  if (inet_pton(AF_INET, sock->ip, &(dest.sin_addr)) != 1)
  {
    fprintf(stderr, "ERROR: Could not resolve address");
    return false;
  }
  int result = sendto(sock->handle, (char *) data, length, 0,
                      (struct sockaddr *) &dest, sizeof(dest));
  return result != UDP_ERROR;
}

bool udp_recv(udp_t sock, BYTE **outData, size_t *outLength)
{
  struct sockaddr_in sender;
  int senderLen = sizeof(sender);
  BYTE buffer[UDP_BUFFER_LENGTH];
  int bytesReceived = recvfrom(sock->handle, (char *) buffer, *outLength, 0,
                               (struct sockaddr *) &sender, &senderLen);
  if (bytesReceived <= 0)
  {
    return false;
  }
  BYTE *newData = (BYTE *) realloc(*outData, bytesReceived);
  if (!newData)
  {
    fprintf(stderr, "ERROR: Memory allocation failure");
    return false;
  }
  *outData = newData;
  return true;
}

#endif
#endif
/*
This software is served under two licenses - pick which you prefer.
--------------------------------------------------------------------------------
MIT License
Copyright 2023 Dani Yeomans

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
--------------------------------------------------------------------------------
Public Domain
Dedicated 2023 Dani Yeomans

The author of this software and associated documentation files (the "Software")
dedicates any and all copyright interest in the Software to the public domain.
The author makes this dedication for the benefit of the public at large and to
the detriment of the author's heirs and successors. The author intends this
dedication to be an overt act of relinquishment in perpetuity of all present and
future rights to the Software under copyright law.

Any person obtaining a copy of the Software and associated documentation files
is free to to deal in the Software without restriction, including without
limitation the rights to use, copy, modify, merge, publish, distribute,
sublicense, and/or sell copies of the Software, and to permit persons to whom
the Software is furnished to do so, subject to no conditions.
--------------------------------------------------------------------------------
*/
