#ifndef CXJSON_H
#define CXJSON_H


#include "cxglobal.h"

#define RAPIDJSON_ASSERT(x) void(0)

#include "rapidjson/rapidjson.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/reader.h"
#include "rapidjson/writer.h"
#include "rapidjson/document.h"
#include "rapidjson/pointer.h"
#include "rapidjson/filereadstream.h"
#include "rapidjson/filewritestream.h"


class GM_CCXX_CORE_API CxJson
{
public:
    /**
     * load json file, if load false, return.HasParseError() == true;
     * and you can judge
     * @param sFilePath
     * @return
     */
    static inline rapidjson::Document load(const std::string &sFilePath) { return load(sFilePath, 1024 * 1024 * 30); }

    /**
     * load json file, if load false, return.HasParseError() == true;
     * @param sFilePath
     * @param iMaxSize : load file 's limit max size
     * @return
     */
    static rapidjson::Document load(const std::string &sFilePath, size_t iMaxSize);

    /**
     * load json file, return true(load success).
     * @param sFilePath
     * @param doc { rapidjson::Document }
     * @return true : load success
     */
    static inline bool load(const std::string &sFilePath, rapidjson::Document & doc) { return load(sFilePath, doc, 1024 * 1024 * 30); }

    /**
     * load json file, return true(load success).
     * @param sFilePath
     * @param doc
     * @param iMaxSize : load file 's limit max size
     * @return
     */
    static bool load(const std::string &sFilePath, rapidjson::Document & doc, size_t iMaxSize);

    /**
     * rapidjson::Document to json file
     * @param sFilePath
     * @param doc
     * @return
     */
    static bool save(const std::string &sFilePath, rapidjson::Document & doc);

    /**
     * save rapidjson::Document to json file
     * @param sFilePath
     * @param doc
     * @param iBufferSize : saveStream Buffer size
     * @return
     */
    static bool save(const std::string &sFilePath, rapidjson::Document & doc, size_t iBufferSize);

    /**
     * fine member, d must be object, or else return null
     * @param d
     * @param sMember
     * @return
     */
    static rapidjson::Value* findMember(rapidjson::Value& d, const char * sMember);

    static const rapidjson::Value* findMember(const rapidjson::Value &d, const char * sMember);

    static std::string findMemberToString(const rapidjson::Value & d, const char * sMember, bool * bFind = NULL);

    static bool findMemberToBoolean(const rapidjson::Value& d, const char * sMember, bool * bFind = NULL);

    static int findMemberToInt(const rapidjson::Value& d, const char * sMember, bool * bFind = NULL);

    static int64 findMemberToInt64(const rapidjson::Value& d, const char * sMember, bool * bFind = NULL);

    static double findMemberToDouble(const rapidjson::Value& d, const char * sMember, bool * bFind = NULL);

    /**
     *
     * @param v
     * @return
     */
    static std::string valueToString(const rapidjson::Value & v);

    /*
{
  "dbname": "bookstore",
  "tables": [
    {
      "tbname": "user",
      "rows" : [
        {"name" : "user1", "old" : 91},
        {"name" : "user2", "old" : 99}
        ]
    },
    {
      "tbname": "book",
      "rows" : [
        {"title" : "bookname1", "author" : "author1", "price" : 91},
        {"title" : "bookname2", "author" : "author2", "price" : 92}
        ]
    }
    ]
}
    */
    //*load one table
    static int loadTable(const std::string & sFilePath, std::vector<std::map<std::string, std::string> > & rows, const std::string & sDataBaseName = std::string(), const std::string & sTableName = std::string());

    static int loadTable(const rapidjson::Document & doc, std::vector<std::map<std::string, std::string> > & rows, const std::string & sDataBaseName = std::string(), const std::string & sTableName = std::string());

    //*load mult table first row
    static int loadTable(const std::string & sFilePath, std::map<std::string, std::map<std::string, std::string> > & rows, const std::string & sDataBaseName = std::string());

    static int loadTable(const rapidjson::Document & doc, std::map<std::string, std::map<std::string, std::string> > & rows, const std::string & sDataBaseName = std::string());

    //*load mult table
    static int loadTable(const std::string & sFilePath, std::map<std::string, std::vector<std::map<std::string, std::string> > > & tables, const std::string & sDataBaseName = std::string());

    static int loadTable(const rapidjson::Document & doc, std::map<std::string, std::vector<std::map<std::string, std::string> > > & tables, const std::string & sDataBaseName = std::string());

    //*save one table
    static int saveTable(const std::string & sFilePath, const std::vector<std::map<std::string, std::string> > & rows, const std::string & sDataBaseName = std::string(), const std::string & sTableName = std::string()) ;

    //*save mult table first row
    static int saveTable(const std::string & sFilePath, const std::map<std::string, std::map<std::string, std::string> > & rows, const std::string & sDataBaseName = std::string(), int iFormat = 0);

    //*load mult table
    static int saveTable(const std::string & sFilePath, std::map<std::string, std::vector<std::map<std::string, std::string> > > & tables, const std::string & sDataBaseName = std::string(), int iFormat = 0);

private:
    static std::string formatJson(const std::string &sSource, size_t * pLevel, int iFormat);

};

#endif // CXJSON_H
