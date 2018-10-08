#ifndef TESTS_H
#define TESTS_H
#include "../Gateway/Router.h"
#include "../Gateway/Users.h"
#include "../Shared/Database.h"
#include "../Shared/HttpAssist.h"
#include "fmt/format.h"
#include "gtest/gtest.h"
#include <chrono>
#include <thread>

bool runServer()
{
    static bool serverIsAlive = false;
    if (serverIsAlive)
        return serverIsAlive;

    Model model;
    int argc = 7;
    char* argv[7];
    argv[0] = "./tests";
    argv[1] = "--docroot";
    argv[2] = ".";
    argv[3] = "--http-address";
    argv[4] = "0.0.0.0";
    argv[5] = "--http-port";
    argv[6] = "8080";
    Wt::WServer* server = new Wt::WServer(argc, argv);
    Router* router = new Router;
    router->route(*server);

    std::thread t([&]() { server->run(); });
    t.detach();
    while (!server->isRunning()) {
    }
    serverIsAlive = true;
    HttpAssist::Client client;
    std::cout << "\nDATABASE2:" << Db::GetInst()->GetMysql();
    client.get("http://localhost:8080/clear");
    client.waitDone();

    return serverIsAlive;
}

TEST(Gateway, Users)
{
    runServer();

    json params;
    const std::string name = "test8";
    const std::string password = "test2";

    HttpAssist::Client client;
    params["name"] = name;
    params["password"] = password;

    //REGISTRATION_TEST
    Http::Message msgReg;
    msgReg.addBodyText(params.dump());
    client.post("http://localhost:8080/register", msgReg);
    client.waitDone();
    json resultReg = json::parse(client.message().body());
    int32_t userIdReg = resultReg["userId"].get<int32_t>();
    ASSERT_GE(userIdReg, 0);

    //LOGIN TEST

    Http::Message msgLogin;
    msgLogin.addBodyText(params.dump());
    client.post("http://localhost:8080/login", msgLogin);
    client.waitDone();
    json resultLogin = json::parse(client.message().body());
    int32_t userIdLogin = resultLogin["userId"].get<int32_t>();

    ASSERT_EQ(userIdReg, userIdLogin);

    //DELETE TEST

    Http::Message msgDelete;
    params.clear();
    params["userId"] = userIdLogin;
    msgDelete.addBodyText(params.dump());
    client.deleteRequest("http://localhost:8080/delete", msgDelete);
    client.waitDone();

    ASSERT_EQ(client.message().status(), 200);

    client.get("http://localhost:8080/clear");
    client.waitDone();
}
TEST(Gateway, News)
{
    runServer();

    json params;
    HttpAssist::Client client;

    //CREATENEWS_TEST
    Http::Message msgCreate;
    params["title"] = "title1";
    params["body"] = "body1";
    msgCreate.addBodyText(params.dump());
    client.post("http://localhost:8080/createNews", msgCreate);
    client.waitDone();
    json resultCreateNews = json::parse(client.message().body());
    int32_t newsId = resultCreateNews["newsId"].get<int32_t>();
    ASSERT_GE(newsId, 0);

    //GETTITLES_TEST
    for (int32_t i = 0; i < 15; ++i) {
        Http::Message msgCreate2;

        params["title"] = "title" + std::to_string(i);
        msgCreate2.addBodyText(params.dump());
        client.post("http://localhost:8080/createNews", msgCreate2);
        client.waitDone();
    }
    client.get(fmt::format("http://localhost:8080/titles?page={0}", 1));
    client.waitDone();
    json resultGetTitle = json::parse(client.message().body());
    ASSERT_EQ(resultGetTitle.size(), 10);

    client.get("http://localhost:8080/clear");
    client.waitDone();
}

TEST(Gateway, Comments)
{
    runServer();

    json params;
    HttpAssist::Client client;

    //COMMENTNEWS_TEST
    Http::Message msgComment;
    params["userId"] = 1;
    params["newsId"] = 1;
    params["text"] = "comment1";
    msgComment.addBodyText(params.dump());
    client.post("http://localhost:8080/comment", msgComment);
    client.waitDone();
    json resultComment = json::parse(client.message().body());
    int32_t commentId = resultComment["commentId"].get<int32_t>();
    ASSERT_GE(commentId, 0);

    //LIKECOMMENT_TEST
    ///registration
    Http::Message msgReg;
    params.clear();
    params["name"] = "test1";
    params["password"] = "test2";
    msgReg.addBodyText(params.dump());
    client.post("http://localhost:8080/register", msgReg);
    client.waitDone();
    json resultReg = json::parse(client.message().body());
    int32_t userIdReg = resultReg["userId"].get<int32_t>();
    ASSERT_GE(userIdReg, 0);

    ///like
    Http::Message msgLike;
    params.clear();
    params["commentId"] = commentId;
    params["userId"] = userIdReg;
    msgLike.addBodyText(params.dump());
    client.post("http://localhost:8080/like", msgLike);
    client.waitDone();
    json resultLike = json::parse(client.message().body());

    ASSERT_STREQ(resultLike["result"].get<std::string>().c_str(), "rating is updated!");
    ASSERT_EQ(client.message().status(), 200);

    client.get("http://localhost:8080/clear");
    client.waitDone();
}

#endif // TESTS_H
