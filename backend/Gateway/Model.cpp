#include "Model.h"

#include <chrono>
#include <functional>
#include <map>
#include <tuple>
const std::chrono::seconds timeout(2);

using namespace HttpAssist;
using namespace ns;
const std::string secretService = "gateway";
using namespace jwt::params;

const Wt::Http::Message Model::getfromService(Services::Service service, const std::vector<Http::Message::Header>& headers, const std::string& params, const std::string& path)
{
    Client client;
    client.setTimeout(timeout);
    std::vector<Http::Message::Header> newHeaders = headers;
    getAuthService(newHeaders, service);
    client.get(skServicePaths[service] + "/" + path + "?" + params, newHeaders);
    client.waitDone();
    return client.message();
}

const Http::Message Model::postToService(Services::Service service, const std::vector<Http::Message::Header>& headers, const std::string& body, const std::string& path)
{
    Client client;
    client.setTimeout(timeout);
    Http::Message msg;
    msg.addBodyText(body);
    std::vector<Http::Message::Header> newHeaders = headers;
    getAuthService(newHeaders, service);
    writeHeaders(msg, newHeaders);
    client.post(skServicePaths[service] + "/" + path, msg);
    client.waitDone();
    return client.message();
}

void Model::getAuthService(std::vector<Http::Message::Header>& headers, Services::Service service)
{
    Client client;
    client.setTimeout(timeout);
    auto auth = [&]() {
        jwt::jwt_object obj{ algorithm("HS256"), payload({}), secret(secretService) };
        obj.add_claim("exp", std::chrono::system_clock::now() + std::chrono::minutes(30));
        const std::string token = obj.signature();

        Http::Message::Header serviceHeader("serviceheader", token);
        std::vector<Http::Message::Header> newHeaders;
        newHeaders.push_back(serviceHeader);
        client.get(skServicePaths[service] + "/" + "authService", newHeaders);
        client.waitDone();
        if (client.message().status() > 0) {
            json_t tokenJson = tryParsejson(client.message().body());
            Http::Message::Header outputHeader("serviceheader", tokenJson["token"]);
            std::cout << " EEE" << tokenJson["token"] << std::endl;
            headers.push_back(outputHeader);
        }
    };

    if (!serviceTokens[service].empty()) {
        try {
            auto dec_obj = jwt::decode(serviceTokens[service], algorithms({ "hs256" }), secret(secretServiceStrings[service]), verify(true));
            Http::Message::Header serviceHeader("serviceheader", dec_obj.signature()); //todo add delay 10 min
            headers.push_back(serviceHeader);
            serviceTokens[service] = dec_obj.signature();
        } catch (...) {
            auth();
        }
    } else {
        auth();
    }
}

const Http::Message Model::deletefromService(Services::Service service, const std::vector<Http::Message::Header>& headers, const std::string& body, const std::string& path)
{
    Client client;
    client.setTimeout(timeout);
    Http::Message msg;
    msg.addBodyText(body);
    std::vector<Http::Message::Header> newHeaders = headers;
    getAuthService(newHeaders, service);
    writeHeaders(msg, newHeaders);
    client.deleteRequest(skServicePaths[service] + "/" + path, msg);
    client.waitDone();
    return client.message();
}

