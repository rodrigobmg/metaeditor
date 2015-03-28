#ifndef USER_H
#define USER_H

#include <DAO/object.h>


enum UserRole
{
    Owner,
    Member
};

enum UserPermission
{
    Create,
    Delete,
    Read,
    Update
};

class User : public Object
{
public:
    explicit User();
    explicit User(std::string& name);

private:
    UserRole m_role;
    UserPermission m_permission;
};

#endif // USER_H
