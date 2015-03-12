#include <gtest/gtest.h>
#include <iostream>

//Core Lib imports
#include <DAO/object.h>
#include <DAO/databasemanager.h>
#include <DAO/Utils/bsonutils.h>
//Bson
#include <bson.h>

//Funções utilitárias usadas nos testes
static bson_t * createBson(const char* name)
{
    bson_t* bson = bson_new();

    ////////////////////////////////
    if( bson == nullptr )
    {
        return nullptr;
    }
    ////////////////////////////////

    bson_oid_t oid;
    bson_oid_init(&oid, NULL);

    BSON_APPEND_OID(bson, "_id", &oid);
    BSON_APPEND_UTF8(bson, "name", name);

    return bson;
}

///
/// \brief Teste responsável por verificar se a inclusão ao banco
/// de dados foi executada com sucesso.
/// Há somente dois casos onde o teste pode falhar:
/// 1) Conexão não existente - Caso não haja uma conexão com o
/// banco de dados uma excessão será levantada.
/// 2) Objeto já existe - O nome de objeto já existe na tabela e
/// portanto não pode ser inserido novamente.
///
TEST(CoreLib, DatabaseManagerCreateOK)
{
    try
    {
        char* name = new char[20];
        strcpy(name, "insertTest");
        Object o(name);
        int ret = DatabaseManager::instance().create(o, "users", "test");
        ASSERT_TRUE(ret == true);
    }
    catch(DatabaseException& ex)
    {
        FAIL() << "Falha de conexão: " << ex.what() << std::endl;
    }
}

///
/// \brief Segundo a documentação da API, sempre que uma inserção
/// de um novo documento é feita tanto o banco de dados e as tabelas
/// são criadas caso não tenham sido previamente cadastradas. Dessa
/// forma, o teste de criação apenas checa se já existe um nome na
/// base de dados, como queremos garantir que dados inseridos em uma
/// tabela sejam únicos, caso o nome informado exista, então não deixamos
/// que a inserção ocorra.
/// \author Rodrigo Monteiro
/// \version 0.1
///
TEST(CoreLib, DatabaseManagerCreateFAIL)
{
    try
    {
        char* name = new char[20];
        strcpy(name, "insertTest");
        Object o(name);
        int ret = DatabaseManager::instance().create(o, "users", "test");
        ASSERT_TRUE(ret == false);
    }
    catch(DatabaseException& ex)
    {
        FAIL() << "Falha de conexão: " << ex.what() << std::endl;
    }
}

///
/// \brief Teste responsável por checar se é possível recuperar
/// um objeto cadastrado do banco de dados.
/// O teste falha em 2 ocasiões:
/// 1) Conexão não existente - Caso não haja uma conexão com o
/// banco de dados uma excessão será levantada.
/// 2) Objeto não existe - O nome informado não existe na tabela
/// e o ponteiro para o objeto será nulo.
///
TEST(CoreLib, DatabaseManagerReadOK)
{
    try
    {
        Object* obj = DatabaseManager::instance().read("users", "insertTest", "test");
        //
        ASSERT_TRUE(obj != nullptr);
    }
    catch(DatabaseException& ex)
    {
        FAIL() << "Falha de conexão: " << ex.what() << std::endl;
    }
}

///
/// \brief Teste responsável por afirmar que caso não haja
/// o objeto na tabela informada o retorno da função será
/// um ponteiro nulo para o objeto.
/// O teste falha em 2 ocasiões:
/// 1) Conexão não existente - Caso não haja uma conexão com o
/// banco de dados uma excessão será levantada.
/// 2) Ponteiro para objeto diferente de nulo - Está opção não
/// deveria ser cogitada, mas pode ocorrer da memória estar com
/// um lixo que seja igual ao nome informado para busca
///
TEST(CoreLib, DatabaseManagerReadFAIL)
{
    try
    {
        Object* obj = DatabaseManager::instance().read("users", "insertTest1", "test");
        //
        ASSERT_TRUE(obj == nullptr);
        //
        delete obj;
    }
    catch(DatabaseException& ex)
    {
        FAIL() << "Falha de conexão: " << ex.what() << std::endl;
    }
}

///
/// \brief Teste responsável por afirmar que caso não haja
/// o objeto na tabela informada o retorno da função será
/// um ponteiro nulo para o objeto.
/// O teste falha em 2 ocasiões:
/// 1) Conexão não existente - Caso não haja uma conexão com o
/// banco de dados uma excessão será levantada.
/// 2) Ponteiro para objeto diferente de nulo - Está opção não
/// deveria ser cogitada, mas pode ocorrer da memória estar com
/// um lixo que seja igual ao nome informado para busca
///
TEST(CoreLib, DatabaseManagerDestroyOK)
{
    try
    {
        Object* obj = DatabaseManager::instance().read("users", "insertTest", "test");
        //
        ASSERT_TRUE(obj != nullptr);
        //
        bool ret = DatabaseManager::instance().destroy(*obj, "users", "test");
        //
        ASSERT_TRUE(ret == true);

        delete obj;
    }
    catch(DatabaseException& ex)
    {
        FAIL() << "Falha de conexão: " << ex.what() << std::endl;
    }
}

TEST(CoreLib, DatabaseManagerDestroyFAIL)
{
    try
    {
        char* name = new char[20];
        strcpy(name, "insertTest");
        Object o(name);
        //Objeto não existe na base de dados
        bool ret = DatabaseManager::instance().destroy(o, "users", "test");
        //
        ASSERT_TRUE(ret == false);
    }
    catch(DatabaseException& ex)
    {
        FAIL() << "Falha de conexão: " << ex.what() << std::endl;
    }
}

///////////////
/// \brief main
/// \param argc
/// \param argv
/// \return
///
int main(int argc, char** argv)
{
    try
    {
        DatabaseManager::instance().connect();
    }
    catch(DatabaseException& ex)
    {
        std::cout << "Exceção: " << ex.what() << std::endl;
        return -1;
    }

    //Roda os testes
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
