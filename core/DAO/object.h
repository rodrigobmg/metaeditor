#ifndef OBJECT_H
#define OBJECT_H

#include <bson.h>

class Object
{
public:
    explicit Object(const bson_t* data);
    virtual ~Object();
    const char* Name();
    const char* Id();

public:
    virtual bool serialize(char* json);
    virtual bool unserialize();

protected:
    bson_t* m_data;
    char*   m_id;
    char*   m_name;
};

#endif // OBJECT_H
