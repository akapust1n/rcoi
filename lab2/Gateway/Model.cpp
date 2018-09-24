#include "Model.h"
#include "../Shared/HttpAssist.h"
using namespace HttpAssist;

const Wt::Http::Message& Model::getfromNews(const std::vector<Http::Message::Header>& headers, const std::string& params, const std::string& path)
{
    Client client;
    client.get(urlNews + "/" + path + "?" + params, headers);
    client.waitDone();
    return client.message();
}
