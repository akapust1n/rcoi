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

const json Model::reg(const std::string& name, const std::string& pwd)
{
    if (!db)
        db = Db::GetInst()->GetMysql();

    json result;
    auto req = db->prepareStatement("INSERT INTO Users(name,password) VALUES(?,?)");
    const std::string hash = std::to_string(std::hash<std::string>{}(pwd));
    if (!req) {
        LOG_ERROR("Cant prepare statement");
    } else {
        req->bind(0, name);
        req->bind(1, hash);
        LOG_INFO("Db request %s", req->sql().c_str());

        try {
            req->execute();

            if (req->affectedRowCount() != 0) {
                result["userId"] = req->insertedId();
            }
        } catch (...) {
            LOG_ERROR("Cant create user!");
        }
    }
    return result;
}

const json Model::login(const std::string& name, const std::string& pwd)
{
    if (!db)
        db = Db::GetInst()->GetMysql();
    json result;
    auto req = db->prepareStatement("SELECT ID FROM  Users WHERE name=? and password=?");
    const std::string hash = std::to_string(std::hash<std::string>{}(pwd));
    if (!req) {
        LOG_ERROR("Cant prepare statement");
    } else {
        req->bind(0, name);
        req->bind(1, hash);
        LOG_INFO("Db request %s", req->sql().c_str());
        try {
            req->execute();
            if (req->nextRow()) {
                int32_t ID;
                req->getResult(0, &ID);
                result["userId"] = ID;
            }
        } catch (...) {
            LOG_INFO("wrong login!");
        }
    }
    return result;
}

const json Model::del(int32_t userId)
{
    if (!db)
        db = Db::GetInst()->GetMysql();
    json result;
    auto req = db->prepareStatement("DELETE FROM Users WHERE ID=?");

    if (!req) {
        LOG_ERROR("Cant prepare statement");
    } else {
        req->bind(0, userId);
        LOG_INFO("Db request %s", req->sql().c_str());
        try {
            req->execute();
            if (req->affectedRowCount() > 0) {
                result["result"] = "user is deleted!";
            }
        } catch (...) {
            LOG_INFO("wrong userId!");
        }
    }
    return result;
}

const json Model::incRating(int32_t userId)
{
    if (!db)
        db = Db::GetInst()->GetMysql();
    json result;
    auto req = db->prepareStatement("UPDATE Users SET rating = rating+1 WHERE ID=?");

    if (!req) {
        LOG_ERROR("Cant prepare statement");
    } else {
        req->bind(0, userId);
        LOG_INFO("Db request %s", req->sql().c_str());
        try {
            req->execute();
            if (req->affectedRowCount() > 0) {
                result["result"] = "user rating is inremented!";
            }
        } catch (...) {
            LOG_INFO("wrong userId!");
        }
    }
    return result;
}
#ifdef IS_TEST_BUILD
const json Model::clear()
{
    if (!db)
        db = Db::GetInst()->GetMysql();
    json result;
    auto req = db->prepareStatement("DELETE  from Users;");

    if (!req) {
        LOG_ERROR("Cant prepare statement");
    } else {
        LOG_INFO("Db request %s", req->sql().c_str());
        try {
            req->execute();
            result["result"] = "users is dropped!";
        } catch (...) {
            LOG_INFO("cant drop Users!");
        }
    }
    return result;
}
#endif
