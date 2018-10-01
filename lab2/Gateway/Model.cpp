#include "Model.h"
#include "../Shared/HttpAssist.h"
#include "../Shared/JsonStructs.h"
#include <map>

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
        json titles = json::parse(msg.body()); //trust that json is valid
        std::string newsIds;
        std::map<int32_t, std::string> titlesMap;
        for (auto newsIt = titles.cbegin(); newsIt != titles.cend(); ++newsIt) {
            const int32_t ID = (*newsIt)["ID"].get<int32_t>();
            newsIds += std::string("id=") + std::to_string(ID) + "&";
            titlesMap[ID] = (*newsIt)["title"].get<std::string>();
        }
        if (newsIds.begin() != newsIds.end())
            newsIds.erase(newsIds.end() - 1); //remove last &

        const Wt::Http::Message& msgComments = getfromService(Comments, headers, newsIds, "count");
        if (msgComments.status() == 200) {
            json countComments = json::parse(msgComments.body()); //trust that json is valid
            json titlesArray = json::array();
            for (auto countIt = countComments.cbegin(); countIt != countComments.cend(); ++countIt) {
                Title title;
                title.ID = (*countIt)["id"].get<int32_t>();
                title.title = titlesMap[title.ID];
                title.count = (*countIt)["count"].get<int32_t>();
                if (!title.title.empty()) {
                    json titleJson = title;
                    titlesArray.push_back(titleJson);
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
    Wt::Http::Message result = postToService(Comments, headers, body, "like");
    Wt::Http::Message result2 = postToService(Users, headers, body, "incRating");
    if (result.status() != 200 or result2.status() != 200)
        result.setStatus(500);
    return result;
}
