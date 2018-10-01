#ifndef COMMENTS_H
#define COMMENTS_H
#include "../Shared/Base.h"
#include <Wt/Http/Response.h>
#include <Wt/WResource.h>
#include <Wt/WServer.h>

class CreateComment : public Base {
public:
    CreateComment(Model* _model);

protected:
    virtual void handleRequest(const Wt::Http::Request& request, Wt::Http::Response& response);
};

class LikeComment : public Base {
public:
    LikeComment(Model* _model);

protected:
    virtual void handleRequest(const Wt::Http::Request& request, Wt::Http::Response& response);
};

class CountComment : public Base {
public:
    CountComment(Model* _model);

protected:
    virtual void handleRequest(const Wt::Http::Request& request, Wt::Http::Response& response);
};

class DeleteComments : public Base {
public:
    DeleteComments(Model* _model);

protected:
    virtual void handleRequest(const Wt::Http::Request& request, Wt::Http::Response& response);
};

#endif // COMMENTS_H
