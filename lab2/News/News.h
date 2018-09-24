#ifndef HELLOWORLD_H
#define HELLOWORLD_H
#include "../Shared/Base.h"
#include <Wt/Http/Response.h>
#include <Wt/WResource.h>
#include <Wt/WServer.h>

class GetNews : public Wt::WResource, public Base {
public:
    virtual ~GetNews();
    GetNews(Model* _model);

protected:
    virtual void handleRequest(const Wt::Http::Request& request, Wt::Http::Response& response);
};

#endif // HELLOWORLD_H
