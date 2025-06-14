#include "headers.hpp"

Config::Config(const std::string &path) : port(8080), root("./www")
{
	std::ifstream file(path.c_str());
	if (!file.is_open())
		throw std::runtime_error("Cannot open config file");
	parseServerBlock(file);
}
void Config::parseServerBlock(std::ifstream &file)
{
	bool	inLocation;

	std::string line;
	std::string currentRoute;
	inLocation = false;
	while (std::getline(file, line))
	{
		std::istringstream iss(line);
		std::string token;
		iss >> token;
		// Skip empty or whitespace-only lines
		if (line.find_first_not_of(" \t\n\r") == std::string::npos)
			continue ;
		// Warn only if it's a directive that should end in semicolon
		if (line.find(';') == std::string::npos && token != "location" && token != "server"
			&& token != "}")
		{
			std::cerr << "⚠️  Config warning: missing ';' at end of line: " << line << std::endl;
		}
		if (token == "listen")
		{
			iss >> port;
		}
		else if (token == "root")
		{
			iss >> root;
			if (!root.empty() && root[root.size() - 1] == ';')
				root = root.substr(0, root.size() - 1);
		}
		else if (token == "location")
		{
			iss >> currentRoute;
			inLocation = true;
			routes[currentRoute] = Route();
		}
		else if (token == "}")
		{
			inLocation = false;
		}
		else if (token == "methods" && inLocation)
		{
			std::string method;
			while (iss >> method)
			{
				if (!method.empty() && method[method.size() - 1] == ';')
					method = method.substr(0, method.size() - 1);
				routes[currentRoute].methods.push_back(method);
			}
		}
	}
	std::cout << "[Config] Parsed settings:\n";
	std::cout << "  Port: " << port << "\n";
	std::cout << "  Root: " << root << "\n";
	for (std::map<std::string,
		Route>::iterator it = routes.begin(); it != routes.end(); ++it)
	{
		std::cout << "  Route: " << it->first << " Methods: ";
		for (size_t i = 0; i < it->second.methods.size(); ++i)
			std::cout << it->second.methods[i] << " ";
		std::cout << "\n";
	}
}

int Config::getPort() const
{
	return (port);
}
const std::string &Config::getRoot() const
{
	return (root);
}
const std::map<std::string, Route> &Config::getRoutes() const
{
	return (routes);
}