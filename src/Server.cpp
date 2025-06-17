#include "headers.hpp"

static bool	fileExists(const std::string &path)
{
	struct stat	st;

	return (stat(path.c_str(), &st) == 0 && S_ISREG(st.st_mode));
}

static std::string readFile(const std::string &path)
{
	std::ifstream file(path.c_str());
	std::stringstream ss;
	ss << file.rdbuf();
	return (ss.str());
}

Server::Server(const std::string &configPath)
    : config(configPath)
{
    port = config.getPort();
    webRoot = config.getRoot();
    routes = config.getRoutes();
    initSocket();
    setupPoll();
}


Server::~Server()
{
	close(serverSocket);
}

void Server::initSocket()
{
	int					opt;
	struct sockaddr_in	addr;

	serverSocket = socket(AF_INET, SOCK_STREAM, 0);
	addr.sin_port = htons(port);
	if (serverSocket < 0)
		throw std::runtime_error("socket() failed");
	opt = 1;
	setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
	fcntl(serverSocket, F_SETFL, O_NONBLOCK); // non-blocking
	addr.sin_family = AF_INET;
	addr.sin_port = htons(8080);
	addr.sin_addr.s_addr = INADDR_ANY;
	if (bind(serverSocket, (struct sockaddr *)&addr, sizeof(addr)) < 0)
		throw std::runtime_error("bind() failed");
	if (listen(serverSocket, 10) < 0)
		throw std::runtime_error("listen() failed");
}

void Server::setupPoll()
{
	struct pollfd	pfd;

	pfd.fd = serverSocket;
	pfd.events = POLLIN;
	pollFds.push_back(pfd);
}

void Server::run()
{
	int	ret;

	while (true)
	{
		ret = poll(&pollFds[0], pollFds.size(), 1000);
		if (ret < 0)
			throw std::runtime_error("poll() failed");
		for (size_t i = 0; i < pollFds.size(); ++i)
		{
			if (pollFds[i].revents & POLLIN)
			{
				if (pollFds[i].fd == serverSocket)
					acceptConnection();
				else
					handleClient(pollFds[i].fd);
			}
		}
	}
}

void Server::acceptConnection()
{
	int				clientFd;
	struct pollfd	clientPoll;

	clientFd = accept(serverSocket, NULL, NULL);
	if (clientFd < 0)
		return ;
	fcntl(clientFd, F_SETFL, O_NONBLOCK);
	clientPoll.fd = clientFd;
	clientPoll.events = POLLIN;
	pollFds.push_back(clientPoll);
	clients[clientFd] = "";
	std::cout << "New client connected: " << clientFd << std::endl;
}
void Server::handleClient(int fd) {
    char buffer[1024];
    int bytesRead = recv(fd, buffer, sizeof(buffer) - 1, 0);
    if (bytesRead <= 0) {
        closeClient(fd);
        return;
    }

    buffer[bytesRead] = '\0';
    std::string rawRequest(buffer);

    Request req;
    if (!req.parse(rawRequest)) {
        std::string response = Response::build(400, "Bad Request");
        send(fd, response.c_str(), response.length(), 0);
        closeClient(fd);
        return;
    }

    std::string method = req.getMethod();
    std::string path = req.getPath();

    // Match route from config
    bool methodAllowed = false;
    std::string matchedRoute = "/";
    for (std::map<std::string, Route>::const_iterator it = routes.begin(); it != routes.end(); ++it) {
        if (path.find(it->first) == 0 && it->first.length() >= matchedRoute.length()) {
            matchedRoute = it->first;
            const std::vector<std::string> &methods = it->second.methods;
            for (size_t i = 0; i < methods.size(); ++i) {
                if (method == methods[i]) {
                    methodAllowed = true;
                    break;
                }
            }
        }
    }

    std::cout << "Requested method: " << method << ", path: " << path << std::endl;
    std::cout << "Matched route: " << matchedRoute << std::endl;
    std::cout << "Allowed methods: ";
    for (size_t i = 0; i < routes[matchedRoute].methods.size(); ++i)
        std::cout << routes[matchedRoute].methods[i] << " ";
    std::cout << std::endl;

    if (!methodAllowed) {
        std::string response = Response::build(405, "Method Not Allowed");
        send(fd, response.c_str(), response.length(), 0);
        closeClient(fd);
        return;
    }

    if (method == "POST" && path == "/upload") {
        size_t headerEnd = rawRequest.find("\r\n\r\n");
        if (headerEnd != std::string::npos) {
            std::string body = rawRequest.substr(headerEnd + 4);

            std::ofstream out("uploads/uploaded_file.txt");
            if (out.is_open()) {
                out << body;
                out.close();
                std::string response = Response::build(200, "File uploaded successfully.");
                send(fd, response.c_str(), response.size(), 0);
            } else {
                std::string response = Response::build(500, "Failed to save file.");
                send(fd, response.c_str(), response.size(), 0);
            }
        } else {
            std::string response = Response::build(400, "Bad POST request format.");
            send(fd, response.c_str(), response.size(), 0);
        }

        closeClient(fd);
        return;
    }

    std::string filepath = webRoot + (path == "/" ? "/index.html" : path);
    if (fileExists(filepath)) {
        std::string content = readFile(filepath);
        std::string response = Response::build(200, content);
        send(fd, response.c_str(), response.size(), 0);
    } else {
        std::string response = Response::build(404, "Not Found");
        send(fd, response.c_str(), response.size(), 0);
    }


    closeClient(fd);
}

void Server::closeClient(int fd)
{
	close(fd);
	for (size_t i = 0; i < pollFds.size(); ++i)
	{
		if (pollFds[i].fd == fd)
		{
			pollFds.erase(pollFds.begin() + i);
			break ;
		}
	}
	clients.erase(fd);
	std::cout << "Closed connection: " << fd << std::endl;
}
