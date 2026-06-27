// ADVANCED TRENDING: TCP Network Sockets in C++ (Winsock2, Windows)
//
// WHAT THIS DEMONSTRATES:
// - Winsock2 RAII wrappers (WSAStartup/WSACleanup, closesocket)
// - TCP server: bind, listen, accept, recv/send loop
// - TCP client: connect, send, recv
// - Running both in the same process using std::thread
// - Echo server + client protocol
//
// On Windows, use Winsock2. On Linux/Mac, replace with POSIX sockets
// (socket/bind/connect — same concepts, no WSAStartup needed).
//
// Compile:
//   g++ -std=c++17 -Wall -O2 -o network_socket_basics_cpp.exe network_socket_basics_cpp.cpp -lws2_32

#define WIN32_LEAN_AND_MEAN
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <string>
#include <thread>
#include <chrono>
#include <stdexcept>
#include <atomic>
#include <vector>

#pragma comment(lib, "ws2_32.lib")

// ============================================================
// RAII: Winsock initialisation
// ============================================================
struct WinsockInit {
    WinsockInit() {
        WSADATA wsa;
        int rc = WSAStartup(MAKEWORD(2, 2), &wsa);
        if (rc != 0)
            throw std::runtime_error("WSAStartup failed: " + std::to_string(rc));
    }
    ~WinsockInit() { WSACleanup(); }
    WinsockInit(const WinsockInit&) = delete;
};

// ============================================================
// RAII: Socket wrapper
// ============================================================
struct Socket {
    SOCKET s;

    Socket() : s(INVALID_SOCKET) {}
    explicit Socket(SOCKET sock) : s(sock) {}

    Socket(int af, int type, int protocol)
        : s(socket(af, type, protocol))
    {
        if (s == INVALID_SOCKET)
            throw std::runtime_error("socket() failed: " + std::to_string(WSAGetLastError()));
    }

    ~Socket() { if (s != INVALID_SOCKET) closesocket(s); }

    Socket(Socket&& o) noexcept : s(o.s) { o.s = INVALID_SOCKET; }
    Socket& operator=(Socket&&) = delete;
    Socket(const Socket&) = delete;

    bool valid() const { return s != INVALID_SOCKET; }
    operator SOCKET() const { return s; }
};

// ============================================================
// Helper: set socket option (reuse address)
// ============================================================
void set_reuse(SOCKET s) {
    int opt = 1;
    setsockopt(s, SOL_SOCKET, SO_REUSEADDR, reinterpret_cast<const char*>(&opt), sizeof(opt));
}

// ============================================================
// Echo Server
// ============================================================
class EchoServer {
    int port;
    std::atomic<bool> running{false};
    std::atomic<int>  clients_served{0};

public:
    explicit EchoServer(int port) : port(port) {}

    void run(int max_clients = 3) {
        Socket listener(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        set_reuse(listener.s);

        sockaddr_in addr{};
        addr.sin_family      = AF_INET;
        addr.sin_addr.s_addr = INADDR_ANY;
        addr.sin_port        = htons(static_cast<u_short>(port));

        if (bind(listener.s, reinterpret_cast<sockaddr*>(&addr), sizeof(addr)) == SOCKET_ERROR)
            throw std::runtime_error("bind() failed: " + std::to_string(WSAGetLastError()));

        if (listen(listener.s, SOMAXCONN) == SOCKET_ERROR)
            throw std::runtime_error("listen() failed");

        running = true;
        std::cout << "[server] Listening on port " << port << "\n";

        for (int i = 0; i < max_clients; ++i) {
            sockaddr_in client_addr{};
            int addrlen = sizeof(client_addr);
            SOCKET raw = accept(listener.s, reinterpret_cast<sockaddr*>(&client_addr), &addrlen);
            if (raw == INVALID_SOCKET) break;

            Socket client(raw);
            char ip[INET_ADDRSTRLEN];
            inet_ntop(AF_INET, &client_addr.sin_addr, ip, sizeof(ip));
            std::cout << "[server] Client connected from " << ip << "\n";

            // Echo loop
            char buf[1024];
            while (true) {
                int n = recv(client.s, buf, sizeof(buf) - 1, 0);
                if (n <= 0) break;
                buf[n] = '\0';
                std::cout << "[server] Received: \"" << buf << "\"\n";
                send(client.s, buf, n, 0);  // echo back
            }
            ++clients_served;
            std::cout << "[server] Client disconnected\n";
        }

        running = false;
        std::cout << "[server] Done. Served " << clients_served.load() << " clients.\n";
    }

    bool is_running() const { return running.load(); }
};

// ============================================================
// TCP Client
// ============================================================
class TcpClient {
    int port;
    std::string host;

public:
    TcpClient(const std::string& host, int port) : port(port), host(host) {}

