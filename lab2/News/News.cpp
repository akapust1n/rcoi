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
    int32_t page = extractPositiveParam(request, "page");
    std::cout << page << std::endl;
    if (request.method() != "GET" or page < 0) {
        response.setStatus(403);
        return;
    }
    json news = model->getTitles(page);
    if (news.empty()) {
        response.setStatus(500);
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
    json newsJson = tryParsejson(getRequestBody(request));
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

GetNews::GetNews(Model* _model)
    : Base(_model)
{
}

void GetNews::handleRequest(const Http::Request& request, Http::Response& response)
{
    int32_t newsId = extractPositiveParam(request, "newsId");
    if (request.method() != "GET" or newsId < 0) {
        response.setStatus(403);
        return;
    }
    json news = model->getNews(newsId);
    if (news.empty()) {
        response.out() << "Cant find news";
        response.setStatus(500);
        return;
    }
    response.out() << news.dump();
}

#ifdef IS_TEST_BUILD

Clear::Clear(Model* _model)
    : Base(_model)
{
}

void Clear::handleRequest(const Http::Request& request, Http::Response& response)
{
    json result = model->clear();
    response.out() << result.dump();
}
#endif
