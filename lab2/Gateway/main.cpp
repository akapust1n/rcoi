#include <Wt/WServer.h>
#include <Wt/WString.h>
#include <Wt/WResource.h>
#include "Router.h"

int main(int argc, char *argv[])
{

  try {
    Wt::WServer server(argc,argv);
    Router router;
    router.route(server);

    server.run();
  } catch (const Wt::WServer::Exception &e) {
    std::cerr << e.what() << '\n';
    return 1;
  } catch (const std::exception &e) {
    std::cerr << "exception: " << e.what() << '\n';
    return 1;
  }
    return 0;
}
