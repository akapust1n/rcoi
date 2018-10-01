#include "Model.h"
#include "../Shared/Database.h"
#include "../Shared/JsonStructs.h"
using namespace ns;
using namespace Wt::Dbo;

Model::Model()
    : db(nullptr)
{
}

const json Model::createComment(int32_t userId, int32_t newsId, const std::string text)
{
    if (!db)
        db = Db::GetInst()->GetMysql();
    json result;
    auto req = db->prepareStatement("INSERT INTO Comments(newsId, userId, body) VALUES(?, ?, \"?\"); SELECT"
                                    " LAST_INSERT_ID();");
    req->bind(0, newsId);
    req->bind(1, userId);
    req->bind(2, text);

    int32_t commentId;
    if (!req) {
        LOG_ERROR("Cant prepare statement");
    } else {
        try {
            LOG_INFO("Db request %s", req->sql().c_str());
            req->execute();
            req->nextRow();
            req->getResult(0, &commentId);
            if (req->nextRow())
                return json();
            result["commentId"] = commentId;
        } catch (...) {
            LOG_INFO("Cant create comment!");
        }
    };

    return result;
}

const json Model::countComments(const std::vector<int32_t>& ids)
{
    if (!db)
        db = Db::GetInst()->GetMysql();
    json result;
    for (auto id : ids) {
        auto req = db->prepareStatement("SELECT COUNT(*) FROM Comments WHERE newsId=?;");
        req->bind(0, id);

        if (!req) {
            LOG_ERROR("Cant prepare statement");
        } else {
            try {
                int32_t count;
                json temp;
                req->execute();
                LOG_INFO("Db request %s", req->sql().c_str());
                req->nextRow();
                req->getResult(0, &count);
                temp["count"] = count;
                temp["id"] = id;
                result.push_back(temp);

            } catch (...) {
                LOG_ERROR("Cant count comments");
            }
        };
    }

    return result;
}

const json Model::deleteComments(int32_t userId)
{
    if (!db)
        db = Db::GetInst()->GetMysql();
    json result;

    auto req = db->prepareStatement("DELETE FROM Comments WHERE userId=?;");
    req->bind(0, userId);

    if (!req) {
        LOG_ERROR("Cant prepare statement");
    } else {
        try {
            req->execute();
            LOG_INFO("Db request %s", req->sql().c_str());
            result["result"] = "comments are deleted!";
        } catch (...) {
            LOG_ERROR("Cant delete comments!");
        }
    };

    return result;
}

const json Model::likeComment(int32_t commentId)
{
    if (!db)
        db = Db::GetInst()->GetMysql();
    json result;

    auto req = db->prepareStatement("UPDATE Comments SET rating = rating+1 WHERE ID=?;");
    req->bind(0, commentId);

    if (!req) {
        LOG_ERROR("Cant prepare statement");
    } else {
        try {
            req->execute();
            result["result"] = "rating is updated!";
        } catch (...) {
            LOG_ERROR("Cant like comment!");
        }
    };

    return result;
}
