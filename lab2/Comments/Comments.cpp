﻿#include "Comments.h"
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
    json commentJson = tryParsejson(getRequestBody(request));
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
    json body = tryParsejson(getRequestBody(request));
    int32_t id = body["commentId"].get<int32_t>();
    if (request.method() != "POST" or !body.count("commentId")) {
        response.setStatus(403);
        return;
    }
    json likeResult = model->likeComment(id);
    if (likeResult.empty()) {
        response.out() << "Cant like comment";
        response.setStatus(500);
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
    json body = tryParsejson(getRequestBody(request));
    int32_t id = body["userId"].get<int32_t>();
    if (request.method() != "DELETE" or !body.count("userId")) {
        response.setStatus(403);
        return;
    }
    json result = model->deleteComments(id);
    if (result.empty()) {
        response.out() << "Cant delete comments";
        response.setStatus(500);
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
    int32_t newsId = extractPositiveParam(request, "newsId");
    int32_t page = extractPositiveParam(request, "page");
    if (request.method() != "GET" or newsId < 0 or page < 0) {
        response.setStatus(403);
        return;
    }
    json news = model->getComments(newsId, page);
    if (news.empty()) {
        response.out() << "Cant find comments";
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
    auto ids = request.getParameterValues("id");
    if (request.method() != "GET") {
        response.setStatus(403);
        return;
    }
    std::vector<int32_t> idsInt;
    for (auto id : ids) {
        idsInt.push_back(atoi(id.c_str()));
    }
    json comments = model->getCommentsById(idsInt);
    if (comments.empty()) {
        response.out() << "Cant find names comments";
        return;
    }
    response.out() << comments.dump();
}
