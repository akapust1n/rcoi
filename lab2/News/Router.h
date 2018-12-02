#ifndef ROUTER_H
#define ROUTER_H
#include "Model.h"
#include "News.h"
#include <Wt/WResource.h>
#include <Wt/WServer.h>

using namespace Wt;

class Router {
public:
    Router()
        : getTitles(&model)
        , createNews(&model)
        , getNews(&model)
        , authService(&model, Model::getSecretKey())
#ifdef IS_TEST_BUILD
        , clear(&model)
#endif
    {
    }
    void route(WServer& server)
    {
        server.addResource(&getTitles, "/titles");
        server.addResource(&createNews, "/createNews");
        server.addResource(&getNews, "/getnews");
        server.addResource(&authService, "/authService");

#ifdef IS_TEST_BUILD
        server.addResource(&clear, "/clear");
#endif
    }

private:
    GetTitles getTitles;
    CreateNews createNews;
    GetNews getNews;
    Model model;
    AuthService authService;
#ifdef IS_TEST_BUILD
    Clear clear;
#endif
};

#endif // ROUTER_H
