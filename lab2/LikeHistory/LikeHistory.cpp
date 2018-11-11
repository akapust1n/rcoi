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
    int32_t page = extractPositiveParam(request, "page");
    if (request.method() != "GET") {
        response.setStatus(403);
        return;
    }

    json newsResponse = model->getLikes(page);
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
    json likeEntityJson = tryParsejson(getRequestBody(request));
    LikeEntity likeEntity;
    if (request.method() != "POST" or !from_json(likeEntityJson, likeEntity)) {
        response.setStatus(403);
        return;
    }

    json newsResponse = model->writeLike(likeEntity.userId, likeEntity.commentId);
    if (newsResponse.empty()) {
        response.setStatus(500);
        return;
    }
    response.out() << newsResponse.dump();
}
