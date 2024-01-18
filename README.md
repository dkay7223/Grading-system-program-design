# Grading System Program Design

## Overview
This assignment requires the design and implementation of a customized grading system for XYZ university using C/C++ with TCP using fork(). The system involves three types of users: Admin, Teachers, and Students. The server maintains a database with separate files for each teacher. Initially, the database contains records for two teachers. Only one server exists, but multiple clients can connect as teachers or students.

## User Access

### Admin
- Complete control over the database.
- Can add/edit and view information, including marks of all courses and students.
- Can create files.

### Teachers
- Can add/edit and view information for their own course and registered students.
- Identify by giving their name to the server.
- Can create a new file if it doesn't exist.

### Students
- Can view information by providing their roll number to the server.
- Receive a message if the roll number doesn't exist.
- Cannot view information of other students.

## Instructions for Clients
- Clients have the functionality of both teachers and students.
- When a client runs, they choose whether they are a teacher or a student.

### Student
- Entering "student" allows viewing desired information by providing a roll number.
- Roll numbers are matched in all files.
- Receives a message if the roll number doesn't exist.

### Teacher
- Entering "teacher" allows viewing information by providing the teacher's name.
- Teacher's name is matched with file names.
- A message is sent if the file doesn't exist, with an option to create a new file.
- Can add information about a student in the same format as existing files.

## Admin
- On termination of all teachers and students, the server calculates the percentage of each student.
- The percentages are written in front of their roll numbers in the file.

## Server Clarifications
- The admin has complete database control, including file creation and information management.
- Teacher requests are processed by sending a request to the admin.
- Student/teacher information requests are handled by sending a request to the server.

## System Operation
- One terminal for the server.
- Multiple teachers and students can connect to the server as clients.


## Code Overview Report

### 1. Header Files
The code is equipped with essential C header files for socket communication, file operations, process handling, and string manipulation.

### 2. Macro Definitions
- **PORT**: Specifies the server's listening port number for incoming connections.
- **BUFFER_SIZE**: Defines the buffer size for data transmission.
- **DATABASE_PATH**: Specifies the path for storing student information files.

### 3. Function Declarations
The code declares several functions catering to different request types:
- **handleAdmin()**: Manages admin requests, covering tasks like adding, editing, and viewing student information.
- **handleTeacher()**: Manages teacher requests, involving file creation and operations.
- **handleStudent()**: Manages student requests.
- **addStudentInfo()**: Adds student information to the database.
- **editStudentInfo()**: Edits student information in the database.
- **viewStudentInfo()**: Views student information from the database.
- **calculatePercentage()**: Calculates percentages and writes them to a file.

### 4. `main()` Function
The main function serves as the program's entry point, executing the following steps:
- Initializes variables and data structures for socket communication.
- Creates a socket using `socket()`.
- Binds the socket to the specified port using `bind()`.
- Listens for incoming connections with `listen()`.
- Accepts client connections in a loop, forking child processes to handle each client.
- In the child process, calls handling functions based on the received user type.
- In the parent process, waits for child processes to finish before accepting new connections.
- Closes the server socket upon program exit.

### 5. `handleAdmin()` Function
This function manages admin requests within an infinite loop until an invalid request is received. It receives admin requests from the client and performs operations such as adding, editing, or viewing student information.

### 6. `handleTeacher()` Function
This function manages teacher requests, checking if the teacher file exists. If not, it prompts the teacher to create it. If confirmed, the file is created; otherwise, the operation is canceled.

*Note: The code is incomplete, lacking some function implementations. Nevertheless, it serves as a foundation for a server-client program enabling admins, teachers, and students to interact with the server for student information management.*
