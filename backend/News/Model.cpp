#include "Model.h"
#include "../Shared/Database.h"
#include "../Shared/JsonStructs.h"
using namespace ns;
using namespace Wt::Dbo;

Model::Model()
    : db(nullptr)
{
}

const json_t Model::getTitles(int32_t page)
{
    if (!db)
        db = Db::GetInst()->GetMysql();

    json_t result = json_t::array();

    auto req = db->prepareStatement("SELECT ID, title, UNIX_TIMESTAMP(creationDate) from News order by creationDate desc limit ? offset ?");
    req->bind(0, newsPerPage);
    req->bind(1, (page - 1) * newsPerPage);

    if (!req) {
        LOG_ERROR("Cant prepare statement");
    } else {
        req->execute();
        LOG_INFO("Db request %s", req->sql().c_str());
        while (req->nextRow()) {
            News news;
            req->getResult(0, &news.ID);
            req->getResult(1, &news.title, 255);
            req->getResult(2, &news.timestamp);
            result.push_back(news);
        };
    }
    return result;
}

const json_t Model::createNews(const std::string& title, const std::string& body)
{
    if (!db)
        db = Db::GetInst()->GetMysql();
    json_t result;
    auto req = db->prepareStatement("INSERT INTO News(title,body) VALUES(?, ?);");
    req->bind(0, title);
    req->bind(1, body);

    if (!req) {
        LOG_ERROR("Cant prepare statement");
    } else {
        try {
            LOG_INFO("Db request %s", req->sql().c_str());

            req->execute();
            result["newsId"] = req->insertedId();
        } catch (...) {
            LOG_INFO("Cant insert news!");
        }
    };

    return result;
}

const json_t Model::getNews(int32_t newsId)
{
    if (!db)
        db = Db::GetInst()->GetMysql();
    json_t result;
    std::cout << "BEFORE PREPARED STATEMENT" << std::endl;
    auto req = db->prepareStatement("SELECT ID, title,body, UNIX_TIMESTAMP(creationDate) from News where ID=?");
    std::cout << "AFTER AFTER STATEMENT" << std::endl;

    req->bind(0, newsId);
    std::cout << "ADTER BIND" << std::endl;

    if (!req) {
        LOG_ERROR("Cant prepare statement");
    } else {
        try {
            req->execute();
            LOG_INFO("Db request %s newsId %d", req->sql().c_str(), newsId);
            if (req->nextRow()) {
                News news;
                req->getResult(0, &news.ID);
                req->getResult(1, &news.title, 255);
                req->getResult(2, &news.body, 255);
                req->getResult(3, &news.timestamp);
                result = news;
            };
        } catch (...) {
            LOG_INFO("Cant find news news!");
        }
    };

    return result;
}
#ifdef IS_TEST_BUILD
const json Model::clear()
{
    if (!db)
        db = Db::GetInst()->GetMysql();
    json result;
    auto req = db->prepareStatement("DELETE  from News;");

    if (!req) {
        LOG_ERROR("Cant prepare statement");
    } else {
        LOG_INFO("Db request %s", req->sql().c_str());
        try {
            req->execute();
            result["result"] = "news is dropped!";
        } catch (...) {
            LOG_INFO("cant drop News!");
        }
    }
    return result;
}
#endif
