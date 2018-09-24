#include "Sum.h"


Sum::~Sum()
{
    beingDeleted();

}

void Sum::handleRequest(const Wt::Http::Request &request, Wt::Http::Response &response)
{
    const std::string  *a = request.getParameter("a");
    const std::string  *b = request.getParameter("b");

    if (a && b)
        response.out()  << std::to_string(std::stoi(*a) + std::stoi(*b));
     else
        response.out() << "error";
}
