// main.cpp
#include <httplib.h>
#include <iostream>

int main() {
    httplib::Server server;

    server.Get(".*", [](const httplib::Request &, httplib::Response &res) {
        res.set_content(R"({"Message": "OK"})", "application/json");
    });

    std::cout << "Starting server on port 8080..." << std::endl;
    server.listen("0.0.0.0", 8080);

    return 0;
}
