#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

#define PORT 12345
#define BUFFER_SIZE 1024
#define DATABASE_PATH "./database/"

// Function to handle admin requests
void handleAdmin(int client_socket);

// Function to handle teacher requests
void handleTeacher(int client_socket, const char *teacherName);

// Function to handle student requests
void handleStudent(int client_socket, const char *rollNumber);

// Function to add student information to the database
void addStudentInfo(const char *teacherName);

// Function to edit student information in the database
void editStudentInfo(const char *teacherName);

// Function to view student information from the database
void viewStudentInfo(const char *teacherName, int client_socket);

// Function to calculate the percentage and write it in the file
void calculatePercentage(const char *teacherName);

int main() {
    int server_fd, new_socket;
    struct sockaddr_in server_address, client_address;
    int addrlen = sizeof(server_address);

    // Create a socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(PORT);

    // Bind the socket to the specified port
    if (bind(server_fd, (struct sockaddr *)&server_address, sizeof(server_address)) == -1) {
        perror("Socket binding failed");
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    if (listen(server_fd, 5) == -1) {
        perror("Socket listening failed");
        exit(EXIT_FAILURE);
    }

    while (1) {
        // Accept a new connection
        new_socket = accept(server_fd, (struct sockaddr *)&client_address, (socklen_t *)&addrlen);
        if (new_socket == -1) {
            perror("Socket acceptance failed");
            exit(EXIT_FAILURE);
        }

        // Fork a child process to handle the client
        pid_t pid = fork();
        if (pid < 0) {
            perror("Fork failed");
            exit(EXIT_FAILURE);
        }

        if (pid == 0) {
            // Child process (client handling)

            // Close the server socket in the child process
            close(server_fd);

            // Perform the necessary operations based on client type (admin, teacher, or student)
            char userType[BUFFER_SIZE];
            memset(userType, 0, BUFFER_SIZE);

            // Receive user type from client
            if (recv(new_socket, userType, BUFFER_SIZE, 0) == -1) {
                perror("Failed to receive user type from client");
                exit(EXIT_FAILURE);
            }

            // Handle admin requests
            if (strcmp(userType, "admin") == 0) {
                handleAdmin(new_socket);
            }

            // Handle teacher requests
            if (strcmp(userType, "teacher") == 0) {
                char teacherName[BUFFER_SIZE];
                memset(teacherName, 0, BUFFER_SIZE);

                // Receive teacher name from client
                if (recv(new_socket, teacherName, BUFFER_SIZE, 0) == -1) {
                    perror("Failed to receive teacher name from client");
                    exit(EXIT_FAILURE);
                }

                handleTeacher(new_socket, teacherName);
           
            // Handle student requests
            if (strcmp(userType, "student") == 0) {
                char rollNumber[BUFFER_SIZE];
                memset(rollNumber, 0, BUFFER_SIZE);

                // Receive student roll number from client
                if (recv(new_socket, rollNumber, BUFFER_SIZE, 0) == -1) {
                    perror("Failed to receive student roll number from client");
                    exit(EXIT_FAILURE);
                }

                handleStudent(new_socket, rollNumber);
            }

            // Close the client socket in the child process
            close(new_socket);

            // Terminate the child process
            exit(EXIT_SUCCESS);
        } else {
            // Parent process (continue accepting connections)

            // Close the client socket in the parent process
            close(new_socket);

            // Wait for child processes to finish
            int status;
            while (waitpid(-1, &status, WNOHANG) > 0) {
                continue;
            }
        }
    }

    // Close the server socket
    close(server_fd);

    return 0;
}}
// Function to handle admin requests
void handleAdmin(int client_socket) {
    while (1) {
        // Receive admin request from client
        char adminRequest[BUFFER_SIZE];
        memset(adminRequest, 0, BUFFER_SIZE);

        if (recv(client_socket, adminRequest, BUFFER_SIZE, 0) == -1) {
            perror("Failed to receive admin request from client");
            exit(EXIT_FAILURE);
        }

        // Perform the requested operation
        if (strcmp(adminRequest, "add") == 0) {
            // Add information to the database
            char teacherName[BUFFER_SIZE];
            memset(teacherName, 0, BUFFER_SIZE);

            // Receive teacher name from client
            if (recv(client_socket, teacherName, BUFFER_SIZE, 0) == -1) {
                perror("Failed to receive teacher name from client");
                exit(EXIT_FAILURE);
            }

            addStudentInfo(teacherName);
        } else if (strcmp(adminRequest, "edit") == 0) {
            // Edit information in the database
            char teacherName[BUFFER_SIZE];
            memset(teacherName, 0, BUFFER_SIZE);

            // Receive teacher name from client
            if (recv(client_socket, teacherName, BUFFER_SIZE, 0) == -1) {
                perror("Failed to receive teacher name from client");
                exit(EXIT_FAILURE);
            }

            editStudentInfo(teacherName);
        } else if (strcmp(adminRequest, "view") == 0) {
            // View information from the database
            char teacherName[BUFFER_SIZE];
            memset(teacherName, 0, BUFFER_SIZE);

            // Receive teacher name from client
            if (recv(client_socket, teacherName, BUFFER_SIZE, 0) == -1) {
                perror("Failed to receive teacher name from client");
                exit(EXIT_FAILURE);
            }

            viewStudentInfo(teacherName, client_socket);
        } else {
            // Invalid request
            break;
        }
    }
}

// Function to handle teacher requests
void handleTeacher(int client_socket, const char *teacherName) {
    // Check if the teacher file exists
    char teacherFilepath[BUFFER_SIZE];
    snprintf(teacherFilepath, BUFFER_SIZE, "%s%s.txt", DATABASE_PATH, teacherName);

    FILE *file = fopen(teacherFilepath, "r");
    if (file == NULL) {
        // Teacher file does not exist, ask if the teacher wants to create it
       
        // Teacher file does not exist, ask if the teacher wants to create it
        char response[BUFFER_SIZE];
        memset(response, 0, BUFFER_SIZE);

        // Send message to client asking if they want to create the file
        char message[BUFFER_SIZE];
        snprintf(message, BUFFER_SIZE, "Teacher file '%s' does not exist. Do you want to create it? (Y/N): ", teacherName);
        if (send(client_socket, message, BUFFER_SIZE, 0) == -1) {
            perror("Failed to send file creation request to client");
            exit(EXIT_FAILURE);
        }

        // Receive response from client
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
