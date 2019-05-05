#ifndef CXHIREDIS_H
#define CXHIREDIS_H

#include "cxglobal.h"

class CxHiredis
{
public:
    static bool checkIsOk(const std::string & sReturn);

    // *** redis client to host ***
    /**
     * 加载客户端并连接服务器，成功返回对象实例（并默认 select db is 0），失败返回null(0)\
     * 如果已经加载，略过
     * 如果客户端连接自己断开，每次以下的操作会尝试连接再进行操作
     * @param sIpAddress
     * @param iPort
     * @return (true)成功 或 (false)失败
     */
    static bool loadClient(const std::string& sIpAddress = std::string("127.0.0.1"), int iPort = 6379);

    /**
     * 卸载客户端。
     * 卸载后，所有以下操作都会失败
     */
    static void unloadClient();

    /**
    * 返回客户端是否已经加载
    */
    static bool isLoadedClient();

    /**
     * 切换到指定的数据库，数据库索引号 index 用数字值指定，以 0 作为起始索引值。
     * @param iIndex
     * @return client unload时返回：string(""); 成功返回 OK
     * SELECT index
     */
    static std::string select(int iIndex);

    /**
     * 客户端向 Redis 服务器发送一个 PING ，如果服务器运作正常的话，会返回一个 PONG 。
        通常用于测试与服务器的连接是否仍然生效，或者用于测量延迟值。
     * @return host unload时返回：string(""); 如果连接正常就返回一个 PONG ，否则返回一个连接错误。
     * PING
     */
    static std::string ping();

    /**
     * 获取 加载成功后 host 的 ip
     * @return
     */
    static std::string serverIpAddress();

    /**
     * 获取 加载成功后 host 的 port
     * @return
     */
    static int serverPort();

    /**
     * 获取 加载成功后 的时间
     * @return
     */
    static msepoch_t loadedClientTime();

    /**
     * 获取最后的错误信息
     * @return
     */
    static std::string lastCommandError();

    /**
     * 获取最后的命令
     * @return
     */
    static std::string lastCommand();

    /**
     * 获取最后的命令发出时间
     * @return
     */
    static msepoch_t lastSendCommandTime();

    /**
     * 返回 key 的数据类型，数据类型有：
        none (key不存在)
        string (字符串)
        list (列表)
        set (集合)
        zset (有序集)
        hash (哈希表)
     * @param sKey
     * @return {std::string}
     * TYPE KEY_NAME
     */
    static std::string getKeyType(const std::string & sKey);

    /**
     * 删除已存在的键。不存在的 key 会被忽略。
     * @param sKey
     * @return 被删除 key 的数量。
     * DEL KEY_NAME
     */
    static int deleteKey(const std::string & sKey);

    // *** value type is string
    /**
     * 获取指定 key 的值。如果 key 不存在，返回 nil 。如果key 储存的值不是字符串类型，返回一个错误。
     * @param sKey
     * @return 返回 key 的值，如果 key 不存在时，返回 nil(std::string(""))。 如果 key 不是字符串类型，那么返回一个错误(std::string(""))。
     * GET KEY_NAME
     */
    static std::string stringGet(const std::string & sKey);

    /**
     * 设置给定 key 的值。如果 key 已经存储其他值， SET 就覆写旧值，且无视类型。
     * @param sKey
     * @param sValue
     * @return SET 在设置操作成功完成时，才返回 OK 。
     * SET KEY_NAME VALUE
     */
    static std::string stringSet(const std::string & sKey, const std::string & sValue);

    // *** value type is list
    /**
     * 命令将一个或多个值插入到列表头部。 如果 key 不存在，一个空列表会被创建并执行 LPUSH 操作。 当 key 存在但不是列表类型时，返回一个错误（-1）。
     * @param sKey
     * @param sValue
     * @return 执行 LPUSH 命令后，列表的长度。
     * LPUSH KEY_NAME VALUE1.. VALUEN
     */
    static int listPush(const std::string & sKey, const std::string & sValue);

    /**
     * 命令将一个或多个值插入到列表头部。 如果 key 不存在，一个空列表会被创建并执行 LPUSH 操作。 当 key 存在但不是列表类型时，返回一个错误（-1）。
     * @param sKey
     * @param sValues
     * @return 执行 LPUSH 命令后，列表的长度。
     * LPUSH KEY_NAME VALUE1.. VALUEN
     */
    static int listPushMultiple(const std::string & sKey, const std::vector<std::string> & sValues);

    /**
     * 返回列表的长度。 如果列表 key 不存在，则 key 被解释为一个空列表，返回 0 。 如果 key 不是列表类型，返回一个错误（-1）。
     * @param sKey
     * @return 列表的长度。
     * LLEN KEY_NAME
     */
    static int listGetLength(const std::string & sKey);

    /**
     * 返回列表中指定区间内的元素，区间以偏移量 iStart 和 iStop 指定。 其中 0 表示列表的第一个元素，
     *  1 表示列表的第二个元素，以此类推。 你也可以使用负数下标，以 -1 表示列表的最后一个元素， -2 表示列表的倒数第二个元素，以此类推。
     * @param sKey
     * @param iStart
     * @param iStop
     * @return 一个列表，包含指定区间内的元素。
     * LRANGE KEY_NAME START END
     */
    static std::vector<std::string> listGet(const std::string & sKey, int iStart, int iStop);

