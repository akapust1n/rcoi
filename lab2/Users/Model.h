#ifndef MODEL_H
#define MODEL_H
#include "../Shared/Database.h"
#include "../Shared/HttpAssist.h"
#include "../Shared/JsonStructs.h"
#include <Wt/Http/Client>
#include <Wt/Http/Response>
#include <string>

using nlohmann::json;

class Model {
public:
    Model();
    const json_t reg(const std::string& name, const std::string& pwd);
    const json_t login(const std::string& name, const std::string& pwd);
    const json_t del(int32_t userId);
    const json_t incRating(int32_t userId);
    const json_t getNames(const std::vector<int32_t>& ids);
    int32_t getAuthCode(int32_t clientId);
    json_t getToken(int32_t clientId, int32_t authCode);
    uint32_t checkAuth(uint32_t userId, const std::string& token);
    json_t refreshToken(const std::string& token);
#ifdef IS_TEST_BUILD
    const json clear();
#endif

private:
    Model(const Model&) = delete;
    Dbo::backend::MySQL* db;
};
const std::string key = "secret";
using namespace jwt::params;

#endif // MODEL_H
