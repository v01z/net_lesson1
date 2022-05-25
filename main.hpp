/* GNU General Public License v3.0 */
/******************************************************************************
*
* Name: main.hpp
* Main header file for lesson_1 project
*
* Copyright (C) 2022, binary_cat, v01z@mail.ru
*
*****************************************************************************/

#ifndef LESSON_1_MAIN_HPP
#define LESSON_1_MAIN_HPP

#include <iostream>

#ifdef _WIN32
#include <winsock2.h>
#include <Ws2ipdef.h> //Win: INET_ADDRESTRLEN
#include <ws2tcpip.h> //Win: inet_pton()
#pragma comment(lib, "Ws2_32.lib")
typedef SSIZE_T ssize_t;
#endif //_WIN32

#include <string.h> //strerror(), memset()
#include <errno.h> //errno

#ifdef __unix__
#include <unistd.h> //close()
#include <arpa/inet.h> //inet_ntop()
#include <netdb.h> //getnameinfo()
//This header has been already included by <arpa/inet.h> and <netdb.h>
//#include <netinet/in.h> //sockaddr_in

//This header has been already included by <netinet/in.h>
//#include <sys/socket.h> //socket()
#endif //__unix__

//-----------------------------------------------------------------------------

#define BUFF_SIZE 256
#define PORT_NUM 51511

#define EXIT_COMMAND "exit"

//-----------------------------------------------------------------------------

#endif //LESSON_1_MAIN_HPP
