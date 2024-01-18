import socket
import os

# Constants
HOST = '127.0.0.1'  # Server IP address
PORT = 8000  # Server port
BUFFER_SIZE = 1024

# Database file path
DATABASE_PATH = 'database/'
# Function to add student information to the database
def add_student_info(teacher_name, student_info):
    teacher_filepath = DATABASE_PATH + teacher_name + '.txt'

    # Add student information to the teacher's database file
    with open(teacher_filepath, 'a') as file:
        file.write(student_info + '\n')

# Function to edit student information in the database
def edit_student_info(teacher_name, student_info):
    teacher_filepath = DATABASE_PATH + teacher_name + '.txt'

    # Read the contents of the teacher's database file
    with open(teacher_filepath, 'r') as file:
        lines = file.readlines()

    # Find the student information to edit
    student_roll_number = student_info.split(',')[0].strip()
    edited_lines = []

    for line in lines:
        roll = line.split(',')[0].strip()

        if roll == student_roll_number:
            # Replace the existing student information with the edited information
            edited_lines.append(student_info + '\n')
        else:
            edited_lines.append(line)

    # Write the edited contents back to the teacher's database file
    with open(teacher_filepath, 'w') as file:
        file.writelines(edited_lines)

# Function to view student information in the teacher's database
def view_student_info(teacher_name, client_socket):
    teacher_filepath = DATABASE_PATH + teacher_name + '.txt'

    # Read the contents of the teacher's database file
    with open(teacher_filepath, 'r') as file:
        lines = file.readlines()

    # Send the student information to the client
    for line in lines:
        client_socket.send(line.encode())

    # Send end of file message to the client
    eof_msg = 'EOF'
    client_socket.send(eof_msg.encode())

    # Close the client socket
    client_socket.close()

# Function to handle admin requests
def handle_admin(client_socket):
    while True:
        # Receive admin request from client
        admin_request = client_socket.recv(BUFFER_SIZE).decode()

        if admin_request == 'add':
            # Add information to the database
            teacher_name = client_socket.recv(BUFFER_SIZE).decode()
            add_student_info(teacher_name)
        elif admin_request == 'edit':
            # Edit information in the database
            teacher_name = client_socket.recv(BUFFER_SIZE).decode()
            edit_student_info(teacher_name)
        elif admin_request == 'view':
            # View information from the database
            teacher_name = client_socket.recv(BUFFER_SIZE).decode()
            view_student_info(teacher_name, client_socket)
        else:
            # Invalid request
            break

# Function to handle teacher requests
def handle_teacher(client_socket, teacher_name):
    # Check if the teacher file exists
    teacher_filepath = DATABASE_PATH + teacher_name + '.txt'

    try:
        file = open(teacher_filepath, 'r')
        file.close()
    except FileNotFoundError:
        # Teacher file does not exist, ask if the teacher wants to create it
        response = client_socket.recv(BUFFER_SIZE).decode()

        if response.lower() == 'y':
            file = open(teacher_filepath, 'w')
            file.close()

            # Send confirmation message to the client
            confirmation = 'File created successfully.'
            client_socket.send(confirmation.encode())
        else:
            # Send cancellation message to the client
            cancellation = 'Operation cancelled.'
            client_socket.send(cancellation.encode())

            # Close the client socket
            client_socket.close()
            return

    # Continue handling other teacher operations (add/edit/view information)
    while True:
        # Receive teacher operation request from client
        teacher_request = client_socket.recv(BUFFER_SIZE).decode()

        if teacher_request == 'add':
            # Add student information to the database
            student_info = client_socket.recv(BUFFER_SIZE).decode()
            add_student_info_to_teacher(teacher_name, student_info)
        elif teacher_request == 'edit':
            # Edit student information in the database
            student_info = client_socket.recv(BUFFER_SIZE).decode()
            edit_student_info_in_teacher(teacher_name, student_info)
        elif teacher_request == 'view':
            # View student information from the database
            view_student_info_in_teacher(teacher_name, client_socket)
        else:
            # Invalid request
            break

