#include "bsonutils.h"

const bson_value_t* BSONUtils::getField(bson_t *bson, const char *field_name)
{
    bson_iter_t it;

    if( bson_iter_init_find(&it, bson, field_name) == true )
    {
        return bson_iter_value(&it);
    }

    return nullptr;
}

char* BSONUtils::getString(bson_t *bson, const char *field_name)
{
    bson_value_t* ret;
    if( ( ret = BSONUtils::getField(bson, field_name) ) != nullptr )
    {
        //We are searching only the string symbol
        if(ret->value_type == BSON_TYPE_SYMBOL)
        {
            return ret->value.v_symbol.symbol;
        }
    }

    return "";
}

int BSONUtils::getInt(bson_t *bson, const char *field_name)
{
    bson_value_t* ret;
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

double BSONUtils::getDouble(bson_t *bson, const char *field_name)
{
    bson_value_t* ret;
    if( ( ret = BSONUtils::getField(bson, field_name) ) != nullptr )
    {
        //We are searching only the string symbol
        if(ret->value_type == BSON_TYPE_DOUBLE)
        {
            return ret->value.v_double;
        }
    }

    return -1.0;
}

char* BSONUtils::getArray(bson_t *bson, const char *field_name)
{
    return "";
}


