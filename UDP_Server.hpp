/* GNU General Public License v3.0 */
/******************************************************************************
*
* Name: UDP_Server.hpp
* UDP_Server class header for lesson_1 project
*
* Copyright (C) 2022, binary_cat, v01z@mail.ru
*
*****************************************************************************/

#ifndef LESSON_1_UDP_SERVER_HPP
#define LESSON_1_UDP_SERVER_HPP

#include "main.hpp"

//-----------------------------------------------------------------------------

class UDP_Server {
private:
    struct sockaddr_in _addr;
    int _socket;

    bool init();
    void sendResponseToClient(const char*, struct sockaddr_in*);
    void closeSocket();
    ~UDP_Server() = default;

protected:
    UDP_Server(const int);

public:
    UDP_Server() = delete;
    UDP_Server(const UDP_Server&) = delete;
    void operator=(const UDP_Server&) = delete;
    static UDP_Server* getInstance(const int);
    void loop();
};

//-----------------------------------------------------------------------------

#endif //LESSON_1_UDP_SERVER_HPP
