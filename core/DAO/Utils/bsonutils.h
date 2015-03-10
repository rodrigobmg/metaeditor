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
    static const bson_value_t* getField(const bson_t *bson, const char* field_name);

public:
    static char* getString(const bson_t* bson, const char* field_name);
    static int getInt(const bson_t* bson, const char* field_name);
    static double getDouble(const bson_t *bson, const char* field_name);
    static char* getArray(const bson_t* bson, const char* field_name);
};

#endif // BSONUTILS_H
