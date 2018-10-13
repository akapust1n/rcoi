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
        , oneNews(&model)
#ifdef IS_TEST_BUILD
        , clear(&model)
#endif
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
        server.addResource(&oneNews, "/oneNews");
#ifdef IS_TEST_BUILD
        server.addResource(&clear, "/clear");
#endif
    }

private:
    GetTitles titles;
    CreateComment createComment;
    Like like;
    Login login;
    Register reg;
    DelUser delUser;
    CreateNews createNews;
    OneNews oneNews;
    Model model;
#ifdef IS_TEST_BUILD
    Clear clear;
#endif
};

#endif // ROUTER_H
