#include "cxprofile_skv_ini.h"

#include "cxfile.h"

//CxSkverIniFactory * CxSkverIniFactory::s_instance = CxSkverIniFactory::createSingleton();


using namespace std;


int fn_deal_file_stream(void * oObject, int, int iLength, const void * pData)
{
    if (! oObject) return 0;
    CxSkverIni * oSkverIni = (CxSkverIni *)oObject;
    return oSkverIni->dealLoad(iLength, (char *)pData);
}

CxSkverIni::CxSkverIni() :
    CxSkverBase(),
    _dealState(0),
    _dealType(0)
{
    _text = _textData;
}

CxSkverIni::CxSkverIni( const std::string & sFilePath ) :
    CxSkverBase(),
    _dealState(0),
    _dealType(0)
{
    _text = _textData;
    load(sFilePath);
}

CxSkverIni::~CxSkverIni()
{
}

int CxSkverIni::dealLoad(int iLength, const char * pData)
{
    /*
    if ( i <= 0) return 0;
    const char * pBegin = pData;
    const char * pEnd = pData + iLength;
    const char * p;
    const char * pRead;
    for (; pBegin < pEnd; ++pBegin)
    {
        switch (_dealType)
        {
        case 0:

            break;
        case 1:
            char * getSection();
        case 2:
            char * getEntry();
        case 3:
            char * getValue();
            break;
        case 4:
             pBegin getRubbish();
            break;
        case 10:
            if (*pBegin == _dealEnd)
            {
                _text
                _textEnd =
                pRend =
            }
            break;
        case 0:

            break;
        case 0:

            break;
        default:
            break;
        }
    }
    memcpy();
*/
    for (int i = 0; i < iLength; ++i)
    {
        switch ( _dealState )
        {
        case 0:
        {
            if ( pData [ i ] == '[' )
            {
                _dealState = 1;
                _dealType = 1;
                _end = ']';
            }
        }
            break;
        case 1:
        {
            if ( pData [ i ] == ']' && _text > _textData )
            {
                _currentSection = string(_textData, _text);
                _text = _textData;
                _dealState = 2;
            }
            else if ( pData [ i ] > 0x20 && _text - _textData < sizeof(_textData))
            {
                * _text = pData [ i ];
                ++ _text;
                _dealState = 3;
            }
            else if ( pData [ i ] == '\n' || pData [ i ] == '\r')
            {
                _text = _textData;
                _dealState = 0;
            }
        }
            break;
        case 2:
        {
            if ( pData [ i ] == '=' && _text > _textData )
            {
                _currentEntry = string(_textData, _text);
                _text = _textData;
                _dealState = 3;
            }
            else if ( pData [ i ] > 0x20 && _text - _textData < sizeof(_textData))
             {
                * _text = pData [ i ];
                ++ _text;
            }
            else if ( pData [ i ] == '\n' || pData [ i ] == '\r')
            {
                _text = _textData;
            }
            else if ( pData [ i ] == '[' )
            {
                _text = _textData;
                _dealState = 1;
            }
            else if ( pData [ i ] == '#')
            {
                _dealState = 4;
            }
        }
            break;
        case 3:
        {
            if ( pData [ i ] < 0x20 && _text > _textData )
            {
                _currentValue = string(_textData, _text);
                setValue(_currentSection, _currentEntry, _currentValue);
                _text = _textData;
                _dealState = 2;
            }
            else
            {
                if (_text - _textData < sizeof(_textData))
                {
                    * _text = pData [ i ];
                    ++ _text;
                }
                else
                {
                    _text = _textData;
                    _dealState = 2;
                }
            }
        }
            break;
        case 4:
        {
            if ( pData [ i ] == '\n' || pData [ i ] == '\r')
            {
                _text = _textData;
                _dealState = 2;
            }
        }
            break;
        }
            // end case
    }
    return 0;
}

