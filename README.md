# ft_irc: Internet Relay Chat Server

## Project Overview
This is a custom IRC server implementation in C++, developed as part of a school project. The server supports standard IRC protocol features and allows multiple clients to connect and communicate.

## Features
- Multiple client connections
- Channel creation and management
- Private messaging
- User authentication
- Basic IRC command support

## Prerequisites
- C++ compiler (C++98 standard)
- Make
- UNIX-like operating system

## Compilation
```bash
make
```

## Usage
```bash
./ircserv <port> <password>
```

## Supported Commands
- `NICK`: Set or change nickname
- `USER`: Register a new user
- `JOIN`: Join a channel
- `PART`: Leave a channel
- `PRIVMSG`: Send private messages
- `QUIT`: Disconnect from server

## Project Structure
- `src/`: Source code files
- `include/`: Header files
- `Makefile`: Build configuration
- `README.md`: Project documentation

## Error Handling
- Robust error management
- Graceful connection handling
- Comprehensive logging

## Limitations
- Supports only IPv4
- No SSL/TLS encryption
- Limited to text-based communication

## Contributing
1. Fork the repository
2. Create your feature branch
3. Commit changes
4. Push to the branch
5. Create a pull request

