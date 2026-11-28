//tests using internal loopback address 127.0.0.1

#include "../shared/net-platform.h"

#include <iostream>
#include <cstring>
#include <thread>

int base_TCP_test() {
    platform_init();

    // 1. Create server socket
    Socket server = tcp_create();
    if (!valid_socket(server)) {
        std::cerr << "Server socket creation failed\n";
        return 1;
    }

    // 2. Bind server to localhost port 5005
    if (!bind_port(server, 5005)) {
        std::cerr << "Bind failed\n";
        close_socket(server);
        return 1;
    }

    // 3. Start listening
    if (!listen_on(server, 5)) {
        std::cerr << "Listen failed\n";
        close_socket(server);
        return 1;
    }

    std::cout << "Server listening on port 5005\n";

    // 4. Create client socket
    Socket client = tcp_create();
    if (!valid_socket(client)) {
        std::cerr << "Client socket creation failed\n";
        close_socket(server);
        return 1;
    }

    // 5. Connect client to server
    Address destination;
    destination.ip = 0x7F000001; // 127.0.0.1
    destination.port = 5005;

    IoResult connected = connect_to(client, destination);
    if (connected.result != Result::Ok) {
        std::cerr << "Connection failed\n";
        close_socket(client);
        close_socket(server);
        return 1;
    }

    std::cout << "Client connected\n";

    // 6. Accept incoming connection
    Address peer;
    Result res;

    Socket accepted = accept_client(server, &peer, &res);

    if (!valid_socket(accepted)) {
        std::cerr << "Accept failed\n";
        close_socket(client);
        close_socket(server);
        return 1;
    }

    std::cout << "Accepted connection from "
              << peer.to_string() << "\n";

    // 7. Send message from client
    const char* message = "Hello from client!";
    IoResult sent = tcp_send(client, message, strlen(message));

    if (sent.result != Result::Ok) {
        std::cerr << "Send failed\n";
        return 1;
    }

    std::cout << "Sent " << sent.bytes << " bytes\n";

    // 8. Receive message on accepted socket
    char buffer[128] = {};
    IoResult received = tcp_recv(accepted, buffer, sizeof(buffer));

    if (received.result != Result::Ok) {
        std::cerr << "Receive failed\n";
        return 1;
    }

    std::cout << "Received: " << buffer << "\n";

    // 9. Cleanup
    close_socket(accepted);
    close_socket(client);
    close_socket(server);

    platform_shutdown();

    std::cout << "Test completed successfully\n";
    return 0;
}