const Http::Message Model::getTitles(const std::vector<Http::Message::Header>& headers, const std::string& params)
{

    Wt::Http::Message result;
    const Wt::Http::Message msg = getfromService(Services::News, headers, params, "titles");
    if (msg.status() == 200) {
        json_t titles = json_t::parse(msg.body()); //trust that json is valid
        std::string newsIds;
        std::map<int32_t, Title> titlesMap;
        for (auto titlesIt = titles.cbegin(); titlesIt != titles.cend(); ++titlesIt) {
            const int32_t ID = (*titlesIt)["ID"].get<int32_t>();
            newsIds += std::string("id=") + std::to_string(ID) + "&";
            Title title;
            title.title = (*titlesIt)["title"].get<std::string>();
            title.timestamp = (*titlesIt)["timestamp"].get<long long>();
            titlesMap[ID] = title;
        }
        if (newsIds.begin() != newsIds.end())
            newsIds.erase(newsIds.end() - 1); //remove last &

        const Wt::Http::Message& msgComments = getfromService(Services::Comments, headers, newsIds, "count");
        json_t titlesArray = json_t::array();
        if (msgComments.status() == 200) {
            json_t countComments = json_t::parse(msgComments.body()); //trust that json is valid
            for (auto countIt = countComments.cbegin(); countIt != countComments.cend(); ++countIt) {
                Title title;
                title.ID = (*countIt)["id"].get<int32_t>();
                title.title = titlesMap[title.ID].title;
                title.count = (*countIt)["count"].get<int32_t>();
                title.timestamp = titlesMap[title.ID].timestamp;
                if (!title.title.empty()) {
                    json_t titlejson = title;
                    titlesArray.push_back(titlejson);
                }
            }

        } else {
            LOG_ERROR("%s", "Comment service error. Degradation");
            for (auto titlesIt = titlesMap.cbegin(); titlesIt != titlesMap.cend(); ++titlesIt) {
                Title title;
                title.ID = titlesIt->first;
                title.title = titlesIt->second.title;
                title.timestamp = titlesIt->second.timestamp;
                if (!title.title.empty()) {
                    json_t titlejson = title;
                    titlesArray.push_back(titlejson);
                }
            }
        }
        result.setStatus(200);
        result.addBodyText(titlesArray.dump());
        writeHeaders(result, headers);
    } else {
        LOG_ERROR("%s", "News service error");
        result.setStatus(msg.status());
        result.addBodyText(msg.body());
    }
    return result;
}

const Http::Message Model::createComment(const std::vector<Http::Message::Header>& headers, const std::string& body)
{
    Wt::Http::Message result = postToService(Services::Comments, headers, body, "comment");
    return result;
}

const Http::Message Model::createNews(const std::vector<Http::Message::Header>& headers, const std::string& body)
{
    Wt::Http::Message result = postToService(Services::News, headers, body, "createNews");
    return result;
}

const Http::Message Model::login(const std::vector<Http::Message::Header>& headers, const std::string& body)
{
    Wt::Http::Message result = postToService(Services::Users, headers, body, "login");
    return result;
}

const Http::Message Model::reg(const std::vector<Http::Message::Header>& headers, const std::string& body)
{
    Wt::Http::Message result = postToService(Services::Users, headers, body, "register");
    return result;
}

const Http::Message Model::del(const std::vector<Http::Message::Header>& headers, const std::string& body)
{
    Wt::Http::Message result;
    WriteResponse(result, 200);
    Wt::Http::Message deleteUsersResult = deletefromService(Services::Users, headers, body, "delete");
    if (deleteUsersResult.status() != 200) {
        auto delayedTask = std::bind(&Model::deletefromService, this, Services::Users, headers, body, "delete");
        TaskQueue::Instance().addTask(delayedTask);
    }
    Wt::Http::Message deleteCommentsResult = deletefromService(Services::Comments, headers, body, "deleteByUser");
    if (deleteCommentsResult.status() != 200) {
        auto delayedTask = std::bind(&Model::deletefromService, this, Services::Comments, headers, body, "deleteByUser");
        TaskQueue::Instance().addTask(delayedTask);
    }

    return deleteCommentsResult;
}

const Http::Message Model::getAuthCode(const std::vector<Http::Message::Header>& headers, const std::string& params)
{
    const Wt::Http::Message result = getfromService(Services::Users, headers, params, "getAuthCode");
    return result;
}

const Http::Message Model::getToken(const std::vector<Http::Message::Header>& headers, const std::string& params)
{
    const Wt::Http::Message result = getfromService(Services::Users, headers, params, "getToken");
    return result;
}

const Http::Message Model::refreshToken(const std::vector<Http::Message::Header>& headers, const std::string& body)
{
    const Wt::Http::Message result = postToService(Services::Users, headers, body, "refreshToken");
    return result;
}

const Http::Message Model::like(const std::vector<Http::Message::Header>& headers, const std::string& body)
{
    Wt::Http::Message checkLike = postToService(Services::LikeHistory, headers, body, "writeLike");
    json_t likeStatus = tryParsejson(checkLike.body());
    auto entityIt = likeStatus.find("entityId");
    Wt::Http::Message result;
    if (checkLike.status() == 200 and entityIt != likeStatus.end() and entityIt.value().get<int32_t>() != -1) {
        Wt::Http::Message commentsResult = postToService(Services::Comments, headers, body, "like");
        if (commentsResult.status() != 200) {
            auto rollbackLike = deletefromService(Services::LikeHistory, headers, body, "deleteLike");
            WriteResponse(result, commentsResult.status());
        } else {
            result.setStatus(200);
        }
    }
    if (checkLike.status() != 200)
        WriteResponse(result, checkLike.status());
    return result;
}

