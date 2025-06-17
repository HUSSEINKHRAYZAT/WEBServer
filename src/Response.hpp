#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include "headers.hpp"

class Response {
public:
    static std::string build(int statusCode, const std::string &body);
};

#endif
