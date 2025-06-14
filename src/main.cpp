#include "headers.hpp"

int main(int argc, char **argv) {
    std::string config = (argc > 1) ? argv[1] : "default.conf";

    try {
        Server server(config);
        server.run();
    } catch (const std::exception &e) {
        std::cerr << "Fatal error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
