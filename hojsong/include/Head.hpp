#ifndef HAED_HPP
#define HEAD_HPP

#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <sys/errno.h>
#include <arpa/inet.h>
#include <sys/select.h>
#include <poll.h>
#include <sys/event.h>
#include <sys/time.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/wait.h>
#include <dirent.h>

#include <stdexcept>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <deque>
#include <list>

#include "Member.hpp"
#include "MemberRepository.hpp"

#define PORT        3550
#define PACKET_SIZE 1024
#endif