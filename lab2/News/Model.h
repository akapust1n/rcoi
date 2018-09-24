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
    const json getNews(int32_t numNews);

private:
    Model(const Model&) = delete;
    Dbo::backend::MySQL* db;
};

namespace ns {
struct News {
    int32_t ID;
    std::string title;
    std::string body;
    std::string timestamp;
};
inline void to_json(json& j, const News& p)
{
    j = json{ { "ID", p.ID }, { "title", p.title }, { "body", p.body }, { "timestamp", p.timestamp } };
}
}

#endif // MODEL_H
