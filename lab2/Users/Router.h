#ifndef ROUTER_H
#define ROUTER_H
#include "Model.h"
#include "Users.h"
#include <Wt/WResource.h>
#include <Wt/WServer.h>

using namespace Wt;

class Router {
public:
    Router()
        : login(&model)
        , reg(&model)
        , del(&model)
        , incRating(&model)
    {
    }
    void route(WServer& server)
    {
        server.addResource(&login, "/login");
        server.addResource(&reg, "/register");
        server.addResource(&del, "/delete");
        server.addResource(&incRating, "/incRating");
    }

private:
    Login login;
    Reg reg;
    Del del;
    IncRating incRating;
    Model model;
};

#endif // ROUTER_H
