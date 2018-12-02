#include "Model.h"
#include "../Shared/Database.h"
#include "../Shared/JsonStructs.h"
#include <chrono>
#include <functional>

using namespace ns;
using namespace Wt::Dbo;
const int32_t logoutTime = 1800; //minutes
const int32_t timeLiveUserToken = 86400; //1 data

Model::Model()
    : db(nullptr)
{
}

const json_t Model::reg(const std::string& name, const std::string& pwd)
{
    if (!db)
        db = Db::GetInst()->GetMysql();

    json_t result;
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

const json_t Model::login(const std::string& name, const std::string& pwd)
{
    if (!db)
        db = Db::GetInst()->GetMysql();
    json_t result;
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
                //result["userId"] = ID;
                if (ID > 0) {
                    jwt::jwt_object obj{ algorithm("HS256"), payload({ { "userId", std::to_string(ID) } }), secret(key) };
                    obj.add_claim("rand", std::chrono::system_clock::now());

                    const std::string token = obj.signature();
                    req = db->prepareStatement("DELETE FROM Tokens WHERE userId=? AND kind=0");
                    req->bind(0, ID);
                    if (!req) {
                        LOG_ERROR("Cant prepare statement");
                    } else {
                        req->execute();
                        req = db->prepareStatement("INSERT INTO Tokens(token, userId, expireTimestamp, updateTimestamp) "
                                                   "VALUES(?, ?, NOW() + INTERVAL 1 DAY, NOW())");
                        if (!req) {
                            LOG_ERROR("Cant prepare statement");
                        } else {
                            req->bind(0, token);
                            req->bind(1, ID);
                            LOG_INFO("Db request %s", req->sql().c_str());
                            try {
                                req->execute();
                                if (req->insertedId() > 0) {
                                    result["authtoken"] = token;
                                }
                            } catch (...) {
                                LOG_INFO("can create token!");
                            }
                        }
                    }
                }
            }

        } catch (...) {
            LOG_INFO("wrong login!");
        }
    }
    db->commitTransaction();
    return result;
}

