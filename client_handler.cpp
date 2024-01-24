#include "client_handler.h"


int Handler::connectToServer(char* ip, char* port){
    // Resolve arguments to IPv4 address with a port number
    addrinfo *resolved, hints = {.ai_flags = 0, .ai_family = AF_INET, .ai_socktype = SOCK_STREAM};
    int res = getaddrinfo(ip, port, &hints, &resolved);
    if (res || !resolved)
        error(1, 0, "getaddrinfo: %s", gai_strerror(res));

    // create socket
    prime_sock = socket(resolved->ai_family, resolved->ai_socktype, 0);
    if (prime_sock == -1)
        error(1, errno, "socket failed");

    // attempt to connect
    res = connect(prime_sock, resolved->ai_addr, resolved->ai_addrlen);
    if (res<0)
        error(1, errno, "connect failed");

    // free memory
    freeaddrinfo(resolved);
    // std::thread t(receiver, sock);
    return prime_sock;
}
int Handler::getRoomsInfo(){
    char buffer[256];
    std::strcpy(buffer,"getrooms");
    write(prime_sock,buffer,sizeof(buffer));
    std::size_t n;
    ssize_t count = recv(prime_sock, &n, sizeof(n),MSG_WAITALL);
    if (count < 0) 
        perror("read failed");
    std::cout<<n<<std::endl;
    count = recv(prime_sock, buffer, sizeof(buffer),MSG_WAITALL);
    if (count < 0) {
        perror("read failed");
        // Handle error
    } else if (count == 0) {
        // Connection closed
        printf("Connection closed by server\n");
    } else {
        // Convert the received data back to a string
        // std::string receivedString(buffer, static_cast<size_t>(count));
        printf("Available rooms:\n %s\n", buffer);
    }
    std::cout<<"Select room by number (0 to create room):";
    int t=0;
    std::cin>>t;
    return t;
}
