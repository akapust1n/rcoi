#include "Model.h"
#include "../Shared/Database.h"
using namespace Wt::Dbo;
using namespace ns;

Model::Model()
{
}

const json Model::getNews(int32_t numNews)
{
    db = Db::GetInst()->GetMysql();

    json result = json::array();
    //, UNIX_TIMESTAMP(creationDate)
    auto req = db->prepareStatement(fmt::format("SELECT ID, title, body from News order by creationDate limit {0}", numNews));

    if (!req) {
        LOG_ERROR("Cant prepare statement");
    } else {
        req->execute();
        while (req->nextRow()) {
            News news;
            req->getResult(0, &news.ID);
            req->getResult(1, &news.title, 255);
            req->getResult(2, &news.body, 255);
            //req->getResult(3, &news.timestamp, 255);
            result.push_back(news);
        };
    }
    return result;
}
