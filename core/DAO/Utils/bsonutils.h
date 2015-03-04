#ifndef BSONUTILS_H
#define BSONUTILS_H

#include <bson.h>

/**
 * @brief The BSONUtils class
 * Utility static class to help at obtain data from bson
 */
class BSONUtils
{
private:
    static const bson_value_t* getField(bson_t* bson, const char* field_name);

public:
    static char* getString(bson_t* bson, const char* field_name);
    static int getInt(bson_t* bson, const char* field_name);
    static double getDouble(bson_t* bson, const char* field_name);
    static char* getArray(bson_t* bson, const char* field_name);
};

#endif // BSONUTILS_H
