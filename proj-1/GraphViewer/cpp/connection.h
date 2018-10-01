#ifndef _CONNECTION_
#define _CONNECTION_

#include <cstdio>
#include <cstdlib>
#include <cstring>

#ifdef linux
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#else
#include <winsock2.h>
#endif

#include <string>
#include <iostream>

class Connection {
 public:
  Connection(short port);

  bool sendMsg(std::string msg);
  std::string readLine();
 private: 
#ifdef linux
  int sock;
#else
  SOCKET sock;
#endif
};

#endif
