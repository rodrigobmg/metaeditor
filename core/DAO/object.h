#ifndef OBJECT_H
#define OBJECT_H

#include <bson.h>

///
/// \brief MaxIdSize
/// Acima de 300 podem ocorrer problemas
///
const int MaxIdSize = 256;

class Object
{
public:
    explicit Object();
    explicit Object(char* name);
    virtual ~Object();

    //Properties
    const char* Name();
    const char* Id();
    void setName(char* name);

    //Utils
    virtual void wrap(bson_t* data);
    virtual bool unwrap(const bson_t *data);

private:
    char*   m_id;
    char*   m_name;
};

#endif // OBJECT_H
