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
    const json createComment(int32_t userId, int32_t newsId, const std::string text);
    const json countComments(const std::vector<int32_t>& ids);
    const json deleteComments(int32_t userId);
    const json likeComment(int32_t commentId);
    const json getComments(int32_t newsId, int32_t page);

private:
    Model(const Model&) = delete;
    Dbo::backend::MySQL* db;
    const int32_t commentsPerPage = 10;
};

#endif // MODEL_H