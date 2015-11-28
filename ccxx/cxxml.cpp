#include "cxxml.h"

#include "cxstring.h"

using namespace std;


void CxXml::loadTable1LevelByDocument(const TiXmlDocument &doc, std::vector<std::map<string, string> > &rows, const string &sTableName)
{
    const TiXmlElement * oElementLevel1 = doc.FirstChildElement();
    while (oElementLevel1)
    {
        //*table name
        if (sTableName.size()>0)
        {
            if (oElementLevel1->ValueTStr() != sTableName)
            {
                continue;
            }
        }
        for( const TiXmlElement * oElementLevel2 = oElementLevel1->FirstChildElement(); oElementLevel2; oElementLevel2 = oElementLevel2->NextSiblingElement() )
        {
            if (! oElementLevel2) break;
            map<string, string> row;
            loadRowByElement(oElementLevel2, row);
            if (row.size()>0)
            {
                rows.push_back(row);
            }
        }
        oElementLevel1 = oElementLevel1->NextSiblingElement();
    }
}

void CxXml::loadTable1Level(const string &sFilePath, std::vector<std::map<string, string> > &rows, const string &sTableName)
{
    TiXmlDocument doc;
    if(doc.LoadFile(sFilePath))
    {
        loadTable1LevelByDocument(doc, rows, sTableName);
    }
}

void CxXml::loadTable1Level(const char *pData, int iLength, std::vector<std::map<string, string> > &rows, const string &sTableName)
{
    if (pData && iLength > 5)
    {
        TiXmlDocument doc;
        if(doc.Parse(pData))
        {
            loadTable1LevelByDocument(doc, rows, sTableName);
        }
    }
}

void CxXml::loadTable2LevelByDocument(const TiXmlDocument &doc, std::vector<std::map<string, string> > &rows, const string &sDataBaseName, const string &sTableName)
{
    const TiXmlElement * oElementLevel1 = doc.FirstChildElement();
    while (oElementLevel1)
    {
        //*database name
        if (sDataBaseName.size()>0)
        {
            if (oElementLevel1->ValueTStr() != sDataBaseName)
            {
                continue;
            }
        }
        for( const TiXmlElement * oElementLevel2 = oElementLevel1->FirstChildElement(); oElementLevel2; oElementLevel2 = oElementLevel2->NextSiblingElement() )
        {
            if (! oElementLevel2) break;
            //*tablename
            if (sTableName.size()>0)
            {
                if (oElementLevel2->ValueTStr() != sTableName)
                {
                    continue;
                }
            }
            for( const TiXmlElement * oElementLevel3 = oElementLevel2->FirstChildElement(); oElementLevel3; oElementLevel3 = oElementLevel3->NextSiblingElement() )
            {
                if (! oElementLevel3) break;
                map<string, string> row;
                loadRowByElement(oElementLevel3, row);
                if (row.size()>0)
                {
                    rows.push_back(row);
                }
            }
        }
        oElementLevel1 = oElementLevel1->NextSiblingElement();
    }
}

void CxXml::loadTable2Level(const std::string &sFilePath, std::vector<std::map<string, string> > &rows, const std::string &sDataBaseName, const std::string &sTableName)
{
    TiXmlDocument doc;
    if(doc.LoadFile(sFilePath))
    {
        loadTable2LevelByDocument(doc, rows, sDataBaseName, sTableName);
    }
}

void CxXml::loadTable2Level(const char *pData, int iLength, std::vector<std::map<string, string> > &rows, const string &sDataBaseName, const string &sTableName)
{
    if (pData && iLength > 5)
    {
        TiXmlDocument doc;
        if(doc.Parse(pData))
        {
            loadTable2LevelByDocument(doc, rows, sDataBaseName, sTableName);
        }
    }
}

