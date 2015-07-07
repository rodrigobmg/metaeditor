#ifndef DATAACCESS_H
#define DATAACCESS_H

#include <redox.hpp>

namespace Data {

static const std::string DefaultHost = "locahost";
static const u_int16_t DefaultPortNumber = 6379;

class RedisWrapper
{
private:
    const std::string CommandZAdd = "ZADD";

public:

    enum CASCADE
    {
        ON,
        OFF
    };

    RedisWrapper();
    virtual ~RedisWrapper();

    /**
     * @brief Connect Realiza a conexão ao banco de dados Redis
     * @param host Ip do servidor
     * @param port Porta do servidor
     * @return Verdadeiro se a conexão foi estabelecida com sucesso
     */
    bool Connect(const std::string& host = DefaultHost, const u_int16_t port = DefaultPortNumber);

    /**
     * @brief Insert Utilizado para inserir atributos ao banco de dados
     * @param key a chave referente ao que está sendo inserido
     * @param value o valor a ser inserido
     * @return Verdadeiro se a inserção for um sucesso
     */
    bool Insert(const std::string& key, const std::string& value);
    /**
     * @brief Remove
     * @param key
     * @return
     */
    bool Remove(const std::string& key);
    /**
     * @brief CreateSet
     * @param key
     * @return
     */
    bool CreateSet(const std::string& key);
    /**
     * @brief InsertSet
     * @param key
     * @param value
     * @return
     */
    bool InsertSet(const std::string& key, const std::string& value);
    /**
     * @brief RemoveSet Remove um conjunto de dados do banco nosql
     *        Pode promover um efeito cascata caso definido pelo desenvolvedor
     * @param key A chave do conjunto
     * @return Verdadeiro se a o conjunto foi removido com sucesso.
     */
    bool RemoveSet(const std::string& key, CASCADE cascade);
    /**
     * @brief Append
     * @param key
     * @param value
     * @return
     */
    bool Append(const std::string& key, const std::string& value);

private:

    bool Command(const std::string& command, std::vector<std::string> &values);

    bool IsConnected() const;

private:
    bool _connected;
    redox::Redox _rdx;
};

}
#endif // DATAACCESS_H
