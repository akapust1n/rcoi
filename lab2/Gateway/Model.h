#ifndef MODEL_H
#define MODEL_H
#include "../Shared/HttpAssist.h"
#include "../Shared/JsonStructs.h"
#include "TaskQueue.h"
#include <Wt/Http/Client>
#include <Wt/Http/Response>
#include <array>
#include <mutex>
#include <string>

class Model {

public:
    Model() = default;
    Model(const Model&) = delete;

    const Wt::Http::Message getTitles(const std::vector<Wt::Http::Message::Header>& headers, const std::string& params);
    const Wt::Http::Message createComment(const std::vector<Wt::Http::Message::Header>& headers, const std::string& body);
    const Wt::Http::Message createNews(const std::vector<Wt::Http::Message::Header>& headers, const std::string& body);
    const Wt::Http::Message login(const std::vector<Wt::Http::Message::Header>& headers, const std::string& body);
    const Wt::Http::Message reg(const std::vector<Wt::Http::Message::Header>& headers, const std::string& body);
    const Wt::Http::Message del(const std::vector<Wt::Http::Message::Header>& headers, const std::string& body);
    const Wt::Http::Message getAuthCode(const std::vector<Wt::Http::Message::Header>& headers, const std::string& params);
    const Wt::Http::Message getToken(const std::vector<Wt::Http::Message::Header>& headers, const std::string& params);
    const Wt::Http::Message refreshToken(const std::vector<Wt::Http::Message::Header>& headers, const std::string& body);
    const Wt::Http::Message like(const std::vector<Wt::Http::Message::Header>& headers, const std::string& body);
    const Wt::Http::Message getOneNews(const std::vector<Wt::Http::Message::Header>& headers, const std::string& params);
    const Wt::Http::Message history(const std::vector<Wt::Http::Message::Header>& headers, const std::string& params);
    bool checkAuth(const std::vector<Wt::Http::Message::Header>& headers, uint32_t& userId, uint64_t accessMask);
#ifdef IS_TEST_BUILD
    const Wt::Http::Message clear(const std::vector<Wt::Http::Message::Header>& headers);
#endif
private:
    const Wt::Http::Message deletefromService(Services::Service service, const std::vector<Wt::Http::Message::Header>& headers, const std::string& body, const std::string& path);
    const Wt::Http::Message getfromService(Services::Service service, const std::vector<Wt::Http::Message::Header>& headers, const std::string& params, const std::string& path);
    const Http::Message postToService(Services::Service service, const std::vector<Wt::Http::Message::Header>& headers, const std::string& body, const std::string& path);
    void getAuthService(std::vector<Http::Message::Header>& headers, Services::Service service);

private:
    std::array<std::string, Services::ServiceCount> skServicePaths = { HttpAssist::skUrlNews, HttpAssist::skUrlComments, HttpAssist::skUrlUsers, HttpAssist::skUrlLikeHistory };
    std::map<Services::Service, std::string> serviceTokens;
    std::map<Services::Service, std::string> secretServiceStrings = { { Services::News, "news" }, { Services::Comments, "comments" },
        { Services::Users, "users" }, { Services::LikeHistory, "likehistory" } };
};

#endif // MODEL_H
