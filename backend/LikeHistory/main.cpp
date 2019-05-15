#include "../Shared/Congif.h"
#include "../Shared/Database.h"
#include "Router.h"
#include <Wt/WResource.h>
#include <Wt/WServer.h>
#include <Wt/WString.h>

int main(int argc, char* argv[])
{

    try {
        Wt::WServer server(argc, argv);
        Router router;
        router.route(server);
        if (!Db::GetInst()->Init(Config::GetInst()->getLikeHistoryDb())) {
            LOG_ERROR("Cant connect to database");
            return 1;
        }
        server.run();
    } catch (const Wt::WServer::Exception& e) {
        std::cerr << e.what() << '\n';
        LOG_ERROR("%s", e.what());
        return 1;
    } catch (const std::exception& e) {
        LOG_ERROR("%s", e.what());
        return 1;
    }
    return 0;
}