const json_t Model::del(int32_t userId)
{
    if (!db)
        db = Db::GetInst()->GetMysql();
    json_t result;
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

const json_t Model::incRating(int32_t userId)
{
    if (!db)
        db = Db::GetInst()->GetMysql();
    json_t result;
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

const json_t Model::getNames(const std::vector<int32_t>& ids)
{
    if (!db)
        db = Db::GetInst()->GetMysql();
    json_t result;
    for (auto id : ids) {
        auto req = db->prepareStatement("SELECT name FROM Users WHERE ID=?;");
        req->bind(0, id);

        if (!req) {
            LOG_ERROR("Cant prepare statement");
        } else {
            try {
                req->execute();
                LOG_INFO("Db request %s", req->sql().c_str());
                if (req->nextRow()) {
                    std::string name;
                    req->getResult(0, &name, 255);
                    json_t temp;
                    temp["userId"] = id;
                    temp["name"] = name;
                    result.push_back(temp);
                }

            } catch (...) {
                LOG_ERROR("Cant count comments");
            }
        };
    }

    return result;
}

int32_t Model::getAuthCode(int32_t clientId)
{
    if (!db)
        db = Db::GetInst()->GetMysql();
    int32_t authCode = rand() % 999;
    int32_t result = -1;

    auto req = db->prepareStatement("INSERT INTO AuthCodes(clientId, authCode, expireTimestamp)"
                                    " VALUES(?,?,DATE_ADD(NOW(), INTERVAL 10 MINUTE))");
    req->bind(0, clientId);
    req->bind(1, authCode);
    if (!req) {
        LOG_ERROR("Cant prepare statement");
    } else {
        try {
            req->execute();
            LOG_INFO("Db request %s", req->sql().c_str());
            if (req->insertedId() > 0) {
                result = authCode;
            }

        } catch (...) {
            LOG_ERROR("Cant find token");
        }
    };

    return result;
}

json_t Model::getToken(int32_t clientId, int32_t authCode)
{
    if (!db)
        db = Db::GetInst()->GetMysql();
    json_t result;

    auto req = db->prepareStatement("SELECT ID from AuthCodes where clientId=? "
                                    "and authCode=? and "
                                    " (UNIX_TIMESTAMP(expireTimestamp) - UNIX_TIMESTAMP(NOW()))>0");
    req->bind(0, clientId);
    req->bind(1, authCode);
    if (!req) {
        LOG_ERROR("Cant prepare statement");
    } else {
        try {
            req->execute();
            LOG_INFO("Db request %s", req->sql().c_str());
            if (req->nextRow()) {
                jwt::jwt_object obj{ algorithm("HS256"), payload({ { "userId", std::to_string(1) } }), secret(key) };
                obj.add_claim("rand", std::chrono::system_clock::now());
                const std::string token = obj.signature();
                jwt::jwt_object objRefresh{ algorithm("HS256"), payload({ { "userId", std::to_string(1) } }), secret(key) };
                objRefresh.add_claim("rand", std::chrono::system_clock::now() + std::chrono::seconds{ 10 });
                const std::string tokenRefresh = objRefresh.signature();

                req = db->prepareStatement("INSERT INTO Tokens(token, userId, expireTimestamp, updateTimestamp, kind) "
                                           "VALUES(?, ?, NOW() + INTERVAL 1 DAY, NOW(),0),"
                                           "(?, ?, NOW() + INTERVAL 1 DAY, NOW(), 1)");
                if (!req) {
                    LOG_ERROR("Cant prepare statement");
                } else {
                    req->bind(0, token);
                    req->bind(1, 1); //hardcoded userId
                    req->bind(2, tokenRefresh); //hardcoded userId
                    req->bind(3, 1); //hardcoded userId

                    LOG_INFO("Db request %s", req->sql().c_str());
                    try {
                        req->execute();
                        std::cout << "INS ID" << req->insertedId() << std::endl;
                        if (req->affectedRowCount() > 0) {
                            std::cout << "TOKENS CREATED" << std::endl;
                            result["authtoken"] = token;
                            result["refreshtoken"] = tokenRefresh;
                        }
                    } catch (...) {
                        LOG_INFO("can create token!");
                    }
                }
            }

        } catch (...) {
            LOG_ERROR("Cant insert tokens");
        }
    }
    return result;
}
uint32_t Model::checkAuth(uint32_t userId, const std::string& token)
{
    if (!db)
        db = Db::GetInst()->GetMysql();
    uint32_t result = 401;
    auto req = db->prepareStatement("SELECT ID "
                                    "FROM Tokens WHERE userId=? and token=? AND "
                                    "  (UNIX_TIMESTAMP(expireTimestamp) - UNIX_TIMESTAMP(NOW())) > 0 AND"
                                    " UNIX_TIMESTAMP(NOW()) - (UNIX_TIMESTAMP(updateTimestamp))< ?");
    req->bind(0, static_cast<int32_t>(userId));
    req->bind(1, token);
    req->bind(2, logoutTime);

    if (!req) {
        LOG_ERROR("Cant prepare statement");
    } else {
        try {
            req->execute();
            LOG_INFO("Db request %s", req->sql().c_str());
            if (req->nextRow()) {
                result = 200;
                req = db->prepareStatement("UPDATE Tokens SET updateTimestamp=now()  WHERE ID=? and token=?;");
                req->bind(0, static_cast<int32_t>(userId));
                req->bind(1, token);
                req->execute();
            }

        } catch (...) {
            LOG_ERROR("Cant find token");
        }
    };

    return result;
}

json_t Model::refreshToken(const std::string& token)
{
    if (!db)
        db = Db::GetInst()->GetMysql();
    json_t result;
    auto req = db->prepareStatement("SELECT ID FROM Tokens where token=? and kind=1");
    req->bind(0, token);

    if (!req) {
        LOG_ERROR("Cant prepare statement");
    } else {
        try {
            req->execute();
            LOG_INFO("Db request %s", req->sql().c_str());
            if (req->nextRow()) {
                jwt::jwt_object obj{ algorithm("HS256"), payload({ { "userId", std::to_string(1) } }), secret(key) };
                obj.add_claim("rand", std::chrono::system_clock::now());
                const std::string token = obj.signature();
                jwt::jwt_object objRefresh{ algorithm("HS256"), payload({ { "userId", std::to_string(1) } }), secret(key) };
                objRefresh.add_claim("rand", std::chrono::system_clock::now() + std::chrono::seconds{ 10 });
                const std::string tokenRefresh = objRefresh.signature();

                req = db->prepareStatement("INSERT INTO Tokens(token, userId, expireTimestamp, updateTimestamp, kind) "
                                           "VALUES(?, ?, NOW() + INTERVAL 1 DAY, NOW(),0),"
                                           "(?, ?, NOW() + INTERVAL 1 DAY, NOW(), 1)");
                if (!req) {
                    LOG_ERROR("Cant prepare statement");
                } else {
                    req->bind(0, token);
                    req->bind(1, 1); //hardcoded userId
                    req->bind(2, tokenRefresh); //hardcoded userId
                    req->bind(3, 1); //hardcoded userId
                    LOG_INFO("Db request %s", req->sql().c_str());
                    try {
                        req->execute();
                        std::cout << "INS ID" << req->insertedId() << std::endl;
                        if (req->affectedRowCount() > 0) {
                            std::cout << "TOKENS CREATED" << std::endl;
                            result["authtoken"] = token;
                            result["refreshtoken"] = tokenRefresh;
                        }
                    } catch (...) {
                        LOG_INFO("cant create token!");
                    }
                }
            }
        } catch (...) {
            LOG_ERROR("Cant find token");
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
