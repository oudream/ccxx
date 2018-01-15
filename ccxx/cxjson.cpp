#include "cxjson.h"

#include "cxstring.h"
#include "cxfile.h"

using namespace std;
using namespace rapidjson;

// todo:
// Value：：IsObject 中缺少模糊查找功能：如：/*/name == "name1"
// Value：：IsArray 中缺少插入
// Document：：缺少总点空间大小函数

rapidjson::Document CxJson::load(const std::string &sFilePath, size_t iMaxSize)
{
    Document doc; //
    FILE *pFile = fopen(sFilePath.c_str(), "rb"); // 非 Windows 平台使用 "r"
    if (pFile == NULL)
    {
        return doc;
    }
    fseek(pFile, 0, SEEK_END);
    size_t iFileSize = ftell(pFile);
    if (iFileSize <= 0 || iFileSize > iMaxSize)
    {
        fclose(pFile);
        return doc;
    }

    // allocate memory to contain the whole file:
    size_t iBufferSize = sizeof(char) * iFileSize * 2;
    string sOut;
    sOut.resize(sOut.size() + iBufferSize);
    if (sOut.size() < iBufferSize)
    {
        fclose(pFile);
        return doc;
    }

    rewind(pFile);

    FileReadStream is(pFile, (char *) sOut.c_str(), sOut.size());
    doc.ParseStream(is);
    fclose(pFile);

    return doc;
}

bool CxJson::load(const std::string &sFilePath, rapidjson::Document &doc, size_t iMaxSize)
{
    FILE *pFile = fopen(sFilePath.c_str(), "rb"); // 非 Windows 平台使用 "r"
    if (pFile == NULL)
    {
        return false;
    }
    fseek(pFile, 0, SEEK_END);
    size_t iFileSize = ftell(pFile);
    if (iFileSize <= 0 || iFileSize > iMaxSize)
    {
        fclose(pFile);
        return false;
    }

    // allocate memory to contain the whole file:
    size_t iBufferSize = sizeof(char) * iFileSize * 2;
    string sOut;
    sOut.resize(sOut.size() + iBufferSize);
    if (sOut.size() < iBufferSize)
    {
        fclose(pFile);
        return false;
    }

    rewind(pFile);

    FileReadStream is(pFile, (char *) sOut.c_str(), sOut.size());
    doc.ParseStream(is);
    fclose(pFile);

    return ! doc.HasParseError();
}

bool CxJson::save(const std::string &sFilePath, rapidjson::Document &doc)
{
    return save(sFilePath, doc, 1024 * 1024);
}

bool CxJson::save(const std::string &sFilePath, rapidjson::Document &doc, size_t iBufferSize)
{
    FILE *pFile;
    pFile = fopen(sFilePath.data(), "wb");  // 非 Windows 平台使用 "w"
    if (pFile == NULL) return false;
    rewind(pFile);
    string sOut;
    sOut.resize(sOut.size() + iBufferSize);
    if (sOut.size() < iBufferSize)
    {
        fclose(pFile);
        return false;
    }
    FileWriteStream os(pFile, (char *)sOut.c_str(), sOut.size());
    Writer<FileWriteStream> writer(os);
    bool r = doc.Accept(writer);
    fclose(pFile);
    return r;
}

rapidjson::Value *CxJson::findMember(rapidjson::Value &d, const char *sMember)
{
    if (d.IsObject())
    {
        rapidjson::Value::MemberIterator it = d.FindMember(sMember);
        if (it != d.MemberEnd())
        {
            rapidjson::Value &v = it->value;
            return &v;
        }
    }
    return NULL;
}

const rapidjson::Value *CxJson::findMember(const rapidjson::Value &d, const char *sMember)
{
    if (d.IsObject())
    {
        rapidjson::Value::ConstMemberIterator it = d.FindMember(sMember);
        if (it != d.MemberEnd())
        {
            const rapidjson::Value &v = it->value;
            return &v;
        }
    }
    return NULL;
}

string CxJson::findMemberToString(const rapidjson::Value &d, const char *sMember, bool *bFind)
{
    if (d.IsObject())
    {
        rapidjson::Value::ConstMemberIterator it = d.FindMember(sMember);
        if (it != d.MemberEnd())
        {
            const rapidjson::Value &v = it->value;
            if (v.IsString())
            {
                if (bFind) *bFind = true;
                return v.GetString();
            }
        }
    }
    if (bFind) *bFind = false;
    return string();
}

