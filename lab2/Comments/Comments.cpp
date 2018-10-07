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
    json commentJson = json::parse(getRequestBody(request));
    Comment comment;
    if (request.method() != "POST" or !from_json(commentJson, comment)) {
        response.setStatus(403);
        return;
    }

    json newsResponse = model->createComment(comment.userId, comment.newsId, comment.text);
    if (newsResponse.empty()) {
        response.setStatus(500);
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
    json body = json::parse(getRequestBody(request));
    int32_t id = body["commentId"].get<int32_t>();
    if (request.method() != "POST" or !body.count("commentId")) {
        response.setStatus(403);
        return;
    }
    json likeResult = model->likeComment(id);
    if (likeResult.empty()) {
        response.out() << "Cant like comment";
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
    std::string result;
    auto ids = request.getParameterValues("id");
    if (request.method() != "GET") {
        response.setStatus(403);
        return;
    }
    std::vector<int32_t> idsInt;
    for (auto id : ids) {
        idsInt.push_back(atoi(id.c_str()));
    }
    json comments = model->countComments(idsInt);
    if (comments.empty()) {
        response.out() << "Cant count comments";
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
    json body = json::parse(getRequestBody(request));
    int32_t id = body["userId"].get<int32_t>();
    if (request.method() != "DELETE" or !body.count("userId")) {
        response.setStatus(403);
        return;
    }
    json result = model->deleteComments(id);
    if (result.empty()) {
        response.out() << "Cant delete comments";
        return;
    }
    response.out() << result.dump();
}

void GetComments::handleRequest(const Http::Request &request, Http::Response &response)
{
    int32_t newsId = extractPositiveParam(request, "newsId");
    int32_t page = extractPositiveParam(request, "page");
    if (request.method() != "GET" or newsId < 0 or page < 0) {
        response.setStatus(403);
        return;
    }
    json news = model->getComments(newsId, page);
    if (news.empty()) {
        response.out() << "Cant find news";
        return;
    }
    response.out() << news.dump();

}
