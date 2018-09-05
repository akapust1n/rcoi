#include "Router.h"
#include <Wt/WServer>
#include <iostream>

using namespace std;

int main(int argc, char** argv)
{
    try {
        WServer server(argv[0]);
        server.setServerConfiguration(argc, argv); //создаем сервер

        Router router; //роутер
        router.route(server);

        if (server.start()) {
            WServer::waitForShutdown();
            server.stop();
        }
    } catch (WServer::Exception& e) {
        std::cerr << e.what() << std::endl;
        std::cout << "why2";
    } catch (std::exception& e) {
        std::cerr << "exception: " << e.what() << std::endl;
    } catch (...) {
        std::cout << "why";
    }
}
