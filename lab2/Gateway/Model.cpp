#include "Model.h"
#include "../Shared/HttpAssist.h"
#include "../Shared/JsonStructs.h"
#include <map>
#include <tuple>

using namespace HttpAssist;
using namespace ns;

const Wt::Http::Message Model::getfromService(Service service, const std::vector<Http::Message::Header>& headers, const std::string& params, const std::string& path)
{
    Client client;
    client.get(skServicePaths[service] + "/" + path + "?" + params, headers);
    client.waitDone();
    return client.message();
}

const Http::Message Model::postToService(Model::Service service, const std::vector<Http::Message::Header>& headers, const std::string& body, const std::string& path)
{
    Client client;
    Http::Message msg;
    msg.addBodyText(body);
    writeHeaders(msg, headers);
    client.post(skServicePaths[service] + "/" + path, msg);
    client.waitDone();
    return client.message();
}

const Http::Message Model::deletefromService(Model::Service service, const std::vector<Http::Message::Header>& headers, const std::string& body, const std::string& path)
{
    Client client;
    Http::Message msg;
    msg.addBodyText(body);
    writeHeaders(msg, headers);
    client.deleteRequest(skServicePaths[service] + "/" + path, msg);
    client.waitDone();
    return client.message();
}

const Http::Message Model::getTitles(const std::vector<Http::Message::Header>& headers, const std::string& params)
{

    Wt::Http::Message result;
    const Wt::Http::Message msg = getfromService(News, headers, params, "titles");
    if (msg.status() == 200) {
        json_t titles = json_t::parse(msg.body()); //trust that json is valid
        std::string newsIds;
        std::map<int32_t, Title> titlesMap;
        for (auto newsIt = titles.cbegin(); newsIt != titles.cend(); ++newsIt) {
            const int32_t ID = (*newsIt)["ID"].get<int32_t>();
            newsIds += std::string("id=") + std::to_string(ID) + "&";
            Title title;
            title.title = (*newsIt)["title"].get<std::string>();
            title.timestamp = (*newsIt)["timestamp"].get<long long>();
            titlesMap[ID] = title;
        }
        if (newsIds.begin() != newsIds.end())
            newsIds.erase(newsIds.end() - 1); //remove last &

        const Wt::Http::Message& msgComments = getfromService(Comments, headers, newsIds, "count");
        if (msgComments.status() == 200) {
            json_t countComments = json_t::parse(msgComments.body()); //trust that json is valid
            json_t titlesArray = json_t::array();
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
            result.setStatus(200);
            result.addBodyText(titlesArray.dump());
            writeHeaders(result, headers);
        } else {
            LOG_ERROR("%s", "Comment service error");
            result.setStatus(msgComments.status());
        }
    } else {
        LOG_ERROR("%s", "News service error");
        result.setStatus(msg.status());
    }
    return result;
}

const Http::Message Model::createComment(const std::vector<Http::Message::Header>& headers, const std::string& body)
{
    Wt::Http::Message result = postToService(Comments, headers, body, "comment");
    return result;
}

const Http::Message Model::createNews(const std::vector<Http::Message::Header>& headers, const std::string& body)
{
    Wt::Http::Message result = postToService(News, headers, body, "createNews");
    return result;
}

const Http::Message Model::login(const std::vector<Http::Message::Header>& headers, const std::string& body)
{
    Wt::Http::Message result = postToService(Users, headers, body, "login");
    return result;
}

const Http::Message Model::reg(const std::vector<Http::Message::Header>& headers, const std::string& body)
{
    Wt::Http::Message result = postToService(Users, headers, body, "register");
    return result;
}

const Http::Message Model::del(const std::vector<Http::Message::Header>& headers, const std::string& body)
{
    Wt::Http::Message result = deletefromService(Users, headers, body, "delete");
    Wt::Http::Message result2 = deletefromService(Comments, headers, body, "deleteByUser");
    if (result.status() != 200 or result2.status() != 200)
        result.setStatus(500);
    return result;
}

const Http::Message Model::like(const std::vector<Http::Message::Header>& headers, const std::string& body)
{
    Wt::Http::Message checkLike = postToService(LikeHistory, headers, body, "writeLike");
    json_t likeStatus = tryParsejson(checkLike.body());
    auto entityIt = likeStatus.find("entityId");

    if (checkLike.status() == 200 and entityIt != likeStatus.end() and entityIt.value().get<int32_t>() != -1) {

        Wt::Http::Message result = postToService(Comments, headers, body, "like");
        Wt::Http::Message result2 = postToService(Users, headers, body, "incRating");
        if (result.status() != 200 or result2.status() != 200)
            result.setStatus(500);
        return result;
    } else {
        checkLike.setStatus(500);
        return checkLike;
    }
}

const Http::Message Model::getOneNews(const std::vector<Http::Message::Header>& headers, const std::string& params)
{
    std::cout << "START REQUEST_" << std::endl;
    Wt::Http::Message result;
    json_t resultjson;
    std::cout << "BEFORE getNews" << std::endl;
    Wt::Http::Message getNews = getfromService(News, headers, params, "getnews"); //title, body, creationDate
    std::cout << "AFTER getNews" << getNews.status() << std::endl;

    if (getNews.status() == 200) {
        writeHeaders(result, getNews.headers());
        json_t newsjson = tryParsejson(getNews.body());
        resultjson["news"] = newsjson;
        json_t resultComments = json_t::array();
        std::cout << "BEFORE getCommnets" << std::endl;
        Wt::Http::Message getComments = getfromService(Comments, headers, params, "getComments");
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
            Wt::Http::Message getUsernames = getfromService(Users, headers, userIds, "names");
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
            }
        } else {
            LOG_ERROR("Comments service error");
            result.setStatus(getComments.status());
        }
    } else {
        LOG_ERROR("News service error");
        result.setStatus(getNews.status());
    }

    return result;
}

const Http::Message Model::history(const std::vector<Http::Message::Header>& headers, const std::string& params)
{
    Wt::Http::Message result;
    Wt::Http::Message getHistory = getfromService(LikeHistory, headers, params, "getLikes");
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

        Wt::Http::Message getUsernames = getfromService(Users, headers, userIds, "names");
        Wt::Http::Message getComments = getfromService(Comments, headers, commentIds, "commentsById");

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
            result.setStatus(getUsernames.status());
        }
    } else {
        LOG_ERROR("LikeHistory service error");
        result.setStatus(getHistory.status());
    }
    return result;
}

bool Model::checkAuth(const std::vector<Http::Message::Header>& headers, uint32_t& userId)
{
    const std::string token = HttpAssist::getAuthToken(headers);
    const std::string params = "token=" + token;

    Wt::Http::Message result = getfromService(Users, {}, params, "auth");
    const bool authorized = result.status() == 200;
    if (authorized) {
        json_t userIdJson = tryParsejson(result.body());
        auto userIdIt = userIdJson.find("userId");
        userId = userIdIt.value().get<uint32_t>();
    }
    return result.status() == 200;
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
