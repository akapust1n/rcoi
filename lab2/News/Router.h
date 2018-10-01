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
    {
    }
    void route(WServer& server)
    {
        server.addResource(&getTitles, "/titles");
        server.addResource(&createNews, "/createNews");
    }

private:
    GetTitles getTitles;
    CreateNews createNews;
    Model model;
};

#endif // ROUTER_H
