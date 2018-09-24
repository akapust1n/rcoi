#ifndef ROUTER_H
#define ROUTER_H
#include "News.h"
#include <Wt/WResource>
#include <Wt/WServer>

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
