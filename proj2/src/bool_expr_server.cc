// Copyright 2025 Stephen Spear

#include <bool_expr_server.h>
#include <domain_socket.h>
#include <bool_expr_parser.h>
#include <iostream>
#include <fstream>
#include <unordered_map>
#include <string>

void BoolExprServer::Run() {
  if (!Init(5)) {
    exit(1);
  }

  while (true) {
    int client_fd = Accept();
    if (client_fd < 0) {
      std::cerr << "DomainSocketServer failed to accept client.\n";
      continue;
    }

    std::cout << "Client connected" << std::endl;

    std::string init_message = std::string(1, US_) + std::string(1, EOT_);
    ::ssize_t bytes_wrote = Write(client_fd, init_message);

    std::string truth_values;
    ::ssize_t bytes_read = Read(client_fd, &truth_values);
    if (bytes_read <= 0) {
      std::cerr << "BoolExprServer: Failed to receive truth"
                    << " values from client.\n";
      close(client_fd);
      return;
    }

    std::string parsed_values = Explode(truth_values.c_str(), US_);
    std::unordered_map<char, bool> variable_map = BuildMap(parsed_values);

    std::ifstream file(file_path_);
    if (!file) {
      std::cerr << "BoolExprServer: Error opening file: " << file_path_ << "\n";
      close(client_fd);
      return;
    }

    int count_true = 0;
    int count_false = 0;
    int count_unknown = 0;
    std::string line;

    while (std::getline(file, line)) {
      line = Explode(line.c_str(), ' ');
      BooleanExpressionParser parser(line, variable_map);
      bool result = parser.Parse();

      if (parser.HasError()) {
        count_unknown++;
      } else if (result) {
        count_true++;
      } else {
        count_false++;
      }
    }

    file.seekg(0, file.beg);

    std::string summary = "True Evaluations: " + std::to_string(count_true) +
                    "\nFalse Evaluations: " + std::to_string(count_false) +
                    "\nCould Not Evaluate: " + std::to_string(count_unknown);

    bytes_wrote += Write(client_fd, summary);
    std::cout << "   " << static_cast<int>(bytes_wrote)
              << "B sent, " << static_cast<int>(bytes_read)
              << "B received" << std::endl;

    //signal(SIGPIPE, SIG_IGN);
    close(client_fd);
    std::cout << "Client disconnected" << std::endl;
  }
}

int main(int argc, char* argv[]) {
  if (argc != 5) {
    std::cerr << "Usage: ./bool_expr_server <expr_file> "
       << "<socket_name> <US_char> <EOT_char>\n";
    return 1;
  }

  std::string file_path = argv[1];
  const char* socket_name = argv[2];
  char US = argv[3][0];
  char EOT = argv[4][0];

  BoolExprServer server(socket_name, file_path, US, EOT);
  server.Run();

  return 0;
}
