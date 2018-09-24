#include "News.h"
#include "../Shared/HttpAssist.h"
#include "Model.h"
using namespace ns;
using namespace HttpAssist;

GetNews::~GetNews()
{
    beingDeleted();
}

GetNews::GetNews(Model* _model)
    : Base(_model)
{
}

void GetNews::handleRequest(const Wt::Http::Request& request, Wt::Http::Response& response)
{
    std::string result;
    int32_t countNews = extractPositiveParam(request, "countNews");
    LOG_INFO("zzz");
    std::cout << countNews << std::endl;
    if (request.method() != "GET" or countNews < 0 or countNews > 50) {
        response.setStatus(403);
        return;
    }
    json news = model->getNews(countNews);
    std::string zz = news.dump();
    response.out() << zz;
}
