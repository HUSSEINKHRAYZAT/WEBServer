#ifndef SERVER_HPP
# define SERVER_HPP

# include "headers.hpp"

#include "Config.hpp"
struct Route;

class Server {
public:
    Server(const std::string &configPath);
    ~Server();
    void run();

private:
    int port;
    std::string webRoot;
    int serverSocket;
    std::vector<struct pollfd> pollFds;
    std::map<int, std::string> clients;

    Config config;  // ✅ Add this line
    std::map<std::string, Route> routes;  // ✅ And this line

    void initSocket();
    void setupPoll();
    void acceptConnection();
    void handleClient(int fd);
    void closeClient(int fd);
};

#endif
