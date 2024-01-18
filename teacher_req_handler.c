#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <sys/types.h>

#define DATABASE_PATH "./database/"

// Function to handle teacher requests
void handleTeacher(int client_socket, const char *teacherName) {
    // Check if the teacher file exists
    char teacherFilepath[BUFFER_SIZE];
    snprintf(teacherFilepath, BUFFER_SIZE, "%s%s.txt", DATABASE_PATH, teacherName);

    FILE *file = fopen(teacherFilepath, "r");
    if (file == NULL) {
        // Teacher file does not exist, ask if the teacher wants to create it
        char createFileRequest[BUFFER_SIZE];
        memset(createFileRequest, 0, BUFFER_SIZE);
        strcpy(createFileRequest, "File does not exist. Do you want to create it? (Y/N): ");

        // Send the create file request to the client
        if (send(client_socket, createFileRequest, BUFFER_SIZE, 0) == -1) {
            perror("Failed to send create file request to client");
            exit(EXIT_FAILURE);
        }

        // Receive the response from the client
        char response[BUFFER_SIZE];
        memset(response, 0, BUFFER_SIZE);
        // ...

        if (recv(client_socket, response, BUFFER_SIZE, 0) == -1) {
            perror("Failed to receive response from client");
            exit(EXIT_FAILURE);
        }

        // Create the teacher file if the response is positive
        if (strcmp(response, "Y") == 0 || strcmp(response, "y") == 0) {
            file = fopen(teacherFilepath, "w");
            if (file == NULL) {
                perror("Failed to create teacher file");
                exit(EXIT_FAILURE);
            }

            // Send confirmation message to the client
            char confirmation[BUFFER_SIZE];
            memset(confirmation, 0, BUFFER_SIZE);
            strcpy(confirmation, "File created successfully.");

            if (send(client_socket, confirmation, BUFFER_SIZE, 0) == -1) {
                perror("Failed to send confirmation to client");
                exit(EXIT_FAILURE);
            }
        } else {
            // Send cancellation message to the client
            char cancellation[BUFFER_SIZE];
            memset(cancellation, 0, BUFFER_SIZE);
            strcpy(cancellation, "Operation cancelled.");

            if (send(client_socket, cancellation, BUFFER_SIZE, 0) == -1) {
                perror("Failed to send cancellation to client");
                exit(EXIT_FAILURE);
            }

            // Close the client socket
            close(client_socket);

            // Terminate the child process
            exit(EXIT_SUCCESS);
        }
    }

    // Continue handling other teacher operations (add/edit/view information)
    // ...
}
