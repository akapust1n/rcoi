#include "HelloWorld.h"


GetNews::~GetNews()
{
     beingDeleted();
}

void GetNews::handleRequest(const Wt::Http::Request &request, Wt::Http::Response &response)
{
   response.out() << " Hello world!";
}
