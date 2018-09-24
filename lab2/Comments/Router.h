#ifndef ROUTER_H
#define ROUTER_H
#include "HelloWorld.h"
#include <Wt/WResource>
#include <Wt/WServer>
#include "Sum.h"

using namespace Wt;

class Router {
public:
    void route(WServer& server)
    {
        server.addResource(&helloWorld, "/hello");
        server.addResource(&sum, "/sum");
    }

private:
    GetNews helloWorld;
    Sum sum;

};

#endif // ROUTER_H
