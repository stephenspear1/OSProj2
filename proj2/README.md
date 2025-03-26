#bool_expr_client.h/cc - The header file contains a BoolExprClient class that uses DomainSocketClient.
                         The source file connects to a server and the US and EOT characters and also the 
                         user inputed T and F values. Those values are sent to the server and then the 
                         nessacary response is read by the client to output the evaluations to terminal.
#bool_expr_server.h/cc - The header file contains a BoolExprServer class that uses DomainSocketServer.
                         The source file hosts a server and waits for a client to connect. After connection,
                         it sends the user inputted US and EOT characters to client. It then reads clients
                         truth values and parses them using the bool_expr_parser.cc file and the inputed
                         expressions file. After getting the results, it sends a response to client.
proj2/
+-- include/
| +-- bool_expr_client.h
| +-- bool_expr_server.h
|
+-- src/
| +-- bool_expr_client.cc
| +-- bool_expr_server.cc
|
+-- README.md