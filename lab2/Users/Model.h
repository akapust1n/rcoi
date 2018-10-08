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
    const json reg(const std::string& name, const std::string& pwd);
    const json login(const std::string& name, const std::string& pwd);
    const json del(int32_t userId);
    const json incRating(int32_t userId);
#ifdef IS_TEST_BUILD
    const json clear();
#endif

private:
    Model(const Model&) = delete;
    Dbo::backend::MySQL* db;
};

#endif // MODEL_H