bool CxJson::findMemberToBoolean(const rapidjson::Value &d, const char *sMember, bool *bFind)
{
    if (d.IsObject())
    {
        rapidjson::Value::ConstMemberIterator it = d.FindMember(sMember);
        if (it != d.MemberEnd())
        {
            const rapidjson::Value &v = it->value;
            if (v.IsBool())
            {
                if (bFind) *bFind = true;
                return v.GetBool();
            }
        }
    }
    if (bFind) *bFind = false;
    return false;
}

int CxJson::findMemberToInt(const rapidjson::Value &d, const char *sMember, bool *bFind)
{
    if (d.IsObject())
    {
        rapidjson::Value::ConstMemberIterator it = d.FindMember(sMember);
        if (it != d.MemberEnd())
        {
            const rapidjson::Value &v = it->value;
            if (v.IsInt())
            {
                if (bFind) *bFind = true;
                return v.GetInt();
            }
        }
    }
    if (bFind) *bFind = false;
    return -1;
}

int64 CxJson::findMemberToInt64(const rapidjson::Value &d, const char *sMember, bool *bFind)
{
    if (d.IsObject())
    {
        rapidjson::Value::ConstMemberIterator it = d.FindMember(sMember);
        if (it != d.MemberEnd())
        {
            const rapidjson::Value &v = it->value;
            if (v.IsInt64())
            {
                if (bFind) *bFind = true;
                return v.GetInt64();
            }
        }
    }
    if (bFind) *bFind = false;
    return -1;
}

double CxJson::findMemberToDouble(const rapidjson::Value &d, const char *sMember, bool *bFind)
{
    if (d.IsObject())
    {
        rapidjson::Value::ConstMemberIterator it = d.FindMember(sMember);
        if (it != d.MemberEnd())
        {
            const rapidjson::Value &v = it->value;
            if (v.IsDouble())
            {
                if (bFind) *bFind = true;
                return v.GetDouble();
            }
        }
    }
    if (bFind) *bFind = false;
    return 0;
}

string CxJson::valueToString(const Value &v)
{
    if (v.IsString())
    {
        return v.GetString();
    }
    else if (v.IsInt())
    {
        return CxString::toString(v.GetInt());
    }
    else if (v.IsNumber())
    {
        return CxString::toString(v.GetDouble());
    }
    else if (v.IsBool())
    {
        return CxString::toString(v.GetBool());
    }
    else
    {
        StringBuffer buffer;
        Writer<StringBuffer> writer(buffer);
        v.Accept(writer);
        return string(buffer.GetString());
    }
}

int
CxJson::loadTable(const string &sFilePath, std::vector<std::map<string, string> > &rows, const string &sDataBaseName, const string &sTableName)
{
    string sFileData = CxString::trim(CxFile::load(sFilePath));
    if (sFileData.empty()) return 0;
    Document d;
    d.Parse(sFileData.data());
    if (d.HasParseError())
    {
        return -1;
    }
    return loadTable(d, rows, sDataBaseName, sTableName);
}

int
CxJson::loadTable(const rapidjson::Document &doc, std::vector<std::map<string, string> > &rows, const string &sDataBaseName, const string &sTableName)
{
    if (doc.HasParseError()) return -1;

    int r = 0;
    const rapidjson::Value *vTables = findMember(doc, "tables");
    if (vTables && vTables->IsArray())
    {
        for (SizeType i = 0; i < vTables->Size(); ++i)
        {
            const Value &vTable = (*vTables)[i];
            const rapidjson::Value *vTbName = findMember(vTable, "tbname");
            string sTbName = "";
            if (vTbName && vTbName->IsString())
            {
                sTbName = vTbName->GetString();
            }
            else
            {
                continue;
            }
            if (!CxString::equalCase(sTbName, sTableName))
            {
                continue;
            }
            const rapidjson::Value *vRows = findMember(vTable, "rows");
            if (vRows && vRows->IsArray())
            {
                for (SizeType i = 0; i < vRows->Size(); ++i)
                {
                    const Value &vRow = (*vRows)[i];
                    if (vRow.IsObject())
                    {
                        map<string, string> row;
                        for (Value::ConstMemberIterator itr = vRow.MemberBegin();
                             itr != vRow.MemberEnd(); ++itr)
                        {
                            string sKey = itr->name.GetString();
                            string sValue = valueToString(itr->value);
                            row[sKey] = sValue;
                        }
                        rows.push_back(row);
                        ++r;
                    }
                }
            }
        }
    }
    return r;
}

