#include <iostream>
#include <vector>
#include <string>
#include <sys/socket.h>
#include <Player.h>
// Assuming you have a Message struct defined
struct Message {
    std::string player_id;
    std::string role;
    std::string type;
    float x;
    float y;
};

// Function to receive a message from a client
void receive_message(int client_fd, Message& received_message) {
    char buffer[1024]; // Adjust the buffer size as needed
    memset(buffer, 0, sizeof(buffer));

    // Receive the message from the client
    ssize_t bytes_received = recv(client_fd, buffer, sizeof(buffer), 0);

    if (bytes_received <= 0) {
        // Handle disconnection or error
        std::cerr << "Error receiving message or client disconnected." << std::endl;
        // You might want to close the client connection or handle the disconnect gracefully
    } else {
        // Assuming the received message is in JSON format
        // You would typically parse the received data according to your protocol
        parse_json_message(buffer, received_message);
    }
}

// Example function to parse a JSON message (you might use a library like cJSON for a more robust solution)
void parse_json_message(const char* json_str, Message& message) {
    // Implement your JSON parsing logic here
    // This is a simplified example assuming the structure is well-formed

    // Example: Parse JSON fields
    sscanf(json_str, "message { player_id: %s role: %s type: %s x: %f y: %f }",
           &message.player_id[0], &message.role[0], &message.type[0], &message.x, &message.y);
}

void handle_client_message(int client_fd, std::vector<Player>& players, std::vector<Player>& observers) {
    // Assuming you have a function to receive the message from the client
    Message received_message;
    receive_message(client_fd, received_message);

    // Process the received message based on its type
    if (received_message.type == "position") {
        // Update the player or observer position based on the role
        if (received_message.role == "player") {
            update_player_position(received_message.player_id, received_message.x, received_message.y, players);
        }
    } else if (received_message.type == "control") {
        // Process control message (e.g., button presses)
        process_control_message(received_message.player_id, received_message);
    } else if (received_message.type == "disconnect") {
        // Handle client disconnect
        handle_client_disconnect(received_message.player_id, players, observers);
    } else {
        // Handle unknown message type
        std::cerr << "Unknown message type: " << received_message.type << std::endl;
    }
}

// Example functions for updating player and observer positions, processing control messages, and handling disconnect
void update_player_position(const std::string& player_id, float x, float y, std::vector<Player>& players) {
    // Find and update the player position based on the player_id
    for (auto& player : players) {
        if (player.get_id() == player_id) {
            player.ApplyForce(x, y);
            break;
        }
    }
}


void process_control_message(const std::string& player_id, const Message& control_message) {
    // Process the control message (e.g., handle button presses)
    // You may implement logic based on the control_message data
    // ...
}

void handle_client_disconnect(const std::string& disconnected_player_id, std::vector<Player>& players, std::vector<Player>& observers) {
    // Handle the case where a player disconnects
    // Remove the player from the players vector
    for (auto it = players.begin(); it != players.end(); ++it) {
        if (it->get_id() == disconnected_player_id) {
            players.erase(it);
            break;
        }
    }

    // Handle the case where an observer disconnects
    // Remove the observer from the observers vector
    for (auto it = observers.begin(); it != observers.end(); ++it) {
        if (it->get_id() == disconnected_player_id) {
            observers.erase(it);
            break;
        }
    }

    // Handle any additional cleanup or logic
}