const Http::Message Model::getOneNews(const std::vector<Http::Message::Header>& headers, const std::string& params)
{
    std::cout << "START REQUEST_" << std::endl;
    Wt::Http::Message result;
    json_t resultjson;
    std::cout << "BEFORE getNews" << std::endl;
    Wt::Http::Message getNews = getfromService(Services::News, headers, params, "getnews"); //title, body, creationDate
    std::cout << "AFTER getNews" << getNews.status() << std::endl;

    if (getNews.status() == 200) {
        writeHeaders(result, getNews.headers());
        json_t newsjson = tryParsejson(getNews.body());
        resultjson["news"] = newsjson;
        json_t resultComments = json_t::array();
        std::cout << "BEFORE getCommnets" << std::endl;
        Wt::Http::Message getComments = getfromService(Services::Comments, headers, params, "getComments");
        std::cout << "AFTER getCommnets" << getComments.status() << std::endl;

        if (getComments.status() == 200) {

            json_t commentsjson = tryParsejson(getComments.body());

            std::string userIds;
            for (auto commentsIt = commentsjson.cbegin(); commentsIt != commentsjson.cend(); ++commentsIt) {
                CommentInternal ci;
                from_json(*commentsIt, ci);
                userIds += std::string("id=") + std::to_string(ci.userId) + "&";
            }

            if (userIds.begin() != userIds.end())
                userIds.erase(userIds.end() - 1); //remove last &
            std::cout << "BEFORE getUsernames" << std::endl;
            Wt::Http::Message getUsernames = getfromService(Services::Users, headers, userIds, "names");
            std::cout << "AFTER getUsernames" << getUsernames.status() << std::endl;
            if (getUsernames.status() == 200 or userIds.empty()) {
                json_t names = tryParsejson(getUsernames.body());
                std::map<int32_t, std::string> userIdtoName;

                for (auto name : names) {
                    userIdtoName[name["userId"].get<int32_t>()] = name["name"].get<std::string>();
                }

                for (auto commentsIt = commentsjson.cbegin(); commentsIt != commentsjson.cend(); ++commentsIt) {
                    CommentExternal ce;
                    ce.body = (*commentsIt)["body"].get<std::string>();
                    ce.commentId = (*commentsIt)["commentId"].get<int32_t>();
                    ce.name = userIdtoName[(*commentsIt)["userId"].get<int32_t>()];
                    ce.rating = (*commentsIt)["rating"].get<int32_t>();
                    ce.userId = (*commentsIt)["userId"].get<int32_t>();
                    resultComments.push_back(ce);
                }
                resultjson["comments"] = resultComments;
                result.addBodyText(resultjson.dump());
                result.setStatus(200);
            } else {
                LOG_ERROR("User service error");
                result.setStatus(getUsernames.status());
                result.addBodyText(getUsernames.body());
            }
        } else {
            LOG_ERROR("Comments service error");
            result.setStatus(getComments.status());
            result.addBodyText(getComments.body());
        }
    } else {
        LOG_ERROR("News service error");
        result.setStatus(getNews.status());
        result.addBodyText(getNews.body());
    }

    return result;
}