void CxXml::loadTable3LevelByDocument(const TiXmlDocument &doc, std::vector<std::map<string, string> > &rows, const string &sDataBaseName, const string &sTableName, const string &sLevel1Key)
{
    const TiXmlElement * oElementLevel1 = doc.FirstChildElement();
    while (oElementLevel1)
    {
        //*database name
        if (sDataBaseName.size()>0)
        {
            if (oElementLevel1->ValueTStr() != sDataBaseName)
            {
                continue;
            }
        }
        for( const TiXmlElement * oElementLevel2 = oElementLevel1->FirstChildElement(); oElementLevel2; oElementLevel2 = oElementLevel2->NextSiblingElement() )
        {
            if (! oElementLevel2) break;
            //*tablename
            if (sTableName.size()>0)
            {
                if (oElementLevel2->ValueTStr() != sTableName)
                {
                    continue;
                }
            }
            for( const TiXmlElement * oElementLevel3 = oElementLevel2->FirstChildElement(); oElementLevel3; oElementLevel3 = oElementLevel3->NextSiblingElement() )
            {
                if (! oElementLevel3) break;
                string sValue = oElementLevel3->ValueTStr();
                for( const TiXmlElement * oElementLevel4 = oElementLevel3->FirstChildElement(); oElementLevel4; oElementLevel4 = oElementLevel4->NextSiblingElement() )
                {
                    if (! oElementLevel4) break;
                    map<string, string> row;

                    row[sLevel1Key] = sValue;
                    loadRowByAttribute(oElementLevel3, row);

                    loadRowByElement(oElementLevel4, row);
                    if (row.size()>0)
                    {
                        rows.push_back(row);
                    }
                }
            }
        }
        oElementLevel1 = oElementLevel1->NextSiblingElement();
    }
}

void CxXml::loadTable3Level(const string &sFilePath, std::vector<std::map<string, string> > &rows, const string &sDataBaseName, const string &sTableName, const string &sLevel1Key)
{
    TiXmlDocument doc;
    if(doc.LoadFile(sFilePath))
    {
        loadTable3LevelByDocument(doc, rows, sDataBaseName, sTableName, sLevel1Key);
    }
}

void CxXml::loadTable3Level(const char *pData, int iLength, std::vector<std::map<string, string> > &rows, const string &sDataBaseName, const string &sTableName, const string &sLevel1Key)
{
    if (pData && iLength > 5)
    {
        TiXmlDocument doc;
        if(doc.Parse(pData))
        {
            loadTable3LevelByDocument(doc, rows, sDataBaseName, sTableName, sLevel1Key);
        }
    }
}

void CxXml::loadTable4LevelByDocument(const TiXmlDocument &doc, std::vector<std::map<string, string> > &rows, const string &sDataBaseName, const string &sTableName, const string &sLevel1Key, const string &sLevel2Key)
{
    const TiXmlElement * oElementLevel1 = doc.FirstChildElement();
    while (oElementLevel1)
    {
        //*database name
        if (sDataBaseName.size()>0)
        {
            if (oElementLevel1->ValueTStr() != sDataBaseName)
            {
                continue;
            }
        }
        for( const TiXmlElement * oElementLevel2 = oElementLevel1->FirstChildElement(); oElementLevel2; oElementLevel2 = oElementLevel2->NextSiblingElement() )
        {
            if (! oElementLevel2) break;
            //*tablename
            if (sTableName.size()>0)
            {
                if (oElementLevel2->ValueTStr() != sTableName)
                {
                    continue;
                }
            }
            for( const TiXmlElement * oElementLevel3 = oElementLevel2->FirstChildElement(); oElementLevel3; oElementLevel3 = oElementLevel3->NextSiblingElement() )
            {
                if (! oElementLevel3) break;
                string sValue1 = oElementLevel3->ValueTStr();
                for( const TiXmlElement * oElementLevel4 = oElementLevel3->FirstChildElement(); oElementLevel4; oElementLevel4 = oElementLevel4->NextSiblingElement() )
                {
                    if (! oElementLevel4) break;
                    string sValue2 = oElementLevel4->ValueTStr();
                    for( const TiXmlElement * oElementLevel5 = oElementLevel4->FirstChildElement(); oElementLevel5; oElementLevel5 = oElementLevel5->NextSiblingElement() )
                    {
                        if (! oElementLevel5) break;
                        map<string, string> row;

                        row[sLevel1Key] = sValue1;
                        loadRowByAttribute(oElementLevel3, row);

                        row[sLevel2Key] = sValue2;
                        loadRowByAttribute(oElementLevel4, row);

                        loadRowByElement(oElementLevel5, row);
                        if (row.size()>0)
                        {
                            rows.push_back(row);
                        }
                    }
                }
            }
        }
        oElementLevel1 = oElementLevel1->NextSiblingElement();
    }
}

