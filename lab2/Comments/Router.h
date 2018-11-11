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
        , getComments(&model)
        , getCommentsById(&model)
#ifdef IS_TEST_BUILD
        , clear(&model)
#endif
    {
    }
    void route(WServer& server)
    {
        server.addResource(&createComment, "/comment");
        server.addResource(&countComment, "/count");
        server.addResource(&delComments, "/deleteByUser");
        server.addResource(&likeComment, "/like");
        server.addResource(&getComments, "/getComments");
        server.addResource(&getCommentsById, "/commentsById");
#ifdef IS_TEST_BUILD
        server.addResource(&clear, "/clear");
#endif
    }

private:
    CreateComment createComment;
    CountComment countComment;
    DeleteComments delComments;
    LikeComment likeComment;
    GetComments getComments;
    GetCommentsById getCommentsById;
    Model model;
#ifdef IS_TEST_BUILD
    Clear clear;
#endif
};

#endif // ROUTER_H