int
CxJson::loadTable(const string &sFilePath, std::map<string, std::map<string, string> > &rows, const string &sDataBaseName)
{
    string sFileData = CxString::trim(CxFile::load(sFilePath));
    if (sFileData.empty()) return 0;
    Document d;
    d.Parse(sFileData.data());
    if (d.HasParseError())
    {
        return -1;
    }
    return loadTable(d, rows, sDataBaseName);
}

int
CxJson::saveTable(const string &sFilePath, const std::vector<std::map<string, string> > &rows, const string &sDataBaseName, const string &sTableName)
{
    FILE *pFile;
    pFile = fopen(sFilePath.data(), "wb");
    if (pFile == NULL)
    {
        return -1;
    }

    string sDataBaseName2 = sDataBaseName.empty() ? string("root") : sDataBaseName;
    string sTableName2 = sTableName.empty() ? string("config") : sTableName;
    string sLine = CxString::format("{\"dbname\": \"%s\",\"tables\": [{\"tbname\": \"%s\",\"rows\" : [", sDataBaseName2.c_str(), sTableName2.c_str());
    size_t iWroteTotal = fwrite(const_cast<char *>(sLine.data()), 1, sLine.size(), pFile);
    size_t iRowsSize = rows.size();
    size_t iRowsEnd = rows.size() - 1;
    for (size_t i = 0; i < iRowsSize; ++i)
    {
        sLine = "{";
        const map<string, string> &sEntrys = rows.at(i);
        for (map<string, string>::const_iterator itEntry = sEntrys.begin(); itEntry != sEntrys.end(); ++itEntry)
        {
            sLine += "\"" + itEntry->first + "\":\"" + itEntry->second + "\",";
        }
        if (sLine.size() > 1)
        {
            char *pch = (char *) sLine.data();
            pch[sLine.size() - 1] = ' ';
        }
        if (i < iRowsEnd)
            sLine += "},";
        else
            sLine += "}";
        size_t iWrote = fwrite(const_cast<char *>(sLine.data()), 1, sLine.size(), pFile);
        if (iWrote != sLine.size())
        {
            fclose(pFile);
            return -1;
        }
        iWroteTotal += iWrote;
    }
    sLine = "]}]}";
    iWroteTotal += fwrite(const_cast<char *>(sLine.data()), 1, sLine.size(), pFile);
    fclose(pFile);
    return iWroteTotal;
}

int
CxJson::loadTable(const Document &doc, std::map<string, std::map<string, string> > &rows, const string &sDataBaseName)
{
    if (doc.HasParseError()) return -1;

    int r = 0;
    const rapidjson::Value *vTables = findMember(doc, "tables");
    if (vTables && vTables->IsArray())
    {
        for (SizeType i = 0; i < vTables->Size(); ++i)
        {
            const Value &vTable = (*vTables)[i];
            const rapidjson::Value *vTbName = findMember(vTable, "tbname");
            string sTbName = "";
            if (vTbName && vTbName->IsString())
            {
                sTbName = vTbName->GetString();
            }
            else
            {
                continue;
            }
            const rapidjson::Value *vRows = findMember(vTable, "rows");
            if (vRows && vRows->IsArray())
            {
                for (SizeType i = 0; i < vRows->Size(); ++i)
                {
                    const Value &vRow = (*vRows)[i];
                    if (vRow.IsObject())
                    {
                        map<string, string> row;
                        for (Value::ConstMemberIterator itr = vRow.MemberBegin();
                             itr != vRow.MemberEnd(); ++itr)
                        {
                            string sKey = itr->name.GetString();
                            string sValue = valueToString(itr->value);
                            row[sKey] = sValue;
                        }
                        rows[sTbName] = row;
                        ++r;
                    }
                }
            }
        }
    }
    return r;
}

int
CxJson::loadTable(const std::string &sFilePath, std::map<std::string, std::vector<std::map<std::string, std::string> > > &tables, const string &sDataBaseName)
{
    string sFileData = CxString::trim(CxFile::load(sFilePath));
    if (sFileData.empty()) return 0;
    Document d;
    d.Parse(sFileData.data());
    if (d.HasParseError())
    {
        return -1;
    }
    return loadTable(d, tables, sDataBaseName);
}

