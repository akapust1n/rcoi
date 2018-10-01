#ifndef BASE_H
#define BASE_H
#include "HttpAssist.h"
#include <Wt/Http/Client>
#include <Wt/Http/Response>
#include <Wt/WResource.h>
#include <string>

class Model;

class Base : public Wt::WResource {
public:
    Base(Model* _model)
        : model(_model)
    {
    }
    virtual ~Base()
    {
        beingDeleted();
    }

protected:
    Model* model;

protected:
    void writeOutput(const Wt::Http::Message& msg, Wt::Http::Response& response)
    {
        HttpAssist::writeHeaders(response, msg.headers());
        response.out() << msg.body();
        response.setStatus(msg.status());
    }
};

#endif // BaseL_H