    void run(const std::vector<std::string>& messages) {
        // Brief wait to let server start
        std::this_thread::sleep_for(std::chrono::milliseconds(200));

        Socket sock(AF_INET, SOCK_STREAM, IPPROTO_TCP);

        sockaddr_in addr{};
        addr.sin_family = AF_INET;
        addr.sin_port   = htons(static_cast<u_short>(port));
        inet_pton(AF_INET, host.c_str(), &addr.sin_addr);

        if (connect(sock.s, reinterpret_cast<sockaddr*>(&addr), sizeof(addr)) == SOCKET_ERROR) {
            std::cerr << "[client] connect() failed: " << WSAGetLastError() << "\n";
            return;
        }

        std::cout << "[client] Connected to " << host << ":" << port << "\n";

        char buf[1024];
        for (const auto& msg : messages) {
            std::cout << "[client] Sending: \"" << msg << "\"\n";
            send(sock.s, msg.c_str(), static_cast<int>(msg.size()), 0);

            int n = recv(sock.s, buf, sizeof(buf) - 1, 0);
            if (n > 0) {
                buf[n] = '\0';
                std::cout << "[client] Echo received: \"" << buf << "\"\n";
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
        }

        std::cout << "[client] Done.\n";
    }
};

// ============================================================
// Demo: server + client in same process, different threads
// ============================================================
void demo_echo() {
    std::cout << "\n=== Echo Server Demo ===\n";

    const int PORT = 54321;

    EchoServer server(PORT);
    std::vector<std::string> messages = {
        "Hello, server!",
        "C++ sockets are fun",
        "TCP is reliable"
    };

    // Server runs in background thread
    std::thread server_thread([&]{ server.run(1); });

    // Client runs in main thread (with a small delay to let server bind)
    TcpClient client("127.0.0.1", PORT);
    std::thread client_thread([&]{ client.run(messages); });

    client_thread.join();
    server_thread.join();
}

// ============================================================
// Explain the concepts
// ============================================================
void explain_sockets() {
    std::cout << "\n=== Socket Programming Concepts ===\n";
    std::cout << "\nServer lifecycle:\n";
    std::cout << "  socket()  -> create socket fd\n";
    std::cout << "  bind()    -> attach to local address:port\n";
    std::cout << "  listen()  -> mark as passive (accept connections)\n";
    std::cout << "  accept()  -> block until client connects, returns new fd\n";
    std::cout << "  recv()    -> read data from connected client\n";
    std::cout << "  send()    -> write data to connected client\n";
    std::cout << "  close()   -> release fd (RAII in this demo)\n";

    std::cout << "\nClient lifecycle:\n";
    std::cout << "  socket()  -> create socket fd\n";
    std::cout << "  connect() -> establish connection to server address:port\n";
    std::cout << "  send()    -> write data to server\n";
    std::cout << "  recv()    -> read server response\n";
    std::cout << "  close()   -> release fd\n";

    std::cout << "\nWinsock vs POSIX:\n";
    std::cout << "  Windows: WSAStartup() before any socket call\n";
    std::cout << "           closesocket() not close()\n";
    std::cout << "           WSAGetLastError() not errno\n";
    std::cout << "  Linux/Mac: no WSAStartup needed, same socket/bind/connect API\n";

    std::cout << "\nFor production: use Boost.Asio or asio (async I/O)\n";
    std::cout << "  async_accept, async_read, async_write — no blocking threads\n";
}

int main() {
    std::cout << "=== TCP Network Sockets in C++ (Winsock2) ===\n";

    try {
        WinsockInit wsa;
        demo_echo();
        explain_sockets();
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }

    return 0;
}
