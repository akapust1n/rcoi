#ifndef HELLOWORLD_H
#define HELLOWORLD_H
#include <Wt/Http/Response>
#include <Wt/WResource.h>
#include <Wt/WServer.h>

class GetNews : public Wt::WResource {
public:
    virtual ~GetNews();

protected:
    virtual void handleRequest(const Wt::Http::Request& request, Wt::Http::Response& response);
};

#endif // HELLOWORLD_H
