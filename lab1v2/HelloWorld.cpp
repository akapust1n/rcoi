#include "HelloWorld.h"


HelloWorld::~HelloWorld()
{
     beingDeleted();
}

void HelloWorld::handleRequest(const Wt::Http::Request &request, Wt::Http::Response &response)
{
   response.out() << " Hello world!";
}