int
CxJson::loadTable(const rapidjson::Document &doc, std::map<std::string, std::vector<std::map<std::string, std::string> > > &tables, const string &sDataBaseName)
{
    if (doc.HasParseError()) return -1;

    int r = 0;
    const rapidjson::Value *vTables = findMember(doc, "tables");
    if (vTables && vTables->IsArray())
    {
        for (SizeType i = 0; i < vTables->Size(); ++i)
        {
            const Value &vTable = (*vTables)[i];
            const rapidjson::Value *vTbName = findMember(vTable, "tbname");
            string sTbName = "";
            if (vTbName && vTbName->IsString())
            {
                sTbName = vTbName->GetString();
            }
            else
            {
                continue;
            }
            const rapidjson::Value *vRows = findMember(vTable, "rows");
            if (vRows && vRows->IsArray())
            {
                vector<map<string, string> > table;
                for (SizeType i = 0; i < vRows->Size(); ++i)
                {
                    const Value &vRow = (*vRows)[i];
                    if (vRow.IsObject())
                    {
                        map<string, string> row;
                        for (Value::ConstMemberIterator itr = vRow.MemberBegin();
                             itr != vRow.MemberEnd(); ++itr)
                        {
                            string sKey = itr->name.GetString();
                            string sValue = valueToString(itr->value);
                            row[sKey] = sValue;
                        }
                        table.push_back(row);
                        ++r;
                    }
                }
                tables[sTbName] = table;
            }
        }
    }
    return r;
}

int
CxJson::saveTable(const string &sFilePath, const std::map<string, std::map<string, string> > &rows, const string &sDataBaseName, int iFormat)
{
    FILE *pFile;
    pFile = fopen(sFilePath.data(), "wb");
    if (pFile == NULL)
    {
        return -1;
    }
    string sDataBaseName2 = sDataBaseName.empty() ? string("root") : sDataBaseName;
    size_t iLevel = 0;
    size_t *pLevel = &iLevel;
    string sLine = formatJson("{", pLevel, iFormat);
    sLine += formatJson(CxString::format("\"dbname\": \"%s\",", sDataBaseName2.c_str()), pLevel, iFormat);
    sLine += formatJson("\"tables\": [", pLevel, iFormat);
//    string sLine = CxString::format("{\"dbname\": \"%s\",\"tables\": [", sDataBaseName2.c_str());
    size_t iWroteTotal = fwrite(const_cast<char *>(sLine.data()), 1, sLine.size(), pFile);
    int iTIndex = 0;
    int iTSize = rows.size();
    for (map<string, map<string, string> >::const_iterator itSection = rows.begin();
         itSection != rows.end(); ++itSection)
    {
        ++iTIndex;
        const string &sTbName = itSection->first;
        const map<string, string> &sEntrys = itSection->second;
        sLine = formatJson("{", pLevel, iFormat);
        sLine += formatJson(CxString::format("\"tbname\": \"%s\",", sTbName.c_str()), pLevel, iFormat);
        iWroteTotal += fwrite(const_cast<char *>(sLine.data()), 1, sLine.size(), pFile);
        sLine = formatJson("\"rows\" : [", pLevel, iFormat);
        iWroteTotal += fwrite(const_cast<char *>(sLine.data()), 1, sLine.size(), pFile);
        sLine = formatJson("{", pLevel, iFormat);
        size_t iEntryCount = sEntrys.size();
        for (map<string, string>::const_iterator itEntry = sEntrys.begin(); itEntry != sEntrys.end(); ++itEntry)
        {
            --iEntryCount;
            if (iEntryCount > 0)
                sLine += formatJson("\"" + itEntry->first + "\": \"" + itEntry->second + "\",", pLevel, iFormat);
            else
                sLine += formatJson("\"" + itEntry->first + "\": \"" + itEntry->second + "\"", pLevel, iFormat);
        }
        sLine += formatJson("}", pLevel, iFormat);
        size_t iWrote = fwrite(const_cast<char *>(sLine.data()), 1, sLine.size(), pFile);
        if (iWrote != sLine.size())
        {
            fclose(pFile);
            return -1;
        }
        iWroteTotal += iWrote;
        if (iTSize == iTIndex)
        {
            sLine = formatJson("]", pLevel, iFormat);
            sLine += formatJson("}", pLevel, iFormat);
        }
        else
        {
            sLine = formatJson("]", pLevel, iFormat);
            sLine += formatJson("},", pLevel, iFormat);
        }
        iWroteTotal += fwrite(const_cast<char *>(sLine.data()), 1, sLine.size(), pFile);
    }
    sLine = formatJson("]", pLevel, iFormat);
    sLine += formatJson("}", pLevel, iFormat);
    iWroteTotal += fwrite(const_cast<char *>(sLine.data()), 1, sLine.size(), pFile);
    fclose(pFile);
    return iWroteTotal;
}

