#ifndef REQUEST_HPP
#define REQUEST_HPP

#include "headers.hpp"

class Request {
public:
    Request();
    bool parse(const std::string &raw);
    const std::string &getMethod() const;
    const std::string &getPath() const;
    const std::map<std::string, std::string> &getHeaders() const;

private:
    std::string method;
    std::string path;
    std::string version;
    std::map<std::string, std::string> headers;
};

#endif
