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
#ifdef IS_TEST_BUILD
        , clear(&model)
#endif
    {
    }
    void route(WServer& server)
    {
        server.addResource(&login, "/login");
        server.addResource(&reg, "/register");
        server.addResource(&del, "/delete");
        server.addResource(&incRating, "/incRating");
#ifdef IS_TEST_BUILD
        server.addResource(&clear, "/clear");
#endif
    }

private:
    Login login;
    Reg reg;
    Del del;
    IncRating incRating;
    Model model;
#ifdef IS_TEST_BUILD
    Clear clear;
#endif
};

#endif // ROUTER_H
