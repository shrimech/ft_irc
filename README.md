*This project has been created as part of the 42 curriculum by ahamou-t, shrimech, houardi.*

# ft_irc

> *Internet Relay Chat Server — 42 Curriculum*

---

## 📄 Description

**ft_irc** is an Internet Relay Chat (IRC) server written in **C++98**. The main goal of this project is to build a fully functional, non-blocking network server capable of handling multiple client connections concurrently without using multi-threading.

Using non-blocking POSIX sockets and a single event multiplexing loop (`poll()`, `select()`, `kqueue()`, or `epoll()`), the server handles partial message reads, client buffering, and command processing. It complies with the project specification and can be tested with standard IRC clients like **Irssi**, **WeeChat**, or `nc`.

### Core Features

*   **Authentication Flow:** Supports `PASS` (server password validation), `NICK` (nickname assignment and duplicate collision handling), and `USER` (registration completion).
*   **Channel Operations:**
    *   `JOIN`: Create or enter channels with topic and member updates.
    *   `TOPIC`: Display or modify the topic based on permissions.
    *   `INVITE`: Invite users to invite-only channels.
    *   `KICK`: Remove target users from channels.
    *   `MODE`: Manage channel state flags:
        *   `+i` / `-i`: Toggle invite-only status.
        *   `+t` / `-t`: Restrict `TOPIC` changes to channel operators.
        *   `+k` / `-k`: Set or remove channel password key.
        *   `+o` / `-o`: Grant or revoke channel operator status.
        *   `+l` / `-l`: Set or remove maximum client limits.
*   **Messaging & Bot:**
    *   Direct `PRIVMSG` routing between individual users.
    *   Channel message broadcasting.
    *   Built-in bot support for automated/interactive command replies.

---

## 🛠️ Instructions

### Prerequisites

*   A standard Unix-like environment (Linux / macOS)
*   A C++ compiler supporting C++98 (`c++`, `g++`, or `clang++`)
*   `make`

### Compilation

Compile the project executable using the provided Makefile:

```bash
make
```

### Execution

Launch the server by passing the target port and network password:

```bash
./ircserv <port> <password>
```

*Example:*
```bash
./ircserv 6667 my_secret_pass
```

### Connecting to the Server

Connect using any IRC client or standard TCP network utility:

```bash
irssi -c localhost -p 6667 -n my_nickname -w my_secret_pass
```

---

## 📚 Resources

### IRC Specifications & Documentation

*   [RFC 1459 — Internet Relay Chat Protocol](https://datatracker.ietf.org/doc/html/rfc1459)
*   [RFC 2812 — Internet Relay Chat: Client Protocol](https://datatracker.ietf.org/doc/html/rfc2812)
*   [Beej's Guide to Network Programming](https://beej.us/guide/bgnet/)
*   [Modern IRC Client Protocol Docs](https://modern.ircdocs.horse/)

### AI Assistance Declaration

Artificial Intelligence tools (ChatGPT / Claude / Gemini) were utilized during this project for the following tasks:

*   **Debugging & Edge Cases:** Analyzing network edge-case behavior (e.g., partial read/write socket buffering and socket disconnection edge cases).
*   **Documentation:** Formatting and structuring project documentation and `README.md` to ensure clarity and standard compliance.
*   **Testing:** Writing automated scripts and test sequences to verify RFC command syntax and protocol edge cases.