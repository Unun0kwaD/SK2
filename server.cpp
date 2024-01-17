
#include "server.h"


int main() {
    // Create the server socket and bind it to a port
    int server_socket = create_server_socket(PORT_NUMBER);

    // Initialize the game state and player data structures
    GameState game_state;
    std::vector<Player> players(MAX_PLAYERS);
    std::vector<Observer> observers(MAX_OBSERVERS);

    // Run the game loop
    while (true) {
        // Accept new client connections
        int client_socket = accept_client_connection(server_socket, players, observers);

        // Handle client messages
        handle_client_message(client_socket, game_state, players, observers);

        // Remove disconnected players
        remove_disconnected_players(players, observers);

        // Update the game state
        update_game_state(game_state, players);

        // Send game state updates to all clients
        send_game_state_to_clients(game_state, players, observers);
    }

    // Close the server socket
    close_socket(server_socket);

    return 0;
}
