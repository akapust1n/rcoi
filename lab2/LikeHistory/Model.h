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
    const json_t getLikes(int32_t page);
    const json_t writeLike(int32_t userId, int32_t commentId);

private:
    Model(const Model&) = delete;
    Dbo::backend::MySQL* db;
    const int32_t likesPerPage = 10;
};

#endif // MODEL_H
