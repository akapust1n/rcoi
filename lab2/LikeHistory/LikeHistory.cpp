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
        response.setStatus(401);
        return;
    }
    int32_t page = extractPositiveParam(request, "page");
    if (request.method() != "GET") {
        response.setStatus(403);
        return;
    }

    json_t newsResponse = model->getLikes(page);
    if (newsResponse.empty()) {
        response.setStatus(500);
        return;
    }
    response.out() << newsResponse.dump();
}

WriteLike::WriteLike(Model* _model)
    : Base(_model)
{
}

void WriteLike::handleRequest(const Http::Request& request, Http::Response& response)
{
    if (!HttpAssist::checkAuth(request, model->getSecretKey())) {
        response.setStatus(401);
        return;
    }
    json_t likeEntityjson = tryParsejson(getRequestBody(request));
    LikeEntity likeEntity;
    if (request.method() != "POST" or !from_json(likeEntityjson, likeEntity)) {
        response.setStatus(403);
        return;
    }

    json_t newsResponse = model->writeLike(likeEntity.userId, likeEntity.commentId);
    if (newsResponse.empty()) {
        response.setStatus(500);
        return;
    }
    response.out() << newsResponse.dump();
}
