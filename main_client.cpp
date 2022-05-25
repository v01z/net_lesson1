/* GNU General Public License v3.0 */
/******************************************************************************
*
* Name: main_client.cpp
* Client source code for lesson_1 project
*
* Copyright (C) 2022, binary_cat, v01z@mail.ru
*
*****************************************************************************/

#include "main.hpp"

//-----------------------------------------------------------------------------

void closeSocket(int socket) {
#ifdef _WIN32
    closesocket(socket);
#else
    close(socket);
#endif
}

//-----------------------------------------------------------------------------

void udpClient(int argc, char *argv[]){
    if (argc < 3 || strcmp(argv[1], "--help") == 0)
    {
        std::cout << "Usage: " << argv[0] <<
        " host-address message\n";
        exit(EXIT_FAILURE);
    }

    int sockd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if ( -1 == sockd)
    {
        std::cerr << "Socket creation error: " << strerror(errno) << std::endl;
        exit (EXIT_FAILURE);
    }

    struct sockaddr_in serverAddr;
    memset(&serverAddr, 0, sizeof(sockaddr_in));

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT_NUM);

    if (inet_pton(AF_INET, argv[1], &serverAddr.sin_addr) <= 0)
    {
        std::cerr << "inet_pton() failed for address: " << argv[1] <<
        "\n" << strerror(errno) << std::endl;

        closeSocket(sockd);

        exit(EXIT_FAILURE);
    }

    std::string msgStr{};

    for (int i{ 2 }; i < argc; i++)
    {
        msgStr.append(argv[i]);
        if (!(i == argc - 1))
            msgStr.append(" ");
    }

    size_t msgLen { strlen(msgStr.c_str()) };

    if (msgLen < BUFF_SIZE - 1)
    {
        if (sendto(sockd, msgStr.c_str(), msgLen, 0,
                  reinterpret_cast<sockaddr*>(&serverAddr),
                    sizeof(sockaddr_in)) != msgLen)
        {
            std::cerr << "Can't send message \'" << msgStr <<
                "\' to a server \'" << argv[1] << "\'.\n" <<
                    strerror(errno) << std::endl;

        closeSocket(sockd);
        exit(EXIT_FAILURE);
        }
        else
            std::cout << "Message \'" << msgStr <<
                "\' has been sent to a server " << argv[1] << std::endl;
    }
    else
    {
        std::cerr << "U've entered too long message ( >= 256 ), sorry.\n";
        closeSocket(sockd);
        exit(EXIT_FAILURE);
    }

    char buff[BUFF_SIZE]{};
    socklen_t serverAddrLen = sizeof(sockaddr_in);

    if (recvfrom(sockd, buff, BUFF_SIZE, 0,
        reinterpret_cast<sockaddr*>(&serverAddr), &serverAddrLen) == -1)
    {
       std::cerr << "recvfrom() failed: " << strerror(errno) << std::endl;
       closeSocket(sockd);
       exit(EXIT_FAILURE);
    }

    std::cout << "Server returned:\n" << buff << std::endl;

    closeSocket(sockd);
}

//-----------------------------------------------------------------------------

int main(int argc, char *argv[]){
#ifdef _WIN32
    WSADATA ws;
    if (WSAStartup(MAKEWORD(2, 2), &ws) != 0)
    {
        std::cerr << "Failed to initialize WinSock. Error Code: %d\n", WSAGetLastError());
return 1;
    }
#endif //_WIN32

    udpClient(argc, argv);

#ifdef _WIN32
    WSACleanup();
#endif
    return EXIT_SUCCESS;
}

//-----------------------------------------------------------------------------
