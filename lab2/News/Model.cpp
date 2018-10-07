#include "Model.h"
#include "../Shared/Database.h"
#include "../Shared/JsonStructs.h"
using namespace ns;
using namespace Wt::Dbo;

Model::Model()
    : db(nullptr)
{
}

const json Model::getTitles(int32_t numTitles)
{
    if (!db)
        db = Db::GetInst()->GetMysql();

    json result = json::array();
    auto req = db->prepareStatement("SELECT ID, title, UNIX_TIMESTAMP(creationDate) from News order by creationDate limit ?");
    req->bind(0, numTitles);

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

const json Model::createNews(const std::string& title, const std::string& body)
{
    if (!db)
        db = Db::GetInst()->GetMysql();
    json result;
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

const json Model::getNews(int32_t newsId)
{
    if (!db)
        db = Db::GetInst()->GetMysql();
    json result;

    auto req = db->prepareStatement("ID, title, UNIX_TIMESTAMP(creationDate) from News where ID=?");
    req->bind(0, newsId);

    if (!req) {
        LOG_ERROR("Cant prepare statement");
    } else {
        try {
            req->execute();
            LOG_INFO("Db request %s", req->sql().c_str());
            if (req->nextRow()) {
                News news;
                req->getResult(0, &news.ID);
                req->getResult(1, &news.title, 255);
                req->getResult(2, &news.timestamp);
                result = news;
            };
        } catch (...) {
            LOG_INFO("Cant insert news!");
        }
    };

    return result;
}