void CxXml::loadTable4Level(const string &sFilePath, std::vector<std::map<string, string> > &rows, const string &sDataBaseName, const string &sTableName, const string &sLevel1Key, const string &sLevel2Key)
{
    TiXmlDocument doc;
    if(doc.LoadFile(sFilePath))
    {
        loadTable4LevelByDocument(doc, rows, sDataBaseName, sTableName, sLevel1Key, sLevel2Key);
    }
}

void CxXml::loadTable4Level(const char *pData, int iLength, std::vector<std::map<string, string> > &rows, const string &sDataBaseName, const string &sTableName, const string &sLevel1Key, const string &sLevel2Key)
{
    if (pData && iLength > 5)
    {
        TiXmlDocument doc;
        if(doc.Parse(pData))
        {
            loadTable4LevelByDocument(doc, rows, sDataBaseName, sTableName, sLevel1Key, sLevel2Key);
        }
    }
}

void CxXml::loadTable5LevelByDocument(const TiXmlDocument &doc, std::vector<std::map<string, string> > &rows, const string &sDataBaseName, const string &sTableName, const string &sLevel1Key, const string &sLevel2Key, const string &sLevel3Key)
{
    const TiXmlElement * oElementLevel1 = doc.FirstChildElement();
    while (oElementLevel1)
    {
        //*database name
        if (sDataBaseName.size()>0)
        {
            if (oElementLevel1->ValueTStr() != sDataBaseName)
            {
                continue;
            }
        }
        for( const TiXmlElement * oElementLevel2 = oElementLevel1->FirstChildElement(); oElementLevel2; oElementLevel2 = oElementLevel2->NextSiblingElement() )
        {
            if (! oElementLevel2) break;
            //*tablename
            if (sTableName.size()>0)
            {
                if (oElementLevel2->ValueTStr() != sTableName)
                {
                    continue;
                }
            }
            for( const TiXmlElement * oElementLevel3 = oElementLevel2->FirstChildElement(); oElementLevel3; oElementLevel3 = oElementLevel3->NextSiblingElement() )
            {
                if (! oElementLevel3) break;
                string sValue1 = oElementLevel3->ValueTStr();
                for( const TiXmlElement * oElementLevel4 = oElementLevel3->FirstChildElement(); oElementLevel4; oElementLevel4 = oElementLevel4->NextSiblingElement() )
                {
                    if (! oElementLevel4) break;
                    string sValue2 = oElementLevel4->ValueTStr();
                    for( const TiXmlElement * oElementLevel5 = oElementLevel4->FirstChildElement(); oElementLevel5; oElementLevel5 = oElementLevel5->NextSiblingElement() )
                    {
                        if (! oElementLevel5) break;
                        string sValue3 = oElementLevel5->ValueTStr();
                        for( const TiXmlElement * oElementLevel6 = oElementLevel5->FirstChildElement(); oElementLevel6; oElementLevel6 = oElementLevel6->NextSiblingElement() )
                        {
                            if (! oElementLevel6) break;
                            map<string, string> row;

                            row[sLevel1Key] = sValue1;
                            loadRowByAttribute(oElementLevel3, row);

                            row[sLevel2Key] = sValue2;
                            loadRowByAttribute(oElementLevel4, row);

                            row[sLevel3Key] = sValue3;
                            loadRowByAttribute(oElementLevel5, row);

                            loadRowByElement(oElementLevel6, row);
                            if (row.size()>0)
                            {
                                rows.push_back(row);
                            }
                        }
                    }
                }
            }
        }
        oElementLevel1 = oElementLevel1->NextSiblingElement();
    }
}

