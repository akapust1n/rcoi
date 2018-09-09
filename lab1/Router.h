#ifndef ROUTER_H
#define ROUTER_H
#include "HelloWorld.h"
#include <Wt/WResource>
#include <Wt/WServer>

using namespace Wt;

class Router {
public:
    void route(WServer& server)
    {
        server.addResource(&helloWorld, "/hello");
    }

private:
    HelloWorld helloWorld;
};

#endif // ROUTER_H
