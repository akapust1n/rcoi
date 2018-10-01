#include "News.h"
#include "../Shared/HttpAssist.h"
#include "../Shared/JsonStructs.h"
#include "Model.h"

using namespace ns;
using namespace HttpAssist;

GetTitles::GetTitles(Model* _model)
    : Base(_model)
{
}

void GetTitles::handleRequest(const Wt::Http::Request& request, Wt::Http::Response& response)
{
    std::string result;
    int32_t countTitles = extractPositiveParam(request, "count");
    std::cout << countTitles << std::endl;
    if (request.method() != "GET" or countTitles < 0 or countTitles > 50) {
        response.setStatus(403);
        return;
    }
    json news = model->getTitles(countTitles);
    if (news.empty()) {
        response.out() << "Cant find news";
        return;
    }
    response.out() << news.dump();
}

CreateNews::CreateNews(Model* _model)
    : Base(_model)
{
}

void CreateNews::handleRequest(const Http::Request& request, Http::Response& response)
{
    json newsJson = json::parse(getRequestBody(request));
    News news;
    if (request.method() != "POST" or !from_json(newsJson, news)) {
        response.setStatus(403);
        return;
    }

    json newsResponse = model->createNews(news.title, news.body);
    if (newsResponse.empty()) {
        response.setStatus(500);
        return;
    }
    response.out() << newsResponse.dump();
}
