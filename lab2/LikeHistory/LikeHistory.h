#ifndef LIKEHISTORY_H
#define LIKEHISTORY_H
#include "../Shared/Base.h"
#include <Wt/Http/Response.h>
#include <Wt/WResource.h>
#include <Wt/WServer.h>

class GetLikes : public Base {
public:
    GetLikes(Model* _model);

protected:
    virtual void handleRequest(const Wt::Http::Request& request, Wt::Http::Response& response);
};

class WriteLike : public Base {
public:
    WriteLike(Model* _model);

protected:
    virtual void handleRequest(const Wt::Http::Request& request, Wt::Http::Response& response);
};
class DeleteLike : public Base {
public:
    DeleteLike(Model* _model);

protected:
    virtual void handleRequest(const Wt::Http::Request& request, Wt::Http::Response& response);
};

#endif // LIKEHISTORY_H
