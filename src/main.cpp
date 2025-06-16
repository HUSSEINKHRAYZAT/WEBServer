#include "headers.hpp"

int main(int argc, char **argv) {
    // if (argc != 2) {
    //     std::cerr << "Usage: " << argv[0] << " [configuration file]" << std::endl;
    //     return 1;
    // }

    std::string config = (argc == 1) ? "default.conf" :  argv[1];

    try {
        std::cout << "Launching server with config: " << config << std::endl;
        Server server(config);
        server.run();
    } catch (const std::exception &e) {
        std::cerr << "Fatal error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
