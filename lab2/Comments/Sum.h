#ifndef SUM_H
#define SUM_H
#include <Wt/WResource.h>
#include <Wt/WServer.h>
#include <Wt/Http/Response>


class Sum: public Wt::WResource
{
public:
    virtual ~Sum();

protected:
    virtual void handleRequest(const Wt::Http::Request& request, Wt::Http::Response& response);
};

#endif // SUM_H
