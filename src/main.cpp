#include <drogon/drogon.h>

#include <exception>
#include <iostream>

int main()
{
    try
    {
        std::cout << "Starting Athena Command Intelligence Engine" << std::endl;
        std::cout << "Listening on http://127.0.0.1:8848" << std::endl;

        drogon::app()
            .addListener("127.0.0.1", 8848)
            .setThreadNum(4)
            .setClientMaxBodySize(1024 * 1024)
            .run();

        return 0;
    }
    catch (const std::exception& error)
    {
        std::cerr << "Athena failed to start: " << error.what() << std::endl;

        return 1;
    }
}