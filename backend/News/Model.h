#ifndef MODEL_H
#define MODEL_H
#include "../Shared/Database.h"
#include "../Shared/HttpAssist.h"
#include <Wt/Http/Client>
#include <Wt/Http/Response>
#include <mutex>
#include <string>
using nlohmann::json;

class Model {
public:
    Model();
    const json_t getTitles(int32_t page);
    const json_t createNews(const std::string& title, const std::string& body);
    const json_t getNews(int32_t newsId);
    static std::string getSecretKey() { return "news"; }
#ifdef IS_TEST_BUILD
    const json clear();
#endif
private:
    Model(const Model&) = delete;
    Dbo::backend::MySQL* db;
    const int32_t newsPerPage = 10;
    std::mutex mutex;
};

#endif // MODEL_H
