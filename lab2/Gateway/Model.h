#ifndef MODEL_H
#define MODEL_H
#include "../Shared/HttpAssist.h"
#include <Wt/Http/Client>
#include <Wt/Http/Response>
#include <array>
#include <string>

class Model {
    enum Service {
        News = 0,
        Comments,
        Users,

        ServiceCount
    };

public:
    Model() = default;
    Model(const Model&) = delete;

    const Wt::Http::Message getTitles(const std::vector<Wt::Http::Message::Header>& headers, const std::string& params);
    const Wt::Http::Message createComment(const std::vector<Wt::Http::Message::Header>& headers, const std::string& body);
    const Wt::Http::Message createNews(const std::vector<Wt::Http::Message::Header>& headers, const std::string& body);
    const Wt::Http::Message login(const std::vector<Wt::Http::Message::Header>& headers, const std::string& body);
    const Wt::Http::Message reg(const std::vector<Wt::Http::Message::Header>& headers, const std::string& body);
    const Wt::Http::Message del(const std::vector<Wt::Http::Message::Header>& headers, const std::string& body);
    const Wt::Http::Message like(const std::vector<Wt::Http::Message::Header>& headers, const std::string& body);

private:
    const Wt::Http::Message deletefromService(Service service, const std::vector<Wt::Http::Message::Header>& headers, const std::string& body, const std::string& path);
    const Wt::Http::Message getfromService(Service service, const std::vector<Wt::Http::Message::Header>& headers, const std::string& params, const std::string& path);
    const Http::Message postToService(Service service, const std::vector<Wt::Http::Message::Header>& headers, const std::string& body, const std::string& path);

private:
    std::array<std::string, ServiceCount> skServicePaths = { HttpAssist::skUrlNews, HttpAssist::skUrlComments, HttpAssist::skUrlUsers };
};

#endif // MODEL_H
