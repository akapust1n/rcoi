#ifndef USERS_H
#define USERS_H
#include "../Shared/Base.h"
#include <Wt/Http/Response.h>
#include <Wt/WResource.h>
#include <Wt/WServer.h>

class Login : public Base {
public:
    Login(Model* _model);

protected:
    virtual void handleRequest(const Wt::Http::Request& request, Wt::Http::Response& response);
};

class Reg : public Base {
public:
    Reg(Model* _model);

protected:
    virtual void handleRequest(const Wt::Http::Request& request, Wt::Http::Response& response);
};

class Del : public Base {
public:
    Del(Model* _model);

protected:
    virtual void handleRequest(const Wt::Http::Request& request, Wt::Http::Response& response);
};

class IncRating : public Base {
public:
    IncRating(Model* _model);

protected:
    virtual void handleRequest(const Wt::Http::Request& request, Wt::Http::Response& response);
};

#endif // USERS_H