const Http::Message Model::history(const std::vector<Http::Message::Header>& headers, const std::string& params)
{
    Wt::Http::Message result;
    Wt::Http::Message getHistory = getfromService(Services::LikeHistory, headers, params, "getLikes");
    std::string userIds;
    std::string commentIds;
    if (getHistory.status() == 200) {
        json_t resultjson;
        json_t resultsHistory = json_t::array();
        json_t historyjson = tryParsejson(getHistory.body());
        for (auto historyIt = historyjson.cbegin(); historyIt != historyjson.cend(); ++historyIt) {
            LikeEntity le;
            from_json(*historyIt, le);
            userIds += std::string("id=") + std::to_string(le.userId) + "&";
            commentIds += std::string("id=") + std::to_string(le.commentId) + "&";
        }
        if (userIds.begin() != userIds.end())
            userIds.erase(userIds.end() - 1); //remove last &
        if (commentIds.begin() != commentIds.end())
            commentIds.erase(commentIds.end() - 1); //remove last &

        Wt::Http::Message getUsernames = getfromService(Services::Users, headers, userIds, "names");
        Wt::Http::Message getComments = getfromService(Services::Comments, headers, commentIds, "commentsById");

        if ((getUsernames.status() == 200 and getComments.status() == 200) or userIds.empty()) {
            json_t names = tryParsejson(getUsernames.body());
            std::map<int32_t, std::string> userIdtoName;
            json_t comments = tryParsejson(getComments.body());
            std::map<int32_t, std::string> commentIdtoComment;

            for (auto name : names) {
                userIdtoName[name["userId"].get<int32_t>()] = name["name"].get<std::string>();
            }
            for (auto comment : comments) {
                commentIdtoComment[comment["commentId"].get<int32_t>()] = comment["body"].get<std::string>();
            }

            for (auto historyIt = historyjson.cbegin(); historyIt != historyjson.cend(); ++historyIt) {
                LikeEntityExternal le;
                le.timestamp = (*historyIt)["timestamp"].get<long long>();
                le.comment = commentIdtoComment[(*historyIt)["commentId"].get<int32_t>()];
                le.name = userIdtoName[(*historyIt)["userId"].get<int32_t>()];
                resultsHistory.push_back(le);
            }
            resultjson["history"] = resultsHistory;
            result.addBodyText(resultjson.dump());
            result.setStatus(200);
        } else {
            LOG_ERROR("User or comments service error");
            if (getUsernames.status() != 200) {
                result.setStatus(getUsernames.status());
                result.addBodyText(getUsernames.body());
            } else {
                result.setStatus(getComments.status());
                result.addBodyText(getComments.body());
            }
        }
    } else {
        LOG_ERROR("LikeHistory service error");
        result.setStatus(getHistory.status());
        result.addBodyText(getHistory.body());
    }
    return result;
}

bool Model::checkAuth(const std::vector<Http::Message::Header>& headers, uint32_t& userId, uint64_t accessMask)
{
    const std::string token = HttpAssist::getAuthToken(headers);
    const std::string params = "token=" + token;
    std::cout << "TOKEN " << token << std::endl;

    Wt::Http::Message result = getfromService(Services::Users, {}, params, "auth");
    bool authorized = result.status() == 200;
    if (authorized) {
        json_t userIdJson = tryParsejson(result.body());
        std::cout << userIdJson.dump();
        auto userIdIt = userIdJson.find("userId");
        userId = userIdIt.value().get<uint32_t>();
        auto accessRightsIt = userIdJson.find("accessRights");
        authorized = accessRightsIt.value().get<uint64_t>() & (1 << accessMask);
    }
    return authorized;
}

bool Model::sendMetric(const std::string& key, const std::string& value)
{
    Client client;
    client.setTimeout(timeout);
    if (metricServerToken.empty()) {
        Http::Message msg;
        msg.addHeader("Content-Type", " application/json");
        json_t auth = R"({ "jsonrpc": "2.0" ,
                       "method": "user.login" ,
                       "params":
                      { "user": "Admin" ,
                       "password": "zabbix" } ,
                      "id": 1 ,
                       "auth": null
                      })"_json; // )))))))
        msg.addBodyText(auth.dump());
        client.post(skMetricServer, msg);
        client.waitDone();
        const std::string response = client.message().body();
        const json_t authjson = tryParsejson(response);
        auto authIt = authjson.find("result");
        if (authIt != authjson.cend()) {
            metricServerToken = authIt.value().get<std::string>();
            std::cout << "token " << metricServerToken << std::endl;
        } else {
            LOG_ERROR("%s", "cant auth to metric server");
            std::cout << msg.body() << std::endl;
        }
    }
    return true;
}
#ifdef IS_TEST_BUILD

const Http::Message Model::clear(const std::vector<Http::Message::Header>& headers)
{
    Wt::Http::Message result;

    Wt::Http::Message result1 = deletefromService(Comments, headers, {}, "clear");
    Wt::Http::Message result2 = deletefromService(Users, headers, {}, "clear");
    Wt::Http::Message result3 = deletefromService(News, headers, {}, "clear");
    if (result1.status() == 200 and result2.status() == 200 and result3.status() == 200) {
        result.addBodyText("Databases is clear!");
        result.setStatus(200);
    } else {
        result.addBodyText("Smth is wrong!");
        result.setStatus(500);
    }
    return result;
}
#endif
