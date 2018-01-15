#include "cxprofile_skv_xml.h"


#include "cxxml.h"


using namespace std;


CxSkverXml::CxSkverXml() :
    CxSkverBase(),
    _dealState(0),
    _dealType(0)
{
    _text = _textData;
}

CxSkverXml::CxSkverXml(const std::map<std::string, std::map<std::string, std::string> > & sectionEntryValues) :
    CxSkverBase(sectionEntryValues),
    _dealState(0),
    _dealType(0)
{
    _text = _textData;
}

CxSkverXml::CxSkverXml( const std::string & sFilePath ) :
    CxSkverBase(),
    _dealState(0),
    _dealType(0)
{
    _text = _textData;
    load(sFilePath);
}

CxSkverXml::~CxSkverXml()
{
}

bool CxSkverXml::loadImpl(std::map<string, std::map<string, string> > &sectionEntryValues, const string &sFilePath)
{
    if (skvType().empty())
    {
        return loadXmlNormal(sectionEntryValues, sFilePath);
    }
    else if (skvType() == "xml_4level")
    {
        map<string, string> sDataParams = CxString::splitToMap(skvParam(), '=', ',');
        string sTableName = CxContainer::value(sDataParams, string("TableName"));
        std::vector<std::map<string, string> > rows;
        CxXml::loadTable4Level(sFilePath, rows, "", sTableName);
        for (size_t i = 0; i < rows.size(); ++i)
        {
            sectionEntryValues[CxString::toString(i)] = rows.at(i);
        }
        return sectionEntryValues.size();
    }
    return false;
}

bool CxSkverXml::saveImpl(const std::map<string, std::map<string, string> > &sectionEntryValues, const string &sFilePath, bool bRenew) const
{
    if (skvType().empty())
    {
        return saveXmlNormal(sectionEntryValues, sFilePath, bRenew);
    }
    else if (skvType() == "xml_4level")
    {
        map<string, string> sDataParams = CxString::splitToMap(skvParam(), '=', ',');
        string sTableName = CxContainer::value(sDataParams, string("TableName"));
        return CxXml::saveTable4Level(sFilePath, CxString::sortToLines(sectionEntryValues), "", sTableName);
    }
    return false;
}

bool CxSkverXml::loadXmlNormal(std::map<std::string, std::map<std::string, std::string> > & sectionEntryValues, const std::string& sFilePath)
{
    TiXmlDocument doc;

    if(doc.LoadFile(sFilePath))
    {
        TiXmlHandle docHandle( &doc );
        TiXmlNode* nodeLevel2 = docHandle.FirstChild("root").FirstChild("config").Node();
        if(nodeLevel2)
        {
            sectionEntryValues.clear();
            int iRowIndex = 0;
            for( TiXmlElement * oElementLevel3 = nodeLevel2->FirstChildElement(); oElementLevel3; oElementLevel3 = oElementLevel3->NextSiblingElement() )
            {
                if (! oElementLevel3) break;
                map<string, string> entryValues;
                const TiXmlAttribute * oAttribute = oElementLevel3->FirstAttribute();
                while (oAttribute)
                {
                    string sKey = oAttribute->NameTStr();
                    string sValue = oAttribute->ValueStr();
                    entryValues[sKey] = sValue;
                    oAttribute = oAttribute->Next();
                }
                iRowIndex++;
                string sSection = CxString::format("%06d",iRowIndex);
                sectionEntryValues[sSection] = entryValues;
            }
            return true;
        }
    }
    return false;
}

bool CxSkverXml::saveXmlNormal(const std::map<std::string, std::map<std::string, std::string> > & sectionEntryValues, const std::string& sFilePath, bool bRenew) const
{
    FILE * pFile;
    pFile = fopen (sFilePath.data(), "wb");
    size_t iWroteTotal = 0;
    if (pFile==NULL)
    {
        return false;
    }
    string sLine = "<?xml version=\"1.0\" encoding=\"utf-8\"?>\r\n<root>\r\n    <config>\r\n";
    iWroteTotal += fwrite (const_cast<char *>(sLine.data()) , 1, sLine.size(), pFile);



    vector<string> sSectionNames = sectionNames(1);
    for (size_t i = 0; i < sSectionNames.size(); ++i)
    {
        const string & sSectionName = sSectionNames.at(i);
        map<string, map<string, string> >::const_iterator it = sectionEntryValues.find(sSectionName);
        if (it != sectionEntryValues.end())
        {
            const map<string, string> & sEntrys = it->second;
            sLine = ("        <prop ");
    //        sLine = ("        < ");
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
    }

//    for (map<string, map<string, string> >::const_iterator itSection = sectionEntryValues.begin(); itSection != sectionEntryValues.end(); ++itSection)
//    {
//        const map<string, string> & sEntrys = itSection->second;
//        sLine = ("        <prop ");
////        sLine = ("        < ");
//        for (map<string, string>::const_iterator itEntry = sEntrys.begin(); itEntry != sEntrys.end(); ++itEntry)
//        {
//            sLine += itEntry->first + "=\"" + itEntry->second + "\" ";
//        }
//        sLine += "/>\r\n";
//        size_t iWrote = fwrite (const_cast<char *>(sLine.data()) , 1, sLine.size(), pFile);
//        if (iWrote != sLine.size())
//            return false;
//        iWroteTotal += iWrote;
//    }
    sLine = "    </config>\r\n</root>\r\n";
    iWroteTotal += fwrite (const_cast<char *>(sLine.data()) , 1, sLine.size(), pFile);
    fclose (pFile);
    return iWroteTotal;
}
