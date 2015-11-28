
#include <ccxx.h>

using namespace std;

int main(int argc, char * argv[])
{
    cout << "begin test xml : " << endl;

    CxApplication::init(argc, argv);

    string sFilePath = CxFileSystem::mergeFilePath(CxApplication::applicationPath(), "cics");
    sFilePath = CxFileSystem::mergeFilePath(sFilePath, "template_00304.xml");
    if (! CxFileSystem::isExist(sFilePath))
        return 0;

    string sReceived;
    CxFile::load(sFilePath, sReceived);

    TiXmlDocument doc;
    doc.Parse(sReceived.c_str());
//    doc.LoadFile(sFilePath);

    TiXmlNode * root = doc.FirstChild();
    if (root->Type() == TiXmlNode::TINYXML_DECLARATION)
        root = root->NextSibling();
    cout << string(0, ' ') << root->Value() << endl;
    for( TiXmlElement * node1Level = root->FirstChildElement(); node1Level; node1Level = node1Level->NextSiblingElement() )
    {
        cout << string(4, ' ') << node1Level->Value() << endl;
        for( TiXmlElement * node2Level = node1Level->FirstChildElement(); node2Level; node2Level = node2Level->NextSiblingElement() )
        {
            string sValue;
            for( TiXmlNode * node3Level = node2Level->FirstChild(); node3Level; node3Level = node3Level->NextSibling() )
            {
                if (node3Level->Type() == TiXmlNode::TINYXML_TEXT)
                {
                    sValue = node3Level->ToText()->Value();
                    break;
                }
            }
            cout << string(8, ' ') << node2Level->Value() << "=" << sValue << endl;
        }
    }

    return 0;
}



