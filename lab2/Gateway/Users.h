#ifndef USERS_H
#define USERS_H
#include "../Shared/Base.h"
#include "Model.h"
#include <Wt/Http/Response>
#include <Wt/WResource.h>
#include <Wt/WServer.h>

class Login : public Base {
public:
    Login(Model* _model);

public:
    virtual void handleRequest(const Wt::Http::Request& request, Wt::Http::Response& response);
};

class Register : public Base {
public:
    Register(Model* _model);

protected:
    virtual void handleRequest(const Wt::Http::Request& request, Wt::Http::Response& response);
};

class DelUser : public Base {
public:
    DelUser(Model* _model);

protected:
    virtual void handleRequest(const Wt::Http::Request& request, Wt::Http::Response& response);
};

#endif // USERS_H
