#define _GLIBCXX_USE_CXX11_ABI 1
#include <Wt/WServer.h>
#include <Wt/WString.h>
#include <Wt/WResource.h>

int main(int argc, char *argv[])
{


  try {
    Wt::WServer server(argv[0]);


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
