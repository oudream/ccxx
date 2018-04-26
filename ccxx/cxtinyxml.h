#ifndef CXTINYXML_H
#define CXTINYXML_H

#include "cxglobal.h"

#include "tinyxml/tinyxml.h"

class GM_CCXX_CORE_API CxTinyXml
{
public:
    inline static std::string attribute(const TiXmlElement * element, const std::string & sName) {
        if (element)
        {
            const char * pch = element->Attribute( sName.c_str() );
            if (pch)
            {
                return pch;
            }
        }
        return std::string();
    }

    inline static std::string text(const TiXmlElement * element) {
        if (element)
        {
            const char * pch = element->GetText();
            if (pch)
            {
                return pch;
            }
        }
        return std::string();
    }
/*
<?xml version="1.0"?>
<CONFIG>
    <Prop cmd="0x0011" measure_id="0x00000000" id="3" name="task_1s" ext="run=1;period=1000;file=task_1s.lua"/>
    <Prop cmd="0x0011" measure_id="0x00000001" id="4" name="task_10s" ext="run=1;period=10000;file=task_10s.lua"/>
    <Prop cmd="0x0011" measure_id="0x00000002" id="5" name="task_30s" ext="run=1;period=30000;file=task_30s.lua"/>
    <Prop>
        <cmd>"0x0011"</cmd>
        <measure_id>"0x00000002"</measure_id>
        <id>"5"</id>
        <name>"task_30s"</name>
        <ext>"run=1;period=30000;file=task_30s.lua"</ext>
    </Prop>
</CONFIG>
*/
//<!-- CxTinyXml::loadTable(sFilePath, "YGCT", "CONFIG", rows); -->
    static void loadTable1LevelByDocument(const TiXmlDocument &doc, std::vector<std::map<std::string, std::string> > & rows, const std::string & sTableName = std::string());

    static void loadTable1Level(const std::string & sFilePath, std::vector<std::map<std::string, std::string> > & rows, const std::string & sTableName = std::string());

    static void loadTable1Level(const char * pData, int iLength, std::vector<std::map<std::string, std::string> > & rows, const std::string & sTableName = std::string());

/*
<?xml version="1.0"?>
<YGCT>
    <CONFIG>
        <Prop cmd="0x0011" measure_id="0x00000000" id="3" name="task_1s" ext="run=1;period=1000;file=task_1s.lua"/>
        <Prop cmd="0x0011" measure_id="0x00000001" id="4" name="task_10s" ext="run=1;period=10000;file=task_10s.lua"/>
        <Prop cmd="0x0011" measure_id="0x00000002" id="5" name="task_30s" ext="run=1;period=30000;file=task_30s.lua"/>
        <Prop>
            <cmd>"0x0011"</cmd>
            <measure_id>"0x00000002"</measure_id>
            <id>"5"</id>
            <name>"task_30s"</name>
            <ext>"run=1;period=30000;file=task_30s.lua"</ext>
        </Prop>
    </CONFIG>
</YGCT>
*/
//<!-- CxTinyXml::loadTable(sFilePath, "YGCT", "CONFIG", rows); -->
    static void loadTable2LevelByDocument(const TiXmlDocument & doc, std::vector<std::map<std::string, std::string> > & rows, const std::string & sDataBaseName = std::string(), const std::string & sTableName = std::string());

    static void loadTable2Level(const std::string & sFilePath, std::vector<std::map<std::string, std::string> > & rows, const std::string & sDataBaseName = std::string(), const std::string & sTableName = std::string());

    static void loadTable2Level(const char * pData, int iLength, std::vector<std::map<std::string, std::string> > & rows, const std::string & sDataBaseName = std::string(), const std::string & sTableName = std::string());

/*
<?xml version="1.0"?>
<YGCT>
    <CONFIG>
        <CMD_0x0011>
            <Prop measure_id="0x00000000" id="3" name="task_1s" ext="run=1;period=1000;file=task_1s.lua"/>
            <Prop measure_id="0x00000001" id="4" name="task_10s" ext="run=1;period=10000;file=task_10s.lua"/>
            <Prop measure_id="0x00000002" id="5" name="task_30s" ext="run=1;period=30000;file=task_30s.lua"/>
            <Prop>
                <measure_id>"0x00000002"</measure_id>
                <id>"5"</id>
                <name>"task_30s"</name>
                <ext>"run=1;period=30000;file=task_30s.lua"</ext>
            </Prop>
        </CMD_0x0011>
    </CONFIG>
</YGCT>
*/
//<!-- CxTinyXml::loadTable(sFilePath, "YGCT", "CONFIG", "cmd", rows); -->
    static void loadTable3LevelByDocument(const TiXmlDocument & doc, std::vector<std::map<std::string, std::string> > & rows, const std::string & sDataBaseName, const std::string & sTableName, const std::string & sLevel1Key);

