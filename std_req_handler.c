#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <sys/types.h>

#define DATABASE_PATH "./database/"

// Function to handle student requests
void handleStudent(int client_socket, const char *rollNumber) {
    // Search for the student information in the database
    // ...

    // Send the student information to the client
    // ...
}
