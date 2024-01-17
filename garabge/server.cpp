#include <iostream>
#include <vector>
#include <thread>
#include <chrono>
#include <cstdlib>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>

#include "Player.h"
#include "Ball.h"
#include "Gate.h"

// Constants
const int MAX_PLAYERS = 4;
const int MAX_OBSERVERS = 6;
const int PORT = 12345;

// Game state variables
std::vector<Player> players;
std::vector<Player> observers;
Ball ball;
Gate left_gate({20, 250});
Gate right_gate({980, 250});
int score_left = 0;
int score_right = 0;
bool game_over = false;

// Socket variables
int server_socket;
fd_set active_sockets;
int max_socket;

// Function declarations
void initialize_game();
void update_game();
void handle_client(int client_socket);
void send_game_state(int client_socket);
void broadcast_game_state();

int main()
{
    // Initialize the game and the server socket
    initialize_game();

    // Wait for clients to connect and handle their requests in separate threads
    while (!game_over)
    {
        // Wait for a new client connection
        fd_set read_sockets = active_sockets;
        if (select(max_socket + 1, &read_sockets, NULL, NULL, NULL) < 0)
        {
            std::cerr << "Error in select()" << std::endl;
            continue;
        }
        if (FD_ISSET(server_socket, &read_sockets))
        {
            // Accept the new client connection
            sockaddr_in client_addr;
            socklen_t client_addr_len = sizeof(client_addr);
            int client_socket = accept(server_socket, (sockaddr*)&client_addr, &client_addr_len);
            if (client_socket < 0)
            {
                std::cerr << "Error in accept()" << std::endl;
                continue;
            }

            // Add the new client socket to the list of active sockets
            FD_SET(client_socket, &active_sockets);
            if (client_socket > max_socket)
                max_socket = client_socket;

            // Handle the client in a separate thread
            std::thread t(handle_client, client_socket);
            t.detach();
        }

        // Update the game state
        update_game();
        broadcast_game_state();
    }

    // Close the server socket
    close(server_socket);

    return 0;
}

void initialize_game()
{
    // Initialize the server socket
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0)
    {
        std::cerr << "Error creating server socket" << std::endl;
        std::exit(1);
    }

    // Bind the server socket to the port
    sockaddr_in server_addr;
    std::memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    if (bind(server_socket, (sockaddr*)&server_addr, sizeof(server_addr)) < 0)
    {
        std::cerr << "Error binding server socket to port " << PORT << std::endl;
        std::exit(1);
    }

    // Listen for incoming connections
    if (listen(server_socket, 5) < 0)
    {
        std::cerr << "Error listening on server socket" << std::endl;
        std::exit(1);// -------------
    }
}

// send game state to all clients
void send_game_state() {
    std::string state = create_game_state_message();
    for (auto& c : clients) {
        send_message(c.sockfd, state);
    }
}

// handle player messages
void handle_player_message(const std::string& message, int sockfd) {
    std::string type = get_message_type(message);
    if (type == "position") {
        // update player position
        std::string player_name = get_player_name_from_message(message);
        int x = get_player_x_position_from_message(message);
        int y = get_player_y_position_from_message(message);
        players[player_name].x = x;
        players[player_name].y = y;
    } else if (type == "button") {
        // handle button press
        std::string player_name = get_player_name_from_message(message);
        std::string button = get_button_from_message(message);
        if (button == "left") {
            players[player_name].vx = -PLAYER_SPEED;
        } else if (button == "right") {
            players[player_name].vx = PLAYER_SPEED;
        } else if (button == "up") {
            players[player_name].vy = -PLAYER_SPEED;
        } else if (button == "down") {
            players[player_name].vy = PLAYER_SPEED;
        } else if (button == "none") {
            players[player_name].vx = 0;
            players[player_name].vy = 0;
        }
    } else if (type == "exit") {
        // handle player exit
        std::string player_name = get_player_name_from_message(message);
        remove_player(player_name);
    }
}

// handle observer messages
void handle_observer_message(const std::string& message, int sockfd) {
    std::string type = get_message_type(message);
    if (type == "exit") {
        // handle observer exit
        remove_observer(sockfd);
    }
}

// handle incoming messages from clients
void handle_message(const std::string& message, int sockfd) {
    std::string role = get_client_role(sockfd);
    if (role == "player") {
        handle_player_message(message, sockfd);
    } else if (role == "observer") {
        handle_observer_message(message, sockfd);
    }
}

// main loop
void run() {
    while (true) {
        fd_set readfds;
        int max_fd = -1;
        FD_ZERO(&readfds);
        FD_SET(sockfd, &readfds);
        max_fd = sockfd;
        for (auto& c : clients) {
            FD_SET(c.sockfd, &readfds);
            max_fd = std::max(max_fd, c.sockfd);
        }
        int activity = select(max_fd + 1, &readfds, NULL, NULL, NULL);
        if (activity == -1) {
            // error in select
            perror("select failed");
            exit(EXIT_FAILURE);
        }
        if (FD_ISSET(sockfd, &readfds)) {
            // new client connection
            int client_sockfd = accept_connection(sockfd);
            add_client(client_sockfd);
        }
        for (auto& c : clients) {
            if (FD_ISSET(c.sockfd, &readfds)) {
                // client sent a message
                std::string message = receive_message(c.sockfd);
                if (message.empty()) {
                    // client disconnected
                    remove_client(c.sockfd);
                } else {
                    // handle message
                    handle_message(message, c.sockfd);
                }
            }
        }
        send_game_state();
    }
}