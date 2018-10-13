#ifndef NEWS_H
#define NEWS_H
#include "../Shared/Base.h"
#include "Model.h"
#include <Wt/Http/Response>
#include <Wt/WResource.h>
#include <Wt/WServer.h>

class GetTitles : public Base {
public:
    GetTitles(Model* _model);

protected:
    virtual void handleRequest(const Wt::Http::Request& request, Wt::Http::Response& response);
};

class CreateNews : public Base {
public:
    CreateNews(Model* _model);

protected:
    virtual void handleRequest(const Wt::Http::Request& request, Wt::Http::Response& response);
};

class OneNews : public Base {
public:
    OneNews(Model* _model);

protected:
    virtual void handleRequest(const Wt::Http::Request& request, Wt::Http::Response& response);
};
#ifdef IS_TEST_BUILD
class Clear : public Base {
public:
    Clear(Model* _model);

protected:
    virtual void handleRequest(const Wt::Http::Request& request, Wt::Http::Response& response);
};
#endif

#endif // NEWS_H
