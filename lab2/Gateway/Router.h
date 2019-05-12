#ifndef ROUTER_H
#define ROUTER_H
#include "Comments.h"
#include "LikeHistory.h"
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
        , history(&model)
        , getAuthCode(&model)
        , getToken(&model)
        , refreshToken(&model)
#ifdef IS_TEST_BUILD
        , clear(&model)
#endif
    {
    }
    void route(WServer& server)
    {
        server.addResource(&titles, "/api/titles");
        server.addResource(&createComment, "/api/comment");
        server.addResource(&like, "/api/like");
        server.addResource(&login, "/api/login");
        server.addResource(&delUser, "/api/delete");
        server.addResource(&createNews, "/api/createNews");
        server.addResource(&reg, "/api/register");
        server.addResource(&oneNews, "/api/oneNews");
        server.addResource(&history, "/api/history");
        server.addResource(&getAuthCode, "/api/getAuthCode");
        server.addResource(&getToken, "/api/getToken");
        server.addResource(&refreshToken, "/api/refreshToken");
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
    History history;
    GetAuthCode getAuthCode;
    GetToken getToken;
    RefreshToken refreshToken;
#ifdef IS_TEST_BUILD
    Clear clear;
#endif
};

#endif // ROUTER_H