# Function to handle student requests
def handle_student(client_socket, roll_number):
    # Search for the student information in the database
    student_info = search_student_info(roll_number)

    if student_info:
        # Send the student information to the client
        client_socket.send(student_info.encode())
    else:
        # Send error message to the client
        error_msg = 'Student not found.'
        client_socket.send(error_msg.encode())

    # Close the client socket
    client_socket.close()

# Function to add student information to the database
# Function to add student information to the database
def add_student_info_to_teacher(teacher_name, student_info):
    teacher_filepath = DATABASE_PATH + teacher_name + '.txt'

    # Add student information to the teacher's database file
    with open(teacher_filepath, 'a') as file:
        file.write(student_info + '\n')

# Function to edit student information in the database
def edit_student_info_in_teacher(teacher_name, student_info):
    teacher_filepath = DATABASE_PATH + teacher_name + '.txt'

    # Read the contents of the teacher's database file
    with open(teacher_filepath, 'r') as file:
        lines = file.readlines()

    # Find the student information to edit
    student_roll_number = student_info.split(',')[0].strip()
    edited_lines = []

    for line in lines:
        roll = line.split(',')[0].strip()

        if roll == student_roll_number:
            # Replace the existing student information with the edited information
            edited_lines.append(student_info + '\n')
        else:
            edited_lines.append(line)

    # Write the edited contents back to the teacher's database file
    with open(teacher_filepath, 'w') as file:
        file.writelines(edited_lines)

# Function to view student information in the teacher's database
def view_student_info_in_teacher(teacher_name, client_socket):
    teacher_filepath = DATABASE_PATH + teacher_name + '.txt'

    # Read the contents of the teacher's database file
    with open(teacher_filepath, 'r') as file:
        lines = file.readlines()

    # Send the student information to the client
    for line in lines:
        client_socket.send(line.encode())

    # Send end of file message to the client
    eof_msg = 'EOF'
    client_socket.send(eof_msg.encode())

    # Close the client socket
    client_socket.close()

# Function to search for student information in all teacher's databases
def search_student_info(roll_number):
    student_info = None

    # Iterate over all teacher's databases
    for teacher_name in get_teacher_names():
        teacher_filepath = DATABASE_PATH + teacher_name + '.txt'

        # Search for the student information in the teacher's database
        with open(teacher_filepath, 'r') as file:
            lines = file.readlines()

        for line in lines:
            roll = line.split(',')[0].strip()

            if roll == roll_number:
                # Student found, store the information
                student_info = line
                break

    return student_info

# Function to get the names of all teachers
def get_teacher_names():
    teacher_names = []

    # Iterate over the database files
    for filename in os.listdir(DATABASE_PATH):
        if filename.endswith('.txt'):
            # Extract the teacher name from the file name
            teacher_name = filename[:-4]
            teacher_names.append(teacher_name)

    return teacher_names

# Main server function
def server():
    # Create a socket object
    server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

    # Bind the socket to a specific address and port
    server_socket.bind((HOST, PORT))

    # Listen for incoming connections
    server_socket.listen(1)
    print('Server listening on {}:{}'.format(HOST, PORT))

    while True:
        # Accept a client connection
        client_socket, addr = server_socket.accept()
        print('Connected to client:', addr)

        # Receive user type from client
        user_type = client_socket.recv(BUFFER_SIZE).decode()

        if user_type == 'admin':
            # Handle admin requests
            handle_admin(client_socket)
        elif user_type == 'teacher':
            # Receive teacher name from client
            teacher_name = client_socket.recv(BUFFER_SIZE).decode()

            # Handle teacher requests
            handle_teacher(client_socket, teacher_name)
        elif user_type == 'student':
            # Receive roll number from client
            roll_number = client_socket.recv(BUFFER_SIZE).decode()

            # Handle student requests
            handle_student(client_socket, roll_number)
        else:
            # Invalid user type
            break

    # Close the server socket
    server_socket.close()

# Start the server
server()
