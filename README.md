# ft_irc

> *Internet Relay Chat Server — 42 Curriculum*

**ft_irc** is a custom Internet Relay Chat (IRC) server written in C++98. This project challenges developers to understand network protocols, handle multiple client connections simultaneously using non-blocking I/O, and implement a robust server architecture that complies with IRC standards.

---

## 👥 The Team

This project was developed collaboratively by:
*   **ahamou-t**
*   **shrimech**
*   **houardi**

---

## 🚀 Features

The server supports a core set of IRC commands and features, allowing users to connect via standard IRC clients (such as irssi, WeeChat, or netcat) and communicate in real-time.

### Connection & Authentication
*   Password protection (`PASS`)
*   Nickname registration and collision handling (`NICK`)
*   User registration (`USER`)

### Channel Operations
*   **JOIN / PART:** Enter and leave channels.
*   **PRIVMSG:** Send private messages to users or broadcast to entire channels.
*   **TOPIC:** View or change the channel topic.

### Operator Commands
Channel operators have elevated privileges to manage their channels:
*   **KICK:** Eject a user from the channel.
*   **INVITE:** Invite a user to a private channel.
*   **MODE:** Modify channel settings:
    *   `+i`: Invite-only channel.
    *   `+t`: Topic restricted to channel operators.
    *   `+k`: Set/remove channel key (password).
    *   `+o`: Give/take channel operator privileges.
    *   `+l`: Set/remove user limit on the channel.

---

## 🛠️ Technical Overview

*   **Language:** Fully compliant with **C++98** standards.
*   **I/O Multiplexing:** Utilizes a single polling mechanism (`poll()`, `epoll()`, or `kqueue()`) to handle all incoming data from multiple clients without blocking the main execution thread.
*   **Sockets:** Built on standard POSIX sockets (`sys/socket.h`), using strictly non-blocking file descriptors (`fcntl`).
*   **Client Management:** Gracefully handles partial data reception, unexpected client disconnections, and buffering to ensure stable packet framing.

---

## ⚙️ Installation & Usage

### Prerequisites
*   A C++ compiler (`c++`, `clang++`, or `g++`)
*   `make`

### Build
Compile the project using the provided Makefile:

```bash
make
