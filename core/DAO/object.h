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
    //explicit Object(const bson_t* data);
    explicit Object(char* name);
    virtual ~Object();

    //Properties
    const char* Name();
    const char* Id();
    //const bson_t* Data();
    void setName(char* name);

    //Utils
    virtual void wrap(bson_t* data);
    virtual bool unwrap(const bson_t *data);

protected:
    ///
    /// \brief m_data
    /// Ponteiro para o objeto [BSON](http://bsonspec.org/).
    /// Este objeto representa o formato utilizado pelo
    /// banco de dados [Mongo](http://www.mongodb.org)
    ///
    //bson_t* m_data;
    char*   m_id;
    char*   m_name;
};

#endif // OBJECT_H