    static void loadTable3Level(const std::string & sFilePath, std::vector<std::map<std::string, std::string> > & rows, const std::string & sDataBaseName = std::string(), const std::string & sTableName = std::string(), const std::string & sLevel1Key = std::string());

    static void loadTable3Level(const char * pData, int iLength, std::vector<std::map<std::string, std::string> > & rows, const std::string & sDataBaseName = std::string(), const std::string & sTableName = std::string(), const std::string & sLevel1Key = std::string());

/*
<?xml version="1.0"?>
<YGCT>
    <CONFIG>
        <CMD_0x0011>
            <MEAS_0x00000000>
                <Prop id="3" name="task_1s" ext="run=1;period=1000;file=task_1s.lua"/>
                <Prop id="4" name="task_10s" ext="run=1;period=10000;file=task_10s.lua"/>
                <Prop id="5" name="task_30s" ext="run=1;period=30000;file=task_30s.lua"/>
                <Prop>
                    <id>"6"</id>
                    <name>"task_40s"</name>
                    <ext>"run=1;period=40000;file=task_40s.lua"</ext>
                </Prop>
            </MEAS_0x00000000>
        </CMD_0x0011>
    </CONFIG>
</YGCT>
*/
//<!-- CxTinyXml::loadTable(sFilePath, "YGCT", "CONFIG", "cmd", "measure", rows); -->
    static void loadTable4LevelByDocument(const TiXmlDocument & doc, std::vector<std::map<std::string, std::string> > & rows, const std::string & sDataBaseName = std::string(), const std::string & sTableName = std::string(), const std::string & sLevel1Key = std::string(), const std::string & sLevel2Key = std::string());

    static void loadTable4Level(const std::string & sFilePath, std::vector<std::map<std::string, std::string> > & rows, const std::string & sDataBaseName = std::string(), const std::string & sTableName = std::string(), const std::string & sLevel1Key = std::string(), const std::string & sLevel2Key = std::string());

    static void loadTable4Level(const char * pData, int iLength, std::vector<std::map<std::string, std::string> > & rows, const std::string & sDataBaseName = std::string(), const std::string & sTableName = std::string(), const std::string & sLevel1Key = std::string(), const std::string & sLevel2Key = std::string());

    static void loadTable5LevelByDocument(const TiXmlDocument & doc, std::vector<std::map<std::string, std::string> > & rows, const std::string & sDataBaseName = std::string(), const std::string & sTableName = std::string(), const std::string & sLevel1Key = std::string(), const std::string & sLevel2Key = std::string(), const std::string & sLevel3Key = std::string());

    static void loadTable5Level(const std::string & sFilePath, std::vector<std::map<std::string, std::string> > & rows, const std::string & sDataBaseName = std::string(), const std::string & sTableName = std::string(), const std::string & sLevel1Key = std::string(), const std::string & sLevel2Key = std::string(), const std::string & sLevel3Key = std::string());

    static void loadTable5Level(const char * pData, int iLength, std::vector<std::map<std::string, std::string> > & rows, const std::string & sDataBaseName = std::string(), const std::string & sTableName = std::string(), const std::string & sLevel1Key = std::string(), const std::string & sLevel2Key = std::string(), const std::string & sLevel3Key = std::string());

    //find all key=value
    static void loadRowByElement(const TiXmlElement * oElementLevel, std::map<std::string, std::string> &row);

    //find all key=value
    static void loadRowByAttribute(const TiXmlElement * oElementLevel, std::map<std::string, std::string> &row);

    //*save
    static bool saveTable4Level(const std::string & sFilePath, const std::vector<std::map<std::string, std::string> > & rows, const std::string & sDataBaseName = std::string(), const std::string & sTableName = std::string()) ;

    //</row field1=value1 field2=value2 ...>
    static std::string toXmlBuffer1(const std::vector<std::string> & sFields, const std::vector<std::vector<std::string> > & sRows, const std::string & sLevel1Name = "db", const std::string & sLevel2Name = "table", const std::string & sLevel3Name = "row");

    //<row><field1>value1<\field1><field2>value2<\field2></row>
    static std::string toXmlBuffer2(const std::vector<std::string> & sFields, const std::vector<std::vector<std::string> > & sRows, const std::string & sLevel1Name = "db", const std::string & sLevel2Name = "table", const std::string & sLevel3Name = "row");

};

#endif // CXTINYXML_H
