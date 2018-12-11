#include "LikeHistory.h"
#include "../Shared/HttpAssist.h"
#include "../Shared/JsonStructs.h"
#include "Model.h"

using namespace ns;
using namespace HttpAssist;

GetLikes::GetLikes(Model* _model)
    : Base(_model)
{
}

void GetLikes::handleRequest(const Http::Request& request, Http::Response& response)
{
    if (!HttpAssist::checkAuth(request, model->getSecretKey())) {
        WriteResponse(response, 400);
        return;
    }
    int32_t page = extractPositiveParam(request, "page");
    if (request.method() != "GET" or page == -1) {
        WriteResponse(response, 403);
        return;
    }

    json_t GetLikes = model->getLikes(page);
    if (GetLikes.empty()) {
        WriteResponse(response, 500);
        return;
    }
    response.setStatus(200);
    response.out() << GetLikes.dump();
}

WriteLike::WriteLike(Model* _model)
    : Base(_model)
{
}

void WriteLike::handleRequest(const Http::Request& request, Http::Response& response)
{
    if (!HttpAssist::checkAuth(request, model->getSecretKey())) {
        WriteResponse(response, 400);
        return;
    }
    json_t likeEntityjson = tryParsejson(getRequestBody(request));
    LikeEntity likeEntity;
    if (request.method() != "POST" or !from_json(likeEntityjson, likeEntity)) {
        WriteResponse(response, 403);
        return;
    }

    json_t likeWriteResult = model->writeLike(likeEntity.userId, likeEntity.commentId);
    if (likeWriteResult["entityId"].get<int32_t>() == -1) {
        WriteResponse(response, 208);
        return;
    }
    response.out() << likeWriteResult.dump();
}

DeleteLike::DeleteLike(Model* _model)
    : Base(_model)
{
}

void DeleteLike::handleRequest(const Http::Request& request, Http::Response& response)
{
    if (!HttpAssist::checkAuth(request, model->getSecretKey())) {
        WriteResponse(response, 400);
        return;
    }
    json_t likeEntityjson = tryParsejson(getRequestBody(request));
    LikeEntity likeEntity;
    if (request.method() != "DELETE" or !from_json(likeEntityjson, likeEntity)) {
        WriteResponse(response, 403);
        return;
    }

    json_t likeDeleteResult = model->deleteLike(likeEntity.userId, likeEntity.commentId);
    if (likeDeleteResult["entityId"].get<int32_t>() == -1) {
        WriteResponse(response, 500);
        return;
    }
    response.out() << likeDeleteResult.dump();
}