bool CxSkverIni::loadImpl(std::map<std::string, std::map<std::string, std::string> > & sectionEntryValues, const std::string& sFilePath)
{
    vector<string> sLines;
    if (CxFile::load(sFilePath, sLines, '\n'))
    {
        sectionEntryValues.clear();
//        std::pair<std::map<std::string, std::map<std::string, std::string> >::iterator,bool> ret;

        map<string, string> entrys;
        for (size_t i = 0; i < sLines.size(); ++i)
        {
            string sLine =  CxString::trim(sLines.at(i));
            switch (sLine[0])
            {
            case '[':
            {
                if (entrys.size() > 0 && _currentSection.size() > 0)
                {
                    sectionEntryValues[_currentSection] = entrys;
                }
                _currentSection = CxString::unquote(sLine, '[', ']');
                entrys = map<string, string>();
            }
                break;
            case '#':
            case ';':
            {
            }
                break;
            default:
            {
                bool bHas;
                _currentValue = CxString::tokenLeft(sLine, '=', &bHas);
                if (! bHas)
                {
                    break;
                }
                _currentValue = CxString::trim(_currentValue);
                _currentEntry = CxString::trim(sLine);
                if (_currentSection.size() > 0 && _currentEntry.size() > 0)
                {
                    entrys[_currentEntry] = _currentValue;
                }
            }
                break;
            }
        }
        if (entrys.size() > 0 && _currentSection.size() > 0)
        {
            sectionEntryValues[_currentSection] = entrys;
        }
        return true;
    }
    return false;
//    oSectionEntryValues = & sectionEntryValues;
//    return CxFile::load(sFilePath, this, fn_deal_file_stream);
}

bool CxSkverIni::saveImpl(const std::map<std::string, std::map<std::string, std::string> > & sectionEntryValues, const std::string& sFilePath, bool bRenew) const
{
    FILE * pFile;
    pFile = fopen (sFilePath.data(), "wb");
    size_t iWroteTotal = 0;
    if (pFile==NULL)
    {
        return false;
    }
    vector<string> sSectionNames = sectionNames(1);
    for (size_t i = 0; i < sSectionNames.size(); ++i)
    {
        const string & sSectionName = sSectionNames.at(i);
        map<string, map<string, string> >::const_iterator it = sectionEntryValues.find(sSectionName);
        if (it != sectionEntryValues.end())
        {
            const map<string, string> & sEntrys = it->second;
            string sLine("\r\n[");
            sLine += sSectionName;
            sLine += "]\r\n";
            iWroteTotal += fwrite (const_cast<char *>(sLine.data()) , 1, sLine.size(), pFile);
            for (map<string, string>::const_iterator itEntry = sEntrys.begin(); itEntry != sEntrys.end(); ++itEntry)
            {
                sLine = itEntry->first + "=" + itEntry->second + "\r\n";
                size_t iWrote = fwrite (const_cast<char *>(sLine.data()) , 1, sLine.size(), pFile);
                if (iWrote != sLine.size())
                    return false;
                iWroteTotal += iWrote;
            }
        }
    }

//    for (map<string, map<string, string> >::const_iterator itSection = sectionEntryValues.begin(); itSection != sectionEntryValues.end(); ++itSection)
//    {
//        string sLine("\r\n[");
//        sLine += itSection->first;
//        sLine += "]\r\n";
//        iWroteTotal += fwrite (const_cast<char *>(sLine.data()) , 1, sLine.size(), pFile);
//        const map<string, string> & sEntrys = itSection->second;
//        for (map<string, string>::const_iterator itEntry = sEntrys.begin(); itEntry != sEntrys.end(); ++itEntry)
//        {
//            sLine = itEntry->first + "=" + itEntry->second + "\r\n";
//            size_t iWrote = fwrite (const_cast<char *>(sLine.data()) , 1, sLine.size(), pFile);
//            if (iWrote != sLine.size())
//                return false;
//            iWroteTotal += iWrote;
//        }
//    }
    fclose (pFile);
    return iWroteTotal;
}

