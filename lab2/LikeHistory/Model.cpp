#include "Model.h"
#include "../Shared/Database.h"
#include "../Shared/JsonStructs.h"
#include <functional>

using namespace ns;
using namespace Wt::Dbo;

Model::Model()
    : db(nullptr)
{
}

const json Model::getLikes(int32_t page)
{
    if (!db)
        db = Db::GetInst()->GetMysql();

    json result = json::array();

    auto req = db->prepareStatement("SELECT commentId,userId,UNIX_TIMESTAMP(creationDate) from  LikeHistory order by creationDate desc limit ? offset ?");
    req->bind(0, likesPerPage);
    req->bind(1, (page - 1) * likesPerPage);

    if (!req) {
        LOG_ERROR("Cant prepare statement");
    } else {
        req->execute();
        LOG_INFO("Db request %s", req->sql().c_str());
        while (req->nextRow()) {
            LikeEntity likeEntity;
            req->getResult(0, &likeEntity.commentId);
            req->getResult(1, &likeEntity.userId);
            req->getResult(2, &likeEntity.timestamp);
            result.push_back(likeEntity);
        };
    }
    return result;
}

const json Model::writeLike(int32_t userId, int32_t commentId)
{
    if (!db)
        db = Db::GetInst()->GetMysql();

    json result;

    auto req = db->prepareStatement("INSERT INTO  LikeHistory(userId, commentId) VALUES(?,?)");
    req->bind(0, userId);
    req->bind(1, commentId);
    if (!req) {
        LOG_ERROR("Cant prepare statement");
    } else {
        try {
            LOG_INFO("Db request %s", req->sql().c_str());
            req->execute();
            result["entityId"] = req->insertedId();
        } catch (...) {
            result["entityId"] = -1;
            LOG_INFO("Cant insert like!");
        }
    };

    return result;
}
