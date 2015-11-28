#ifndef CXEFILE_H
#define CXEFILE_H

#include "cxxml.h"
#include "cxstring.h"
#include "cxcontainer.h"
#include "cxfile.h"
#include "cxdatabase.h"

//E文件数据元素
class EFileElement
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
//debug
//       for(size_t i=0;i<value.size();i++)
//            cxPrompt()<<value.at(i);
   }
   //获取记录

   std::vector<std::map<std::string,std::string> > getRec();

   void setRec(std::vector<std::map<std::string,std::string> > &vRec);
   void setRec2(std::vector<std::map<std::string,std::string> > &vRec);

   int saveTable(std::vector<int> & columnTypes);

   void setType(int type){_type =type;}
   int getType(){return _type;}

   int loadTable(const std::string &sql);
   int loadTable2(const std::string &sql);
private:
   std::string _className; //类名,模板名
   std::string _InstanceName;//实例名,表名
   std::vector<std::string> _field;  //字段名
   std::vector<std::string> _unit;  //量纲
   std::vector<std::vector<std::string> > _rec; //记录
   int _type; //类型
};

//E文件解析
class ExplainEfile
{
public:
    ExplainEfile();
    //文件检查
    static bool check(const std::string &sFile,const std::string &sErr);
    //解析E文件
    static int explain(const char * pData, int iLength, std::map<std::string, std::string> & heads, std::map<std::string, std::string> & majors, std::vector<std::map<std::string, std::string> > & details);
    //解析E文件
    static int explain(const char * pData, int iLength, std::map<std::string, std::string> & declare, std::vector<EFileElement> & vObj);
    //解析E文件
    static int explain(const std::vector<std::string> &data, std::map<std::string, std::string> & declare, std::vector<EFileElement> & vObj);
    //解析E文件
    static int explain(std::string &data, std::map<std::string, std::string> & declare, std::vector<EFileElement> & vObj);

    //生成E文件
    static std::vector<char> toEfileBuffer(std::map<std::string, std::string> & heads, std::map<std::string, std::string> & majors, std::vector<std::map<std::string, std::string> > & details);
    //生成E文件
    static std::vector<std::string> toEfileBuffer(std::map<std::string, std::string> & declare, std::vector<EFileElement> & vObj);
    //加载文件
    static int loadFromFile(const std::string &sFile,std::map<std::string, std::string> & declare, std::vector<EFileElement> & vObj);
    //存文件
    static int saveToFile(const std::string &sFile,std::map<std::string, std::string> & declare, std::vector<EFileElement> & vObj);
// 清除
    static void clear();
//
    static void setFieldTemplate(std::map<std::string,std::vector<std::string> > &map1);
//创建声明
    static bool createDeclare(std::map<std::string, std::string> & declare);
//创建单元
    static bool createElement_sql(const std::string &sClassName,const std::string &sInstanceName,const std::string &sql,const std::string &sFieldList);
//创建单元
    static bool createElement_sql(const std::string &sClassName,const std::string &sInstanceName,const std::string &sql);
//创建单元
    static bool createElement_row(const std::string &sClassName, const std::string &sInstanceName, const std::string &row);
//创建单元
    static bool createElement_rows(const std::string &sClassName, const std::string &sInstanceName, std::vector<std::string> &rows);


//存文件
    static int toFile(const std::string &sFile);
//到缓冲区
    static std::string toString();
//加载缓冲区
    static int loadFromString(const std::string &sBuffer,std::map<std::string, std::string> & declare, std::vector<EFileElement> & vObj);


};

class CxEfile
{
public:
    static std::vector<std::string> toEfileBuffer(const std::string & sClassName, const std::string & sInstanceName, const std::vector<std::string> & sFields, const std::vector<std::vector<std::string> > & sRec);

};

#endif // CXEFILE_H
