#ifndef CXEFILE_H
#define CXEFILE_H


#include "cxxml.h"
#include "cxstring.h"
#include "cxcontainer.h"
#include "cxfile.h"
#include "cxdatabase.h"


class GM_CCXX_CORE_API EFileElement
{
public:
    enum ee_type_enum {ee_type_none=0, ee_type_row, ee_type_column_s, ee_type_column_m};

public:
   EFileElement();
   ~EFileElement();

   EFileElement& operator=(const EFileElement &o);

   void clear();

   std::vector<std::string> toEfileBuffer();

   int fromEfileBuffer(const char * pData, int iLength);
public:
   void setName(std::string className,std::string InstanceName)
   {
       _className = className;
       _InstanceName = InstanceName;
   }
   void getName(std::string &className,std::string &InstanceName)
   {
       className = _className;
       InstanceName = _InstanceName;
   }

   void seField(std::vector<std::string> value)
   {
       _field = value;
   }
   void getField(std::vector<std::string> &value)
   {
       value = _field;
   }
   void setUnit(std::vector<std::string> value)
   {
       _unit = value;
   }
   void getUint(std::vector<std::string> &value)
   {
       value = _unit;
   }
   void setRec(const std::vector<std::vector<std::string> > & value)
   {
       _rec = value;
   }
   void getRec(std::vector<std::vector<std::string> > &value)
   {
       value = _rec;
   }
   void addRec(std::vector<std::string> value)
   {
       _rec.push_back(value);
   }

   std::vector<std::map<std::string,std::string> > getRec();

   void setRec(std::vector<std::map<std::string,std::string> > &vRec);
   void setRec2(std::vector<std::map<std::string,std::string> > &vRec);

   int saveTable(std::vector<int> & columnTypes);

   void setType(int type){_type =type;}
   int getType(){return _type;}

   int loadTable(const std::string &sql);
   int loadTable2(const std::string &sql);
private:
   std::string _className;
   std::string _InstanceName;
   std::vector<std::string> _field;
   std::vector<std::string> _unit;
   std::vector<std::vector<std::string> > _rec;
   int _type;
};



class GM_CCXX_CORE_API ExplainEfile
{
public:
    ExplainEfile();

    static bool check(const std::string &sFile,const std::string &sErr);

    static int explain(const char * pData, int iLength, std::map<std::string, std::string> & heads, std::map<std::string, std::string> & majors, std::vector<std::map<std::string, std::string> > & details);

    static int explain(const char * pData, int iLength, std::map<std::string, std::string> & declare, std::vector<EFileElement> & vObj);

    static int explain(const std::vector<std::string> &data, std::map<std::string, std::string> & declare, std::vector<EFileElement> & vObj);

    static int explain(std::string &data, std::map<std::string, std::string> & declare, std::vector<EFileElement> & vObj);

    static std::vector<char> toEfileBuffer(std::map<std::string, std::string> & heads, std::map<std::string, std::string> & majors, std::vector<std::map<std::string, std::string> > & details);

    static std::vector<std::string> toEfileBuffer(std::map<std::string, std::string> & declare, std::vector<EFileElement> & vObj);

    static int loadFromFile(const std::string &sFile,std::map<std::string, std::string> & declare, std::vector<EFileElement> & vObj);

    static int saveToFile(const std::string &sFile,std::map<std::string, std::string> & declare, std::vector<EFileElement> & vObj);

    static void clear();

    static void setFieldTemplate(std::map<std::string,std::vector<std::string> > &map1);

    static bool createDeclare(std::map<std::string, std::string> & declare);

    static bool createElement_sql(const std::string &sClassName,const std::string &sInstanceName,const std::string &sql,const std::string &sFieldList);

    static bool createElement_sql(const std::string &sClassName,const std::string &sInstanceName,const std::string &sql);

    static bool createElement_row(const std::string &sClassName, const std::string &sInstanceName, const std::string &row);

    static bool createElement_rows(const std::string &sClassName, const std::string &sInstanceName, std::vector<std::string> &rows);

    static int toFile(const std::string &sFile);

    static std::string toString();

    static int loadFromString(const std::string &sBuffer,std::map<std::string, std::string> & declare, std::vector<EFileElement> & vObj);

};

class GM_CCXX_CORE_API CxEfile
{
public:
    static std::vector<std::string> toEfileBuffer(const std::string & sClassName, const std::string & sInstanceName, const std::vector<std::string> & sFields, const std::vector<std::vector<std::string> > & sRec);

};

#endif // CXEFILE_H
