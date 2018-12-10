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

    json_t newsResponse = model->getLikes(page);
    if (newsResponse.empty()) {
        WriteResponse(response, 500);
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
        WriteResponse(response, 400);
        return;
    }
    json_t likeEntityjson = tryParsejson(getRequestBody(request));
    LikeEntity likeEntity;
    if (request.method() != "POST" or !from_json(likeEntityjson, likeEntity)) {
        WriteResponse(response, 403);
        ;
        return;
    }

    json_t newsResponse = model->writeLike(likeEntity.userId, likeEntity.commentId);
    if (newsResponse.empty()) {
        WriteResponse(response, 500);
        return;
    }
    response.out() << newsResponse.dump();
}
