// Copyright 2025 Stephen Spear
#ifndef BOOL_EXPR_CLIENT_H
#define BOOL_EXPR_CLIENT_H

#include <domain_socket.h>
#include <vector>
#include <string>

class BoolExprClient : public DomainSocketClient {
 public:
    using ::DomainSocketClient::DomainSocketClient;
    BoolExprClient(const char* socket_path, const std::vector<std::string>&
                   truth_values)
        : DomainSocketClient(socket_path, false), truth_values_(truth_values) {}

    void Run();

 private:
    std::vector<std::string> truth_values_;
    char US_;
    char EOT_;
};

#endif  // BOOL_EXPR_CLIENT_H
