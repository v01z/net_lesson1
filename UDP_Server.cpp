/* GNU General Public License v3.0 */
/******************************************************************************
*
* Name: UDP_Server.cpp
* UDP_Server class source code for lesson_1 project
*
* Copyright (C) 2022, binary_cat, v01z@mail.ru
*
*****************************************************************************/

#include "UDP_Server.hpp"

//-----------------------------------------------------------------------------

UDP_Server::UDP_Server(int port): _addr{}, _socket{ -1 }
{
    _addr.sin_family = AF_INET;
    _addr.sin_port = htons(port);
    _addr.sin_addr.s_addr = INADDR_ANY;
}

//-----------------------------------------------------------------------------

UDP_Server* UDP_Server::getInstance(const int port) {

    static UDP_Server instance { port };

    return &instance;
}

//-----------------------------------------------------------------------------

void UDP_Server::closeSocket() {
#ifdef _WIN32
    closesocket(_socket);
#else
    close(_socket);
#endif
}

//-----------------------------------------------------------------------------

bool UDP_Server::init() {

    _socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if ( -1 == _socket)
    {
        std::cerr << "Socket creation error: " << strerror(errno) << std::endl;
        return false;
    }

    if (bind(_socket, reinterpret_cast<sockaddr*>(&_addr),
    sizeof(_addr)) != 0)
    {
        closeSocket();
        std::cerr << "Socket binding error: " << strerror(errno) << std::endl;
        return false;
    }

    return true;
}

//-----------------------------------------------------------------------------

void UDP_Server::sendResponseToClient(const char *message, struct sockaddr_in *clientAddress) {
    size_t msgLength = strlen(message);

    if (sendto(_socket, message, msgLength, 0,
            reinterpret_cast<sockaddr*>(clientAddress),
               sizeof(sockaddr_in)) != msgLength)
    {
        std::cerr << "Can't send response to a client's message.\n" <<
            strerror(errno) << std::endl;
        closeSocket();
        exit(EXIT_FAILURE);
    }

    std::cout << "A response to client's message has been sent:\n";
    std::cout << message << std::endl;
}

//-----------------------------------------------------------------------------

void UDP_Server::loop() {
    if(!init()){

        std::cerr << "Server initialization failed.\n";
        return;
    }

    char buff[BUFF_SIZE] {};
    char clientAddrStr[INET_ADDRSTRLEN] {};

    struct sockaddr_in clientAddr;
    socklen_t clientAddrLen = sizeof(sockaddr_in);

    while (true){
        memset(&clientAddr, 0, sizeof(sockaddr_in));

        ssize_t bytesReceived = recvfrom(_socket, buff, BUFF_SIZE, 0,
                            reinterpret_cast<sockaddr*>(&clientAddr), &clientAddrLen);

        if (bytesReceived == -1){
            std::cerr << "recvfrom() error: " << strerror(errno) << std::endl;
            continue;
        }
        else if (bytesReceived > 0){
            buff[bytesReceived] = '\0';

            if (!strcmp(EXIT_COMMAND, buff))
            {
                sendResponseToClient("Server has been shutdown.\n", &clientAddr);
                closeSocket();
                std::cout << "Received exit command.\n";
                break;
            }
        }

        std::cout << "Server received " << bytesReceived << " bytes:\n" <<
            "\'" << buff << "\'\n";

        char hostName[256];

        int err_retval = getnameinfo(reinterpret_cast<sockaddr *>(&clientAddr),
                                     sizeof(clientAddr), hostName, sizeof(hostName),
                                     NULL, 0, 0);

        if (!err_retval)
            std::cout << "from " << hostName;
        else
            std::cerr << "Couldn't get hostname: " << gai_strerror(err_retval) << std::endl;

        if (inet_ntop(AF_INET, &clientAddr.sin_addr, clientAddrStr,
                      INET_ADDRSTRLEN) != NULL)
        {
            std::cout << " (" << clientAddrStr << ":"
            << ntohs(clientAddr.sin_port) << ')' << std::endl;
        }
        else
            std::cerr << "Couldn't convert network view of IP-address and port number " <<
                "to a presentation view: " << strerror(errno) << std::endl;

        sendResponseToClient(buff, &clientAddr);
    }
}

//-----------------------------------------------------------------------------
