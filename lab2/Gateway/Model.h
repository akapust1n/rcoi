#ifndef MODEL_H
#define MODEL_H
#include "../Shared/HttpAssist.h"
#include <Wt/Http/Client>
#include <Wt/Http/Response>
#include <string>

class Model {
public:
    Model() = default;
    Model(const Model&) = delete;
    const Wt::Http::Message& getfromNews(const std::vector<Wt::Http::Message::Header>& headers, const std::string& params, const std::string& path);

private:
    const std::string urlNews = HttpAssist::skUrlNews;
    const std::string urlComments = "http://localhost:8081";
    const std::string urlUsers = "http://localhost:8081";
};

#endif // MODEL_H
