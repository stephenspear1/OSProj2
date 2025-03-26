// Copyright 2025 Stephen Spear
#ifndef BOOL_EXPR_SERVER_H
#define BOOL_EXPR_SERVER_H

#include <domain_socket.h>
#include <string>

class BoolExprServer : public DomainSocketServer {
 public:
    using ::DomainSocketServer::DomainSocketServer;
    BoolExprServer(const char* socket_path, const std::string& file_path,
                   char US, char EOT)
        : DomainSocketServer(socket_path, US, EOT, false),
                             file_path_(file_path), US_(US), EOT_(EOT) {}

    void Run();
    void HandleClient(int client_fd);

 private:
    std::string file_path_;
    char US_;
    char EOT_;
};

#endif  // BOOL_EXPR_SERVER_H
