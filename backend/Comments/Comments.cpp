#include "Comments.h"
#include "../Shared/HttpAssist.h"
#include "../Shared/JsonStructs.h"
#include "Model.h"
#include <cstdlib>

using namespace ns;
using namespace HttpAssist;

CreateComment::CreateComment(Model* _model)
    : Base(_model)
{
}

void CreateComment::handleRequest(const Wt::Http::Request& request, Wt::Http::Response& response)
{
    if (!HttpAssist::checkAuth(request, model->getSecretKey())) {
        WriteResponse(response, 400);
        return;
    }
    json_t commentjson = tryParsejson(getRequestBody(request));
    Comment comment;
    std::cout << commentjson.dump() << std::endl;
    if (request.method() != "POST" or !from_json(commentjson, comment)) {
        WriteResponse(response, 403);
        ;
        return;
    }

    json_t newsResponse = model->createComment(comment.userId, comment.newsId, comment.text);
    if (newsResponse.empty()) {
        WriteResponse(response, 500);
        return;
    }
    response.out() << newsResponse.dump();
}

LikeComment::LikeComment(Model* _model)
    : Base(_model)
{
}

void LikeComment::handleRequest(const Http::Request& request, Http::Response& response)
{
    if (!HttpAssist::checkAuth(request, model->getSecretKey())) {
        WriteResponse(response, 400);
        return;
    }
    json_t body = tryParsejson(getRequestBody(request));
    int32_t id = body["commentId"].get<int32_t>();
    if (request.method() != "POST" or !body.count("commentId")) {
        WriteResponse(response, 403);
        ;
        return;
    }
    json_t likeResult = model->likeComment(id);
    if (likeResult.empty()) {
        WriteResponse(response, 500);
        return;
    }
    response.out() << likeResult.dump();
}

CountComment::CountComment(Model* _model)
    : Base(_model)
{
}

void CountComment::handleRequest(const Http::Request& request, Http::Response& response)
{
    if (!HttpAssist::checkAuth(request, model->getSecretKey())) {
        WriteResponse(response, 400);
        return;
    }
    auto ids = request.getParameterValues("id");
    if (request.method() != "GET") {
        WriteResponse(response, 403);
        ;
        return;
    }
    std::vector<int32_t> idsInt;
    for (auto id : ids) {
        idsInt.push_back(atoi(id.c_str()));
    }
    json_t comments = model->countComments(idsInt);
    if (comments.empty()) {
        response.setStatus(200);
        return;
    }
    response.out() << comments.dump();
}

DeleteComments::DeleteComments(Model* _model)
    : Base(_model)
{
}

void DeleteComments::handleRequest(const Http::Request& request, Http::Response& response)
{
    json_t body = tryParsejson(getRequestBody(request));
    int32_t id = body["userId"].get<int32_t>();
    if (request.method() != "DELETE" or !body.count("userId")) {
        WriteResponse(response, 403);
        ;
        return;
    }
    json_t result = model->deleteComments(id);
    if (result.empty()) {
        WriteResponse(response, 500);
        return;
    }
    response.out() << result.dump();
}

GetComments::GetComments(Model* _model)
    : Base(_model)
{
}

void GetComments::handleRequest(const Http::Request& request, Http::Response& response)
{
    if (!HttpAssist::checkAuth(request, model->getSecretKey())) {
        WriteResponse(response, 400);
        return;
    }
    int32_t newsId = extractPositiveParam(request, "newsId");
    int32_t page = extractPositiveParam(request, "page");
    if (request.method() != "GET" or newsId < 0 or page < 0) {
        WriteResponse(response, 403);
        ;
        return;
    }
    json_t news = model->getComments(newsId, page);
    if (news.empty()) {
        response.setStatus(200);
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

GetCommentsById::GetCommentsById(Model* _model)
    : Base(_model)
{
}

void GetCommentsById::handleRequest(const Http::Request& request, Http::Response& response)
{
    if (!HttpAssist::checkAuth(request, model->getSecretKey())) {
        WriteResponse(response, 400);
        return;
    }
    auto ids = request.getParameterValues("id");
    if (request.method() != "GET") {
        WriteResponse(response, 403);
        return;
    }
    std::vector<int32_t> idsInt;
    for (auto id : ids) {
        idsInt.push_back(atoi(id.c_str()));
    }
    json_t comments = model->getCommentsById(idsInt);
    if (comments.empty()) {
        WriteResponse(response, 500);
        return;
    }
    response.out() << comments.dump();
}
