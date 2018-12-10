#include "News.h"
#include "../Shared/HttpAssist.h"
#include "../Shared/JsonStructs.h"
#include "Model.h"
#include <iostream>
using namespace ns;
using namespace HttpAssist;

GetTitles::GetTitles(Model* _model)
    : Base(_model)
{
}

void GetTitles::handleRequest(const Wt::Http::Request& request, Wt::Http::Response& response)
{
    if (!HttpAssist::checkAuth(request, model->getSecretKey())) {
        WriteResponse(response, 400);
        return;
    }
    int32_t page = extractPositiveParam(request, "page");
    if (request.method() != "GET" or page < 0) {
        WriteResponse(response, 403);
        return;
    }
    json_t news = model->getTitles(page);
    if (news.empty()) {
        WriteResponse(response, 500);
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
    if (!HttpAssist::checkAuth(request, model->getSecretKey())) {
        WriteResponse(response, 400);
        return;
    }
    json_t newsjson = tryParsejson(getRequestBody(request));
    News news;
    if (request.method() != "POST" or !from_json(newsjson, news)) {
        WriteResponse(response, 403);
        return;
    }
    if (news.title.size() < 1 or news.body.size() < 1) {
        WriteResponse(response, 403);
        return;
    }

    json_t newsResponse = model->createNews(news.title, news.body);
    if (newsResponse.empty()) {
        WriteResponse(response, 500);
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
    if (!HttpAssist::checkAuth(request, model->getSecretKey())) {
        WriteResponse(response, 400);
        return;
    }
    std::cout << "BEFORE GET NEWS" << std::endl;
    int32_t newsId = extractPositiveParam(request, "newsId");
    std::cout << "AFTER GET NEWS " << newsId << std::endl;
    if (request.method() != "GET" or newsId < 0) {
        WriteResponse(response, 403);
        return;
    }
    std::cout << "BEFORE GET MODEL" << std::endl;
    json_t news = model->getNews(newsId);
    std::cout << "AFTER GET MODEL" << std::endl;

    if (news.empty()) {
        WriteResponse(response, 500);
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
