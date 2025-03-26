// Copyright 2025 Stephen Spear
#include <bool_expr_client.h>
#include <domain_socket.h>
#include <iostream>
#include <vector>
#include <cstddef>
#include <cstdlib>
#include <string>

void BoolExprClient::Run() {
    std::cout << "BoolExprClient initializing..." << std::endl;
    if (!Init()) {
        exit(1);
    }
    std::cout << "BoolExprClient connecting..." << std::endl;

    std::string init_message;
    ::ssize_t bytes_read = Read(static_cast<size_t>(2), &init_message);
    if (bytes_read <= 0) {
        std::cerr << "BoolExprClient: Failed to receive initialization "
                     "message.\n";
        exit(3);
    }

    US_ = init_message[0];
    EOT_ = init_message[1];

    std::string message;
    for (const auto& val : truth_values_) {
        message += val + US_;
    }
    message.back() = EOT_;

    ::ssize_t bytes_wrote = Write(message, EOT_);
    if (bytes_wrote < 0) {
        std::cerr << "BoolExprClient terminating...\n";
        exit(4);
    } else if (bytes_wrote == 0) {
        std::cerr << "Server disconnected\n";
        exit(5);
    }

    std::string response;
    ::ssize_t bytes_read_temp = Read(US_, &response);
    bytes_read += bytes_read_temp;
    response.erase(
        std::remove(response.begin(), response.end(), EOT_),
        response.end());

    if (bytes_read_temp > 0) {
        std::cout << "Finished with " << static_cast<int>(bytes_read)
                  << "B received, " << static_cast<int>(bytes_wrote)
                  << "B sent" << std::endl;
        std::cout << "Results\n" << response << "\n";
    } else {
        std::cerr << "Error receiving data\n";
    }
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        std::cerr << "Usage: ./bool_expr_client <socket_name> "
                     "<T/F values>\n";
        return 1;
    }

    const char* socket_name = argv[1];
    std::vector<std::string> truth_values(argv + 2, argv + argc);

    BoolExprClient client(socket_name, truth_values);
    client.Run();

    return 0;
}
