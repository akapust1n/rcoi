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
    const json_t createComment(int32_t userId, int32_t newsId, const std::string text);
    const json_t countComments(const std::vector<int32_t>& ids);
    const json_t deleteComments(int32_t userId);
    const json_t likeComment(int32_t commentId);
    const json_t getComments(int32_t newsId, int32_t page);
    const json_t getCommentsById(const std::vector<int32_t>& ids);
#ifdef IS_TEST_BUILD
    const json clear();
#endif
private:
    Model(const Model&) = delete;
    Dbo::backend::MySQL* db;
    const int32_t commentsPerPage = 10;
};

#endif // MODEL_H