    /**
     * 根据参数 COUNT 的值，移除列表中与参数 VALUE 相等的元素。
        COUNT 的值可以是以下几种：
        count > 0 : 从表头开始向表尾搜索，移除与 VALUE 相等的元素，数量为 COUNT 。
        count < 0 : 从表尾开始向表头搜索，移除与 VALUE 相等的元素，数量为 COUNT 的绝对值。
        count = 0 : 移除表中所有与 VALUE 相等的值。
     * @param sKey
     * @param iCount
     * @param sKey
     * @return 被移除元素的数量。 列表不存在时返回 0 。
     * LREM KEY_NAME COUNT VALUE
     */
    static int listRemove(const std::string & sKey, int iCount, const std::string & sValue);

    /**
     * 对一个列表进行修剪(trim)，就是说，让列表只保留指定区间内的元素，不在指定区间之内的元素都将被删除。
        下标 0 表示列表的第一个元素，以 1 表示列表的第二个元素，以此类推。 你也可以使用负数下标，以 -1 表示列表的最后一个元素，
        -2 表示列表的倒数第二个元素，以此类推。
     * @param sKey
     * @param iStart
     * @param iStop
     * @return 命令执行成功时，返回 ok 。
     * LTRIM KEY_NAME START STOP
     */
    static std::string listTrim(const std::string & sKey, int iStart, int iStop);

    /**
     * 通过索引来设置元素的值。
        当索引参数超出范围，或对一个空列表进行 LSET 时，返回一个错误。
        关于列表下标的更多信息，请参考 LINDEX 命令。
     * @param sKey
     * @param iIndex
     * @param sValue
     * @return 操作成功返回 ok ，否则返回错误信息。
     * LSET KEY_NAME INDEX VALUE
     */
    static std::string listSet(const std::string & sKey, int iIndex, const std::string & sValue);

    // *** value type is hash
    /**
     * 命令用于获取哈希表中字段的数量。
     * @param sKey
     * @return 哈希表中字段的数量。 当 key 不存在时，返回 0 。
     * HLEN KEY_NAME
     */
    static int hashGetLength(const std::string & sKey);

    /**
     * 哈希表的指定字段是否存在。
     * @return 如果哈希表含有给定字段，返回 1 。 如果哈希表不含有给定字段，或 key 不存在，返回 0 。
     * HEXISTS KEY_NAME FIELD_NAME
     */
    static int hashExists(const std::string & sKey, const std::string & sFieldName);

    /**
     * 返回哈希表中指定字段的值。
     * @param sKey
     * @param sFieldName
     * @return 返回给定字段的值。如果给定的字段或 key 不存在时，返回 nil (string(""))。
     * HGET KEY_NAME FIELD_NAME
     */
    static std::string hashGet(const std::string & sKey, const std::string & sFieldName);

    /**
     * 为哈希表中的字段赋值 。
        如果哈希表不存在，一个新的哈希表被创建并进行 HSET 操作。
        如果字段已经存在于哈希表中，旧值将被覆盖。
     * @param sKey
     * @param sFieldName
     * @param sFieldValue
     * @return
     * HSET KEY_NAME FIELD_NAME FIELD_VALUE
     */
    static int hashSet(const std::string & sKey, const std::string & sFieldName, const std::string & sFieldValue);

    /**
     * 删除哈希表 key 中的一个或多个指定字段，不存在的字段将被忽略。
     * @param sKey
     * @param sFieldName
     * @param sFieldValue
     * @return 被成功删除字段的数量，不包括被忽略的字段。
     * HDEL KEY_NAME FIELD1.. FIELDN
     */
    static int hashDelete(const std::string & sKey, std::string & sFieldName);
    static int hashDelete(const std::string & sKey, const std::vector<std::string> & sFieldNames);

    /**
     * 返回哈希表中，所有的字段和值。
        在返回值里，紧跟每个字段名(field name)之后是字段的值(value)，所以返回值的长度是哈希表大小的两倍。（封装成 map 形式）
     * @param sKey
     * @return 以列表形式返回哈希表的字段及字段值。 若 key 不存在，返回空列表。（封装成 map 形式，不重复（但原生支持重复））
     * HGETALL KEY_NAME
     */
    static std::map<std::string, std::string> hashGetAll(const std::string & sKey);

    /**
     * 获取哈希表中的所有域（field）。
     * @param sKey
     * @return 包含哈希表中所有域（field）列表。 当 key 不存在时，返回一个空列表。
     * HKEYS key
     */
    static std::vector<std::string> hashGetKeys(const std::string & sKey);

    /**
     * 返回哈希表中，一个或多个给定字段的值。
        如果指定的字段不存在于哈希表，那么返回一个 nil 值。
     * @param sKey
     * @param sFieldNames
     * @return 一个包含多个给定字段关联值的表，表值的排列顺序和指定字段的请求顺序一样。
     * HMGET KEY_NAME FIELD1...FIELDN
     */
    static std::map<std::string, std::string> hashGetMultiple(const std::string & sKey, const std::vector<std::string> & sFieldNames);

    /**
     * 同时将多个 field-value (字段-值)对设置到哈希表中。
        此命令会覆盖哈希表中已存在的字段。
        如果哈希表不存在，会创建一个空哈希表，并执行 HMSET 操作。
     * @param sKey
     * @param sFieldNameValues
     * @return 如果命令执行成功，返回 OK 。
     * HMSET KEY_NAME FIELD1 VALUE1 ...FIELDN VALUEN
     */
    static std::string hashSetMultiple(const std::string & sKey, const std::map<std::string, std::string> & sFieldNameValues);
    
};

#endif //CXHIREDIS_H
