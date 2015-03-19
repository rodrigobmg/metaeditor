#include "bsonutils.h"

const bson_value_t* BSONUtils::getField(const bson_t *bson, const char *field_name)
{
    bson_iter_t it;
    if( bson_iter_init_find(&it, bson, field_name) == true )
    {
        return bson_iter_value(&it);
    }

    return nullptr;
}

char* BSONUtils::getString(const bson_t *bson, const char *field_name)
{
    const bson_value_t* ret;
    if( ( ret = BSONUtils::getField(bson, field_name) ) != nullptr )
    {
        if(ret->value_type == BSON_TYPE_UTF8)
        {
            return ret->value.v_utf8.str;
        }
    }

    return "";
}

int BSONUtils::getInt(const bson_t *bson, const char *field_name)
{
    const bson_value_t* ret;
    if( ( ret = BSONUtils::getField(bson, field_name) ) != nullptr )
    {
        //We are searching only the string symbol
        if(ret->value_type == BSON_TYPE_INT32)
        {
            return ret->value.v_int64;
        }
    }

    return -1;
}

double BSONUtils::getDouble(const bson_t *bson, const char *field_name)
{
    const bson_value_t* ret;
    if( ( ret = BSONUtils::getField(bson, field_name) ) != nullptr )
    {
        if(ret->value_type == BSON_TYPE_DOUBLE)
        {
            return ret->value.v_double;
        }
    }

    return -1.0;
}

char* BSONUtils::getArray(const bson_t *bson, const char *field_name)
{
    return "";
}

bool BSONUtils::getOID(const bson_t *bson, char* ret_oid)
{
    if( ret_oid == nullptr )
    {
        return false;
    }

    const bson_value_t* ret;
    if( ( ret = BSONUtils::getField(bson, "_id") ) != nullptr )
    {
        if(ret->value_type == BSON_TYPE_OID)
        {
            bson_oid_to_string(&ret->value.v_oid, ret_oid);
            return true;
        }
    }

    return false;
}
