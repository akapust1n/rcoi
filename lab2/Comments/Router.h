#ifndef ROUTER_H
#define ROUTER_H
#include "Comments.h"
#include "Model.h"
#include <Wt/WResource.h>
#include <Wt/WServer.h>

using namespace Wt;

class Router {
public:
    Router()
        : createComment(&model)
        , countComment(&model)
        , delComments(&model)
        , likeComment(&model)
    {
    }
    void route(WServer& server)
    {
        server.addResource(&createComment, "/comment");
        server.addResource(&countComment, "/count");
        server.addResource(&delComments, "/deleteByUser");
        server.addResource(&likeComment, "/like");
    }

private:
    CreateComment createComment;
    CountComment countComment;
    DeleteComments delComments;
    LikeComment likeComment;
    Model model;
};

#endif // ROUTER_H
