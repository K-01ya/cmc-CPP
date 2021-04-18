#include <iostream>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string>
#include <vector>
#include <stdexcept>
#include <arpa/inet.h>
#include <unistd.h>
#include <fstream>
#include <sys/stat.h>
#include <fcntl.h>

class SocketAddress {
  private:
    struct sockaddr_in saddr;
  public:
    SocketAddress() {
        saddr.sin_family = AF_INET;
        saddr.sin_port = htons(1234);
        saddr.sin_addr.s_addr = INADDR_ANY;
    };
    SocketAddress(const char* addr, short port) {
        saddr.sin_family = AF_INET;
        saddr.sin_port = htons(port);
        saddr.sin_addr.s_addr = inet_addr(addr);
    };
    struct sockaddr* GetAddr() const {
        return (sockaddr*) &saddr;
    }
    socklen_t Size() const {
        return sizeof(saddr);
    }
};

class Socket{
  protected:
    int _sd;
    explicit Socket(int sd) : _sd(sd) {};
  public:
    Socket() {
        _sd = socket(AF_INET, SOCK_STREAM, 0);
        if (_sd == -1)
            std::cerr << "socket error" << std::endl;
    }
    void Shutdown() {
        shutdown(_sd, 2);
    }
    ~Socket() {
        close(_sd);
    }
};

class ServerSocket : public Socket {
  public:
    ServerSocket() : Socket() {}
    void Bind(const SocketAddress& ipaddr) {
        if(bind(_sd, ipaddr.GetAddr(), ipaddr.Size()) == -1)
            std::cerr << "bind error" << std::endl;
    };
    int Accept(SocketAddress& clAddr) {
        socklen_t size = clAddr.Size();
        int res = accept(_sd, clAddr.GetAddr(), (socklen_t *)&size);
        if (res == -1)
            std::cerr << "accept error" << std::endl;
        return res;
    };
    void Listen(int backlog) {
        if (listen(_sd, backlog) == -1)
            std::cerr << "listen error" << std::endl;
    };
};

class ConnectedSocket : public Socket {
  public:
    ConnectedSocket() = default;
    explicit ConnectedSocket(int sd) : Socket(sd) {};
    void Write(const std::string& str) {
        if (send(_sd, &str[0], str.length(), 0) == -1)
            std::cerr << "send error" << std::endl;
    };
    void Write(const std::vector<uint8_t>& bytes) {
        if (send(_sd, &bytes[0], bytes.size(), 0) == -1)
            std::cerr << "send error" << std::endl;
    };
    void Read(std::string& str) {
        size_t len = 10000;
        char buf[len];
        if (recv(_sd, buf, len, 0) == -1)
            std::cerr << "read error" << std::endl;
        str = buf;
    };
    void Read (std::vector<uint8_t>& bytes) {
        size_t len = 10000;
        uint8_t buf[len];
        recv(_sd, buf, len, 0);
        std::vector<uint8_t> t(buf, buf + sizeof(buf) / sizeof(buf[0]));
        bytes = t;
    };

};
std::vector<std::string> SplitLines(std::string& s) {
    std::vector<std::string> split_vector;
    std::string delimiter = "\r\n";
    size_t pos = 0;
    std::string token;
    while ((pos = s.find(delimiter)) != std::string::npos) {
        token = s.substr(0, pos);
        split_vector.push_back(token);
        s.erase(0, pos + delimiter.length());
    }
    split_vector.push_back(s);
    return split_vector;
}

std::string parseSpace(std::string &str) {
    int pos1 = str.find(' ');
    int pos2 = str.find(' ', pos1 + 1);
    return str.substr(pos1 + 1, pos2 - pos1 - 1);
}

std::vector<uint8_t > Read(int fd) {
    char buf;
    std::vector<uint8_t> arr;
    while(read(fd, &buf, 1)) {
        arr.push_back(buf);
    }
    return arr;
}

void ProcessConnection(int cd, const SocketAddress& clAddr) {
    ConnectedSocket cs(cd);
    std::string request;
    cs.Read(request);
    std::vector<std::string> lines = SplitLines(request);
    std::string path = parseSpace(lines[0]);
    std::cout << path << "\n";
    path = "index" + path;
    int fd;
    if (path == "index/") {
        fd = open("index/index.html", O_RDONLY);
    }
    else
        fd = open(path.c_str(), O_RDONLY);
    if (fd == -1) {
        cs.Write("HTTP/1.0 404 Not Found");
        fd = open("index/404.html", O_RDONLY);
        std::cerr << "Page is not found \n";
    }
    else {
        cs.Write("HTTP/1.0 200 OK");
    }
    std::vector<uint8_t> arr = Read(fd);
    std::string str = "\r\nVersion: HTTP/1.1\r\nContent-length: " + std::to_string(arr.size()) + "\r\n\r\n";

    cs.Write(str);
    cs.Write(arr);
    close(fd);
    cs.Shutdown();
}

void ServerLoop() {
    SocketAddress serverAddr("127.0.0.1", 1234);
    ServerSocket ss;
    ss.Bind(serverAddr);
    ss.Listen(5);
    for(;;) {
        SocketAddress clAddr;
        int cd = ss.Accept(clAddr);
        ProcessConnection(cd, clAddr);
    }
}

int main() {
    ServerLoop();
}