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

struct CommentInternal {
    int32_t commentId;
    int32_t userId;
    std::string body;
};

inline void to_json(json& j, const CommentInternal& p)
{
    j = json{ { "userId", p.userId }, { "commentId", p.commentId }, { "body", p.body } };
}
inline bool from_json(const json& j, CommentInternal& p)
{
    auto userIdIt = j.find("userId");
    auto commentIdIt = j.find("commentId");
    auto textIt = j.find("body");

    if (userIdIt != j.end() && commentIdIt != j.end() && textIt != j.end()) {
        p.userId = userIdIt.value().get<int32_t>();
        p.commentId = commentIdIt.value().get<int32_t>();
        p.body = textIt.value().get<std::string>();
        return true;
    }
    return false;
}

struct CommentExternal {
    int32_t commentId;
    std::string name;
    std::string body;
};

inline void to_json(json& j, const CommentExternal& p)
{
    j = json{ { "commentId", p.commentId }, { "name", p.name }, { "body", p.body } };
}
inline bool from_json(const json& j, CommentExternal& p)
{
    auto nameIt = j.find("name");
    auto commentIdIt = j.find("commentId");
    auto textIt = j.find("body");

    if (nameIt != j.end() && commentIdIt != j.end() && textIt != j.end()) {
        p.name = nameIt.value().get<std::string>();
        p.commentId = commentIdIt.value().get<int32_t>();
        p.body = textIt.value().get<std::string>();
        return true;
    }
    return false;
}
}

#endif // JsonStructs_H
