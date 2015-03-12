#ifndef SOFTWAREBASE_H
#define SOFTWAREBASE_H

#include <vector>
#include <DAO/object.h>

#include "User.h"

const int MaxBaseName = 50;

class SoftwareBase : public Object
{
public:
    SoftwareBase();

private:
    char* m_description;
    std::vector<User> users;
};

#endif // SOFTWAREBASE_H
