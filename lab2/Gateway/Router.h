#ifndef ROUTER_H
#define ROUTER_H
#include "Comments.h"
#include "News.h"
#include "Users.h"
#include <Wt/WResource>
#include <Wt/WServer>

using namespace Wt;

class Router {
public:
    Router()
        : titles(&model)
        , createComment(&model)
        , like(&model)
        , login(&model)
        , reg(&model)
        , delUser(&model)
        , createNews(&model)
    {
    }
    void route(WServer& server)
    {
        server.addResource(&titles, "/titles");
        server.addResource(&createComment, "/comment");
        server.addResource(&like, "/like");
        server.addResource(&login, "/login");
        server.addResource(&delUser, "/delete");
        server.addResource(&createNews, "/createNews");
        server.addResource(&reg, "/register");
    }

private:
    GetTitles titles;
    CreateComment createComment;
    Like like;
    Login login;
    Register reg;
    DelUser delUser;
    CreateNews createNews;
    Model model;
};

#endif // ROUTER_H
