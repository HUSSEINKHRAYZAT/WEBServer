#include "headers.hpp"

Request::Request() {}

bool Request::parse(const std::string &raw) {
    std::istringstream stream(raw);
    std::string line;

    if (!std::getline(stream, line)) return false;

    std::istringstream startLine(line);
    startLine >> method >> path >> version;
    if (method.empty() || path.empty() || version.empty()) return false;

    while (std::getline(stream, line) && line != "\r") {
        size_t colon = line.find(':');
        if (colon != std::string::npos) {
            std::string key = line.substr(0, colon);
            std::string value = line.substr(colon + 1);
            while (!value.empty() && value[0] == ' ') value.erase(0, 1);
            if (!value.empty() && value[value.size()-1] == '\r')
                value.erase(value.size()-1);
            headers[key] = value;
        }
    }

    return true;
}

const std::string &Request::getMethod() const { return method; }
const std::string &Request::getPath() const { return path; }
const std::map<std::string, std::string> &Request::getHeaders() const { return headers; }
