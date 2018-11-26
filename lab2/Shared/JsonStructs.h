#ifndef jsonStructs_H
#define jsonStructs_H
#include "jwt/jwt.hpp"
#include <cstdlib>
#include <nlohmann/json.hpp>
#include <string>

namespace ns {
struct News {
    int32_t ID = 0;
    std::string title;
    std::string body;
    long long timestamp;
};

inline void to_json(json_t& j, const News& p)
{
    j = json_t{ { "ID", p.ID }, { "title", p.title }, { "body", p.body }, { "timestamp", p.timestamp } };
}

inline bool from_json(const json_t& j, News& p)
{
    try {
        auto titleIt = j.find("title");
        auto bodyit = j.find("body");
        if (titleIt != j.cend() && bodyit != j.cend()) {
            p.body = bodyit.value().get<std::string>();
            p.title = titleIt.value().get<std::string>();
            return true;
        }
    } catch (...) {
    }

    return false;
}

struct Title {
    int32_t ID = 0;
    std::string title;
    int32_t count = 0;
    long long timestamp;
};

inline void to_json(json_t& j, const Title& p)
{
    j = json_t{ { "ID", p.ID }, { "title", p.title }, { "count", p.count }, { "timestamp", p.timestamp } };
}

struct Comment {
    int32_t newsId = 0;
    int32_t userId = 0;
    std::string text;
};

inline void to_json(json_t& j, const Comment& p)
{
    j = json_t{ { "newsId", p.newsId }, { "userId", p.userId }, { "text", p.text } };
}
inline bool from_json(const json_t& j, Comment& p)
{
    try {
        auto userIdIt = j.find("userId");
        auto newsIdIt = j.find("newsId");
        auto textIt = j.find("text");

        if (userIdIt != j.cend() && newsIdIt != j.cend() && textIt != j.end()) {
            p.userId = userIdIt.value().get<int32_t>();
            p.newsId = newsIdIt.value().get<int32_t>();
            p.text = textIt.value().get<std::string>();
            return true;
        }
    } catch (...) {
    }
    return false;
}

struct UserAuth {
    std::string name;
    std::string password;
};

inline void to_json(json_t& j, const UserAuth& p)
{
    j = json_t{ { "name", p.name }, { "password", p.password } };
}
inline bool from_json(const json_t& j, UserAuth& p)
{
    try {
        auto nameIt = j.find("name");
        auto pwdIt = j.find("password");

        if (nameIt != j.end() && pwdIt != j.end()) {
            p.name = nameIt.value().get<std::string>();
            p.password = pwdIt.value().get<std::string>();
            return true;
        }
    } catch (...) {
    }
    return false;
}

struct CommentInternal {
    int32_t commentId;
    int32_t userId;
    std::string body;
    int32_t rating;
};

inline void to_json(json_t& j, const CommentInternal& p)
{
    j = json_t{ { "userId", p.userId }, { "commentId", p.commentId }, { "body", p.body }, { "rating", p.rating } };
}
inline bool from_json(const json_t& j, CommentInternal& p)
{
    try {
        auto userIdIt = j.find("userId");
        auto commentIdIt = j.find("commentId");
        auto textIt = j.find("body");
        auto ratingIt = j.find("rating");

        if (userIdIt != j.end() && commentIdIt != j.end() && textIt != j.end() && ratingIt != j.cend()) {
            p.userId = userIdIt.value().get<int32_t>();
            p.commentId = commentIdIt.value().get<int32_t>();
            p.body = textIt.value().get<std::string>();
            p.rating = ratingIt.value().get<int32_t>();
            return true;
        }
    } catch (...) {
    }
    return false;
}

struct CommentExternal {
    int32_t commentId;
    std::string name;
    std::string body;
    int32_t rating;
    int32_t userId;
};

inline void to_json(json_t& j, const CommentExternal& p)
{
    j = json_t{ { "commentId", p.commentId }, { "name", p.name }, { "body", p.body }, { "rating", p.rating }, { "userId", p.userId } };
}
inline bool from_json(const json_t& j, CommentExternal& p)
{
    try {
        auto nameIt = j.find("name");
        auto commentIdIt = j.find("commentId");
        auto textIt = j.find("body");
        auto ratingIt = j.find("rating");
        auto userIdIt = j.find("userId");

        if (nameIt != j.end() && commentIdIt != j.end() && textIt != j.end()
            && ratingIt != j.cend() && userIdIt != j.cend()) {
            p.name = nameIt.value().get<std::string>();
            p.commentId = commentIdIt.value().get<int32_t>();
            p.body = textIt.value().get<std::string>();
            p.rating = ratingIt.value().get<int32_t>();
            p.userId = userIdIt.value().get<int32_t>();
            return true;
        }
    } catch (...) {
    }
    return false;
}
struct LikeEntity {
    int32_t commentId;
    int32_t userId;
    long long timestamp;
};

inline void to_json(json_t& j, const LikeEntity& p)
{
    j = json_t{ { "commentId", p.commentId }, { "userId", p.userId }, { "timestamp", p.timestamp } };
}

inline bool from_json(const json_t& j, LikeEntity& p)
{
    try {
        auto commentIdIt = j.find("commentId");
        auto userIdIt = j.find("userId");
        if (commentIdIt != j.cend() && userIdIt != j.cend()) {
            p.commentId = commentIdIt.value().get<int32_t>();
            p.userId = userIdIt.value().get<int32_t>();
            return true;
        }
    } catch (...) {
    }

    return false;
}
struct LikeEntityExternal {
    std::string comment;
    std::string name;
    long long timestamp;
};

inline void to_json(json_t& j, const LikeEntityExternal& p)
{
    j = json_t{ { "comment", p.comment }, { "name", p.name }, { "timestamp", p.timestamp } };
}
}

#endif // jsonStructs_H