void CxXml::loadTable5Level(const string &sFilePath, std::vector<std::map<string, string> > &rows, const string &sDataBaseName, const string &sTableName, const string &sLevel1Key, const string &sLevel2Key, const string &sLevel3Key)
{
    TiXmlDocument doc;
    if(doc.LoadFile(sFilePath))
    {
        loadTable5LevelByDocument(doc, rows, sDataBaseName, sTableName, sLevel1Key, sLevel2Key, sLevel3Key);
    }
}

void CxXml::loadTable5Level(const char *pData, int iLength, std::vector<std::map<string, string> > &rows, const string &sDataBaseName, const string &sTableName, const string &sLevel1Key, const string &sLevel2Key, const string &sLevel3Key)
{
    if (pData && iLength > 5)
    {
        TiXmlDocument doc;
        if(doc.Parse(pData))
        {
            loadTable5LevelByDocument(doc, rows, sDataBaseName, sTableName, sLevel1Key, sLevel2Key, sLevel3Key);
        }
    }
}

//key=value node.name=text
void CxXml::loadRowByElement(const TiXmlElement *oElementLevel, std::map<std::string, std::string> & row)
{
    if (oElementLevel)
    {
        string sKey = oElementLevel->ValueTStr();
        string sValue = text(oElementLevel);
        row[sKey] = sValue;
        loadRowByAttribute(oElementLevel, row);
        for( const TiXmlElement * oElementLevelChild = oElementLevel->FirstChildElement(); oElementLevelChild; oElementLevelChild = oElementLevelChild->NextSiblingElement() )
        {
            if (! oElementLevelChild) break;
            loadRowByElement(oElementLevelChild, row);
        }
    }
}

void CxXml::loadRowByAttribute(const TiXmlElement *oElementLevel, std::map<string, string> &row)
{
    if (oElementLevel)
    {
        const TiXmlAttribute * oAttribute = oElementLevel->FirstAttribute();
        while (oAttribute)
        {
            string sKey = oAttribute->NameTStr();
            string sValue = oAttribute->ValueStr();
            row[sKey] = sValue;
            oAttribute = oAttribute->Next();
        }
    }
}

bool CxXml::saveTable4Level(const string &sFilePath, const std::vector<std::map<string, string> > &rows, const string &sDataBaseName, const string &sTableName)
{
    string sLevel1Name = sDataBaseName;
    if (sLevel1Name.empty())
        sLevel1Name = "root";
    string sLevel2Name = sTableName;
    if (sLevel2Name.empty())
        sLevel2Name = "config";
    FILE * pFile;
    pFile = fopen (sFilePath.data(), "wb");
    size_t iWroteTotal = 0;
    if (pFile==NULL)
    {
        return false;
    }
    string sLine = CxString::format("<?xml version=\"1.0\" encoding=\"utf-8\"?>\r\n<%s>\r\n    <%s>\r\n        <c>\r\n            <d>\r\n", sLevel1Name.c_str(), sLevel2Name.c_str());
    iWroteTotal += fwrite (const_cast<char *>(sLine.data()) , 1, sLine.size(), pFile);
    for (size_t i = 0; i < rows.size(); ++i)
    {
        sLine = ("                <prop ");
        const map<string, string> & sEntrys = rows.at(i);
        for (map<string, string>::const_iterator itEntry = sEntrys.begin(); itEntry != sEntrys.end(); ++itEntry)
        {
            sLine += itEntry->first + "=\"" + itEntry->second + "\" ";
        }
        sLine += "/>\r\n";
        size_t iWrote = fwrite (const_cast<char *>(sLine.data()) , 1, sLine.size(), pFile);
        if (iWrote != sLine.size())
            return false;
        iWroteTotal += iWrote;
    }
    sLine = CxString::format("            </d>\r\n        </c>\r\n    </%s>\r\n</%s>\r\n", sLevel2Name.c_str(), sLevel1Name.c_str());;
    iWroteTotal += fwrite (const_cast<char *>(sLine.data()) , 1, sLine.size(), pFile);
    fclose (pFile);
    return iWroteTotal;
}
