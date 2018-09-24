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
        : getNews(&model)
    {
    }
    void route(WServer& server)
    {
        server.addResource(&getNews, "/news");
    }

private:
    GetNews getNews;
    Model model;
};

#endif // ROUTER_H
