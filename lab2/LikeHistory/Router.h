#ifndef ROUTER_H
#define ROUTER_H
#include "LikeHistory.h"
#include "Model.h"
#include <Wt/WResource.h>
#include <Wt/WServer.h>

using namespace Wt;

class Router {
public:
    Router()
        : getLikes(&model)
        , writeLike(&model)
    {
    }
    void route(WServer& server)
    {
        server.addResource(&getLikes, "/getLikes");
        server.addResource(&writeLike, "/writeLike");
    }

private:
    GetLikes getLikes;
    WriteLike writeLike;
    Model model;
};

#endif // ROUTER_H
