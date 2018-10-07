#ifndef MODEL_H
#define MODEL_H
#include "../Shared/Database.h"
#include "../Shared/HttpAssist.h"
#include <Wt/Http/Client>
#include <Wt/Http/Response>
#include <string>
using nlohmann::json;

class Model {
public:
    Model();
    const json getTitles(int32_t numTitles);
    const json createNews(const std::string& title, const std::string& body);
    const json getNews(int32_t newsId);

private:
    Model(const Model&) = delete;
    Dbo::backend::MySQL* db;
};

#endif // MODEL_H