int
CxJson::saveTable(const std::string &sFilePath, std::map<std::string, std::vector<std::map<std::string, std::string> > > &tables, const string &sDataBaseName, int iFormat)
{
    FILE *pFile;
    pFile = fopen(sFilePath.data(), "wb");
    if (pFile == NULL)
    {
        return -1;
    }
    string sDataBaseName2 = sDataBaseName.empty() ? string("root") : sDataBaseName;
    size_t iLevel = 0;
    size_t *pLevel = &iLevel;
    string sLine = formatJson("{", pLevel, iFormat);
    sLine += formatJson(CxString::format("\"dbname\": \"%s\",", sDataBaseName2.c_str()), pLevel, iFormat);
    sLine += formatJson("\"tables\": [", pLevel, iFormat);
//    string sLine = CxString::format("{\"dbname\": \"%s\",\"tables\": [", sDataBaseName2.c_str());
    size_t iWroteTotal = fwrite(const_cast<char *>(sLine.data()), 1, sLine.size(), pFile);
    int iTIndex = 0;
    int iTSize = tables.size();

    for (std::map<std::string, std::vector<std::map<std::string, std::string> > >::const_iterator itTable = tables.begin();
         itTable != tables.end(); ++itTable)
    {
        ++iTIndex;
        const string &sTbName = itTable->first;
        const std::vector<std::map<std::string, std::string> > &table = itTable->second;
        for (int i = 0; i < table.size(); ++i)
        {
            const std::map<std::string, std::string> &sEntrys = table.at(i);
            sLine = formatJson("{", pLevel, iFormat);
            sLine += formatJson(CxString::format("\"tbname\": \"%s\",", sTbName.c_str()), pLevel, iFormat);
            iWroteTotal += fwrite(const_cast<char *>(sLine.data()), 1, sLine.size(), pFile);
            sLine = formatJson("\"rows\" : [", pLevel, iFormat);
            iWroteTotal += fwrite(const_cast<char *>(sLine.data()), 1, sLine.size(), pFile);
            sLine = formatJson("{", pLevel, iFormat);
            size_t iEntryCount = sEntrys.size();
            for (map<string, string>::const_iterator itEntry = sEntrys.begin(); itEntry != sEntrys.end(); ++itEntry)
            {
                --iEntryCount;
                if (iEntryCount > 0)
                    sLine += formatJson("\"" + itEntry->first + "\": \"" + itEntry->second + "\",", pLevel, iFormat);
                else
                    sLine += formatJson("\"" + itEntry->first + "\": \"" + itEntry->second + "\"", pLevel, iFormat);
            }
            sLine += formatJson("}", pLevel, iFormat);
            size_t iWrote = fwrite(const_cast<char *>(sLine.data()), 1, sLine.size(), pFile);
            if (iWrote != sLine.size())
            {
                fclose(pFile);
                return -1;
            }
            iWroteTotal += iWrote;
            if (iTSize == iTIndex)
            {
                sLine = formatJson("]", pLevel, iFormat);
                sLine += formatJson("}", pLevel, iFormat);
            }
            else
            {
                sLine = formatJson("]", pLevel, iFormat);
                sLine += formatJson("},", pLevel, iFormat);
            }
            iWroteTotal += fwrite(const_cast<char *>(sLine.data()), 1, sLine.size(), pFile);
        }
    }
    sLine = formatJson("]", pLevel, iFormat);
    sLine += formatJson("}", pLevel, iFormat);
    iWroteTotal += fwrite(const_cast<char *>(sLine.data()), 1, sLine.size(), pFile);
    fclose(pFile);
    return iWroteTotal;
}

std::string CxJson::formatJson(const std::string &sSource, size_t *pLevel, int iFormat)
{
    if (sSource.size() <= 0) return sSource;
    string r = sSource;
    if (iFormat > 0)
    {
        if ((*pLevel) < 1024 * 1024 * 100)
        {
            r = string((*pLevel) * 4, ' ') + sSource + "\r\n";
        }
    }
    char cEnd = sSource[sSource.size() - 1];
    switch (cEnd)
    {
        case '{':
        case '[':
            ++(*pLevel);
            break;

        case ',':
            //
            break;

            //} ] other
        default:
            --(*pLevel);
    }
    return r;
}
