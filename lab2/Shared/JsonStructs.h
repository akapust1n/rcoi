#ifndef JsonStructs_H
#define JsonStructs_H
#include <cstdlib>
#include <nlohmann/json.hpp>
#include <string>
using json = nlohmann::json;

namespace ns {
struct News {
    int32_t ID = 0;
    std::string title;
    std::string body;
    long long timestamp;
};

inline void to_json(json& j, const News& p)
{
    j = json{ { "ID", p.ID }, { "title", p.title }, { "body", p.body }, { "timestamp", p.timestamp } };
}

inline bool from_json(const json& j, News& p)
{
    auto titleIt = j.find("title");
    auto bodyit = j.find("body");
    if (titleIt != j.cend() && bodyit != j.cend()) {
        p.body = bodyit.value().get<std::string>();
        p.title = titleIt.value().get<std::string>();
        return true;
    }
    return false;
}

struct Title {
    int32_t ID = 0;
    std::string title;
    int32_t count = 0;
};

inline void to_json(json& j, const Title& p)
{
    j = json{ { "ID", p.ID }, { "title", p.title }, { "count", p.count } };
}

struct Comment {
    int32_t newsId = 0;
    int32_t userId = 0;
    std::string text;
};

inline void to_json(json& j, const Comment& p)
{
    j = json{ { "newsId", p.newsId }, { "userId", p.userId }, { "text", p.text } };
}
inline bool from_json(const json& j, Comment& p)
{
    auto userIdIt = j.find("userId");
    auto newsIdIt = j.find("newsId");
    auto textIt = j.find("text");

    if (userIdIt != j.cend() && newsIdIt != j.cend() && textIt != j.end()) {
        p.userId = userIdIt.value().get<int32_t>();
        p.newsId = newsIdIt.value().get<int32_t>();
        p.text = textIt.value().get<std::string>();
        return true;
    }
    return false;
}

struct LikeJs {
    int32_t entity = 0;
    int32_t entityId = 0;
};

inline void to_json(json& j, const LikeJs& p)
{
    j = json{ { "entity", p.entity }, { "entityId", p.entityId } };
}
inline bool from_json(const json& j, LikeJs& p)
{
    auto entityIt = j.find("entity");
    auto entityIdIt = j.find("entityId");

    if (entityIt != j.cend() && entityIdIt != j.cend()) {
        p.entity = entityIt.value().get<int32_t>();
        p.entityId = entityIdIt.value().get<int32_t>();
        return true;
    }
    return false;
}

struct UserAuth {
    std::string name;
    std::string password;
};

inline void to_json(json& j, const UserAuth& p)
{
    j = json{ { "name", p.name }, { "password", p.password } };
}
inline bool from_json(const json& j, UserAuth& p)
{
    auto nameIt = j.find("name");
    auto pwdIt = j.find("password");

    if (nameIt != j.end() && pwdIt != j.end()) {
        p.name = nameIt.value().get<std::string>();
        p.password = pwdIt.value().get<std::string>();
        return true;
    }
    return false;
}
}

#endif // JsonStructs_H
