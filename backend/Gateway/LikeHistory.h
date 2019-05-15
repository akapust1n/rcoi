#ifndef LIKEHISTORY_H
#define LIKEHISTORY_H
#include "../Shared/Base.h"
#include "Model.h"
#include <Wt/Http/Response>
#include <Wt/WResource.h>
#include <Wt/WServer.h>

class History : public Base {
public:
    History(Model* _model);

public:
    virtual void handleRequest(const Wt::Http::Request& request, Wt::Http::Response& response);
};

#endif // LIKEHISTORY_H
