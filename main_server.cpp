/* GNU General Public License v3.0 */
/******************************************************************************
*
* Name: main_server.cpp
* Server source code for lesson_1 project
*
* Copyright (C) 2022, binary_cat, v01z@mail.ru
*
*****************************************************************************/

#include "main.hpp"
#include "UDP_Server.hpp"

//-----------------------------------------------------------------------------

int main(){

#ifdef _WIN32
    WSADATA ws;
    if (WSAStartup(MAKEWORD(2, 2), &ws) != 0)
    {
        std::cerr << "Failed to initialize WinSock. Error Code: %d\n", WSAGetLastError());
        return 1;
    }
#endif //_WIN32

    UDP_Server *udpServer = UDP_Server::getInstance(PORT_NUM);
    udpServer->loop();

    std::cout << "The server has been shutdown." << std::endl;

#ifdef _WIN32
    WSACleanup();
#endif

    return EXIT_SUCCESS;
}

//-----------------------------------------------------------------------------
