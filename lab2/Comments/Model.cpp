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
    auto req = db->prepareStatement("INSERT INTO Comments(newsId, userId, body) VALUES(?, ?, ?);");
    req->bind(0, newsId);
    req->bind(1, userId);
    req->bind(2, text);

    if (!req) {
        LOG_ERROR("Cant prepare statement");
    } else {
        try {
            LOG_INFO("Db request %s", req->sql().c_str());
            req->execute();
            if (req->affectedRowCount() == 0)
                return json();
            result["commentId"] = req->insertedId();
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

const json Model::getComments(int32_t newsId, int32_t page)
{
    if (!db)
        db = Db::GetInst()->GetMysql();

    json result = json::array();

    auto req = db->prepareStatement("SELECT ID,userId,body from Comments where newsId = ? limit ? offset ?");
    req->bind(0, newsId);
    req->bind(1, commentsPerPage);
    req->bind(2, commentsPerPage * (page - 1));

    if (!req) {
        LOG_ERROR("Cant prepare statement");
    } else {
        try {
            req->execute();
            LOG_INFO("Db request %s", req->sql().c_str());
            while (req->nextRow()) {
                CommentInternal ci;
                req->getResult(0, &ci.commentId);
                req->getResult(1, &ci.userId);
                req->getResult(2, &ci.body, 255);
                result.push_back(ci);
            };
        } catch (...) {
            LOG_ERROR("Cant like comment!");
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
    auto req = db->prepareStatement("DELETE  from Comments;");

    if (!req) {
        LOG_ERROR("Cant prepare statement");
    } else {
        LOG_INFO("Db request %s", req->sql().c_str());
        try {
            req->execute();
            result["result"] = "comments is dropped!";
        } catch (...) {
            LOG_INFO("cant drop comment!");
        }
    }
    return result;
}
#endif
