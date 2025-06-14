#include "headers.hpp"

std::string Response::build(int statusCode, const std::string &body)
{
	std::ostringstream res;
	res << "HTTP/1.1 ";
	switch (statusCode)
	{
	case 200:
		res << "200 OK";
		break ;
	case 404:
		res << "404 Not Found";
		break ;
	case 500:
		res << "500 Internal Server Error";
		break ;
	default:
		res << statusCode << " Unknown";
	}
	std::string contentType = "text/plain";
	if (body.find("<html") != std::string::npos)
		contentType = "text/html";
	res << "\r\nContent-Length: " << body.length() << "\r\nContent-Type: " << contentType << "\r\n\r\n" << body;
	return (res.str());
}
