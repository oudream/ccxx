#include "cxprofile_skv_txt.h"

#include "cxfile.h"

//CxSkverTxtFactory * CxSkverTxtFactory::s_instance = CxSkverTxtFactory::createSingleton();

using namespace std;


CxSkverTxt::CxSkverTxt() :
    CxSkverBase()
{
}

CxSkverTxt::CxSkverTxt(const string &sFilePath) :
    CxSkverBase()
{
    load(sFilePath);
}

CxSkverTxt::~CxSkverTxt()
{
}

bool CxSkverTxt::loadImpl(std::map<std::string, std::map<std::string, std::string> > & sectionEntryValues, const std::string& sFilePath)
{
    vector<string> sLines;
    if (CxFile::load(sFilePath, sLines, '\n'))
    {
        sectionEntryValues.clear();
        map<string, string> entrys;
        const char * pchEnd;
        char * pchIndex;
        char * pchFirst;
        size_t iRemain;
        for (size_t i = 0; i < sLines.size(); ++i)
        {
            const string& sLine = sLines.at(i);
            //sLine.end - 1;
            pchEnd = sLine.data() + sLine.size() - 1;
            pchIndex = (char*)memchr(sLine.data(), ',', sLine.size());
            if (pchIndex)
            {
                if (entrys.size() > 0 && _currentSection.size() > 0)
                {
                    sectionEntryValues[_currentSection] = entrys;
                }
                _currentSection = string(sLine.data(), pchIndex-sLine.data());
                pchFirst = pchIndex + 1;
                iRemain = pchEnd - pchIndex;
            }
            while (pchIndex != NULL)
            {
                pchIndex = (char*)memchr(pchIndex, '=', iRemain);
                if (pchIndex)
                {
                    _currentEntry = string(pchFirst, pchIndex-pchFirst);
                    pchFirst = pchIndex + 1;
                    iRemain = pchEnd - pchIndex;
                }
                else
                {
                    break;
                }
                pchIndex = (char*)memchr(pchIndex, ',', iRemain);
                if (pchIndex)
                {
                    _currentValue = string(pchFirst, pchIndex-pchFirst);
                    pchFirst = pchIndex + 1;
                    iRemain = pchEnd - pchIndex;
                }
                else
                {
                    _currentValue = string(pchFirst, pchEnd-pchFirst+1);
                }
                if (_currentSection.size() > 0 && _currentEntry.size() > 0)
                {
                    entrys[_currentEntry] = _currentValue;
                }
            }
        }
        if (entrys.size() > 0 && _currentSection.size() > 0)
        {
            sectionEntryValues[_currentSection] = entrys;
        }
        return true;
    }
    return false;
}

bool CxSkverTxt::saveImpl(const std::map<std::string, std::map<std::string, std::string> > & sectionEntryValues, const std::string &sFilePath, bool bRenew) const
{
    vector<string> sLines;
    for (map<string, map<string, string> >::const_iterator itSection = sectionEntryValues.begin(); itSection != sectionEntryValues.end(); ++itSection)
    {
        string sLine = CxString::newString(itSection->first);
        sLine.push_back(',');
        const map<string, string> & sEntrys = itSection->second;
        for (map<string, string>::const_iterator itEntry = sEntrys.begin(); itEntry != sEntrys.end(); ++itEntry)
        {
            sLine += itEntry->first + "=" + itEntry->second + ',';
        }
        sLines.push_back(sLine);
    }
    std::sort(sLines.begin(), sLines.end());
    return CxFile::save(sFilePath, sLines);
}


