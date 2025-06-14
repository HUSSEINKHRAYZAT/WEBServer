#ifndef CONFIG_HPP
#define CONFIG_HPP

#include "headers.hpp"

struct Route {
    std::vector<std::string> methods;
};

class Config {
public:
    Config(const std::string &path);
    int getPort() const;
    const std::string &getRoot() const;
    const std::map<std::string, Route>& getRoutes() const;

private:
    int port;
    std::string root;
    std::map<std::string, Route> routes;

    void parseServerBlock(std::ifstream &file);
};

#endif
