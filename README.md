# ft_irc

*This project has been created as part of the 42 curriculum by ahamou-t, shrimech, houardi.*

# ft_irc

## Description

`ft_irc` is a custom Internet Relay Chat (IRC) server written in C++98.

The goal of this project is to reproduce the behaviour of a real IRC server closely enough to let standard IRC clients connect, authenticate, join channels, exchange messages, and manage channel state. The server is designed around non-blocking sockets and `poll()` so it can handle multiple clients at the same time without blocking the event loop.

This implementation includes:
- password-based authentication with `PASS`, `NICK`, and `USER`
- multi-client handling with non-blocking I/O
- private messages with `PRIVMSG`
- channel creation and management with `JOIN`, `TOPIC`, `INVITE`, `KICK`, and `MODE`
- channel modes such as `+i`, `+t`, `+k`, `+o`, and `+l`
- graceful shutdown through signal handling

## Instructions

### Requirements
- A C++ compiler compatible with `-std=c++98`
- `make`
- An IRC client such as `irssi`, `WeeChat`, or `nc`

### Compilation
Build the executable from the root of the repository:

```bash
make
```

This produces the server executable:

```bash
./irserve
```

### Execution
Run the server with a port and a password:

```bash
./irserve <port> <password>
```

Example:

```bash
./irserve 6667 1234
```

### Connecting with a client
From an IRC client, connect using:
- host: `127.0.0.1`
- port: the port you chose
- password: the password passed to the server

Then authenticate with the IRC client's normal connection settings or with the `PASS`, `NICK`, and `USER` sequence supported by the server.

### Cleaning the project
```bash
make clean
make fclean
make re
```

## Features

### Authentication
The server requires clients to authenticate before they can use most IRC commands. The registration flow is based on:
- `PASS`
- `NICK`
- `USER`

### Channel management
The server supports:
- `JOIN` to enter a channel
- `TOPIC` to read or change the channel topic
- `INVITE` to invite users into invite-only channels
- `KICK` to remove a user from a channel
- `MODE` to manage channel permissions and restrictions

### Supported channel modes
- `+i`: invite-only channel
- `+t`: only operators can change the topic
- `+k`: channel key/password
- `+o`: give or remove operator privileges
- `+l`: channel user limit

### Messaging
- `PRIVMSG` for direct user and channel messages

## Technical choices

- **C++98**: the project follows the 42 requirement for C++98 compatibility.
- **`poll()`**: used to monitor the server socket and all connected client sockets in one loop.
- **Non-blocking sockets**: prevents one slow client from blocking the whole server.
- **Buffered client handling**: allows partial TCP messages to be assembled before parsing.
- **Graceful cleanup**: sockets are closed properly when clients disconnect or the server receives a termination signal.

## Resources

### References
- RFC 2812 — Internet Relay Chat: Client Protocol
- POSIX `socket`, `bind`, `listen`, `accept`, `send`, `recv`, `fcntl`, and `poll` documentation
- C++98 references for standard library containers and string handling
- IRC client documentation for `irssi` and `WeeChat`

### AI usage
AI was used to help draft this README, organize the sections, and improve the wording of the technical explanation.
## Notes

- The server executable name in this repository is `irserve`.
- The exact IRC client commands may vary slightly depending on the client used.
- If a feature is still in progress in the codebase, this README should be updated to match the final implementation.
