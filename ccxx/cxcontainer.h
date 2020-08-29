#ifndef CXCONTAINER_H
#define CXCONTAINER_H


#include "cxglobal.h"
#include "cxstring.h"

#define GM_PLISTO_DELETEALL(oList, ClassName) \
    for (size_t i = 0; i < oList->size(); ++i) \
    { \
        ClassName* o = oList->at(i); \
        delete o; \
    } \
    oList->clear();



#define GM_PLISTO_FIND(oList, ClassName, field, value) \
    for (size_t i = 0; i < oList->size(); ++i) \
    { \
        ClassName* o = oList->at(i); \
        if ( o->field == value ) \
            return o; \
    } \
    return NULL;


#define GM_LISTO_DONE(list, ClassName, done) \
    for (size_t i = 0; i < list.size(); ++i) \
    { \
        ClassName * o = list.at(i); \
        o->done; \
    }



#define GM_LISTO_FIND(list, ClassName, field, value) \
    for (size_t i = 0; i < list.size(); ++i) \
    { \
        ClassName * o = list.at(i); \
        if ( o->field == value ) \
            return o; \
    } \
    return NULL;


class CxContainer
{
public:
    //vector - append
    template<typename _Tp>
    inline static void append(std::vector<_Tp> & vector1, const std::vector<_Tp> & vector2)
    {
        vector1.insert(vector1.end(), vector2.begin(), vector2.end());
    }

    //vector - update
    template<typename _Tp>
    inline static void update(std::vector<_Tp> & vector1, const _Tp & value1)
    {
        if (std::find(vector1.begin(), vector1.end(), value1) == vector1.end())
        {
            vector1.push_back(value1);
        }
    }

    template<typename _Tp>
    inline static bool update(std::vector<_Tp> & vector1, const _Tp & value1, const _Tp & value2)
    {
        typename std::vector<_Tp>::iterator it = std::find(vector1.begin(), vector1.end(), value1);
        if (it != vector1.end())
        {
            * it = value2;
            return true;
        }
        return false;
    }

    //vector - contain
    template<typename _Tp>
    inline static bool contain(std::vector<_Tp> & vector1, const _Tp & value1)
    {
        return std::find(vector1.begin(), vector1.end(), value1) != vector1.end();
    }

    template<typename _Tp>
    inline static bool contain(const std::vector<_Tp> & vector1, const _Tp & value1)
    {
        return std::find(vector1.begin(), vector1.end(), value1) != vector1.end();
    }

    template<typename _Tp>
    inline static bool contain(const std::vector<_Tp> * oVector1, const _Tp & value1)
    {
        return std::find(oVector1->begin(), oVector1->end(), value1) != oVector1->end();
    }

    template<typename _Tp>
    inline static bool contain(std::vector<_Tp> * oVector1, const _Tp & value1)
    {
        return std::find(oVector1->begin(), oVector1->end(), value1) != oVector1->end();
    }

    template<typename _Key, typename _Tp>
    inline static bool contain(const std::vector<std::pair<_Key, _Tp> > & vec, const _Key& key)
    {
        for (int i = 0; i < vec.size(); ++i)
        {
            if (vec[i]->first == key)
            {
                return true;
            }
        }
        return false;
    }

    template<typename _Key, typename _Tp>
    inline static typename std::vector<std::pair<_Key, _Tp> >::const_iterator contain(const std::vector<std::pair<_Key, _Tp> > & vec, const _Key& key)
    {
        for (int i = 0; i < vec.size(); ++i)
        {
            if (vec[i]->first == key)
            {
                return vec.begin() + i;
            }
        }
        return vec.end();
    }


    //vector - remove
    template<typename _Tp>
    inline static size_t remove(std::vector<_Tp> & vector1, const std::vector<_Tp> & vector2)
    {
        size_t r = vector1.size();
        for(typename std::vector<_Tp>::iterator it = vector1.begin(); it != vector1.end();)
        {
            if (std::find(vector2.begin(), vector2.end(), * it) != vector2.end())
            {
                it = vector1.erase(it);
            }
            else
            {
                ++it;
            }
        }
        return r - vector1.size();
    }

    template<typename _Tp>
    inline static size_t remove2(std::vector<_Tp> & vector1, const std::vector<_Tp> & vector2)
    {
        size_t r = vector1.size();
        std::vector<_Tp> rv(r);
        size_t k = 0;
        for(typename std::vector<_Tp>::iterator it = vector1.begin(); it != vector1.end();)
        {
            if (std::find(vector2.begin(), vector2.end(), * it) == vector2.end())
            {
                rv[k++] = * it;
            }
            ++it;
        }
        rv.resize(k);
        vector1 = rv;
        return r - k;
    }

    template<typename _Tp>
    inline static void remove(std::vector<_Tp> & vector1, const _Tp & value)
    {
        vector1.erase(std::remove(vector1.begin(), vector1.end(), value), vector1.end());
    }

    //vector - remove
    template<typename _Key, typename _Tp>
    inline static size_t remove(const std::vector<std::pair<_Key, _Tp> > & vec, const _Key& key)
    {
        size_t r = vec.size();
        for (int i = vec.size()-1; i >= 0; --i)
        {
            if (vec[i]->first == key)
            {
                vec.erase(vec.begin()+i);
            }
        }
        return r - vec.size();
    }

    template<typename _Key, typename _Tp>
    inline static size_t removeByValue(const std::vector<std::pair<_Key, _Tp> > & vec, const _Tp& value)
    {
        size_t r = vec.size();
        for (int i = vec.size()-1; i >= 0; --i)
        {
            if (vec[i]->second == value)
            {
                vec.erase(vec.begin()+i);
            }
        }
        return r - vec.size();
    }

    //vector - delete clear
    template<typename _Tp>
    inline static void deleteAndclear(std::vector<_Tp> & vector1)
    {
        for(typename std::vector<_Tp>::iterator it = vector1.begin(); it != vector1.end(); ++it)
        {
            _Tp d = * it;
            delete d;
        }
        vector1.clear();
    }

    //vector - pointer begin end
    template<typename _Tp>
    inline static const _Tp * begin_pointer( const std::vector<_Tp> & vector1 )
    {
        return (vector1.size() > 0) ? ( & vector1.front() ) : (NULL);
    }

    template<typename _Tp>
    inline static _Tp * begin_pointer( std::vector<_Tp> & vector1 )
    {
        return (vector1.size() > 0) ? ( & vector1.front() ) : (NULL);
    }

    template<typename _Tp>
    inline static const _Tp * end_pointer( const std::vector<_Tp> & vector1 )
    {
        return (vector1.size() > 0) ? ( & vector1.back() ) : (NULL);
    }

    template<typename _Tp>
    inline static _Tp * end_pointer( std::vector<_Tp> & vector1 )
    {
        return (vector1.size() > 0) ? ( & vector1.back() ) : (NULL);
    }

    //vector - index
    template<typename _Tp>
    inline static size_t index( const std::vector<_Tp> & vector1 , const _Tp & value1 )
    {
        typename std::vector<_Tp>::const_iterator it = std::find(vector1.begin(), vector1.end(), value1);
        if (it != vector1.end())
        {
            return it - vector1.begin();
        }
        return CxGlobal::npos;
    }

    //vector - merge
    template<typename _Tp>
    inline static std::vector<_Tp> merge( const std::vector<_Tp> & vector1 ,  const std::vector<_Tp> & vector2 )
    {
        std::vector<_Tp> r = vector1;
        vector1.insert(r.end(), vector2.begin(), vector2.end());
        return r;
    }

    //vector - next
    template<typename _Tp>
    inline static _Tp next( const std::vector<_Tp> & vector1 ,  const _Tp & value )
    {
        typename std::vector<_Tp>::const_iterator it = std::find(vector1.begin(), vector1.end(), value);
        if (it != vector1.end())
        {
            ++ it;
            if (it != vector1.end())
            {
                return (* it);
            }
        }
        return CxValueType::defaultValue<_Tp>();;
    }

    template<typename _Tp>
    inline static std::string joinToString(const std::vector<_Tp>& ss, char cJoin = CxGlobal::lineCharacter)
    {
        std::string r;
        if (ss.empty())
            return r;
        size_t iSize_1 = ss.size() - 1;
        for (size_t i = 0; i < iSize_1; ++i)
        {
            r.append(CxString::toString(ss.at(i)));
            r.push_back(cJoin);
        }
        if (ss.size()>0)
        {
            r.append(CxString::toString(ss.at(iSize_1)));
        }
        return r;
    }

    template<typename _Tp>
    inline static std::string joinToString(const std::vector<_Tp>& ss, const std::string & sJoin)
    {
        std::string r;
        if (ss.empty())
            return r;
        size_t iSize_1 = ss.size() - 1;
        for (size_t i = 0; i < iSize_1; ++i)
        {
            r.append(CxString::toString(ss.at(i)));
            r.append(sJoin);
        }
        if (ss.size()>0)
        {
            r.append(CxString::toString(ss.at(iSize_1)));
        }
        return r;
    }

    template<typename _Tp>
    inline static std::string joinToHexstring(const std::vector<_Tp>& ss, char cJoin = CxGlobal::lineCharacter)
    {
        std::string r;
        if (ss.empty())
            return r;
        size_t iSize_1 = ss.size() - 1;
        for (size_t i = 0; i < iSize_1; ++i)
        {
            r.append(CxString::toHexstring(ss.at(i)));
            r.push_back(cJoin);
        }
        if (ss.size()>0)
        {
            r.append(CxString::toHexstring(ss.at(iSize_1)));
        }
        return r;
    }

    template<typename _Tp>
    inline static std::string joinToHexstring(const std::vector<_Tp>& ss, const std::string & sJoin)
    {
        std::string r;
        if (ss.empty())
            return r;
        size_t iSize_1 = ss.size() - 1;
        for (size_t i = 0; i < iSize_1; ++i)
        {
            r.append(CxString::toHexstring(ss.at(i)));
            r.append(sJoin);
        }
        if (ss.size()>0)
        {
            r.append(CxString::toHexstring(ss.at(iSize_1)));
        }
        return r;
    }

    //vector - value
    template<typename _Tp, typename _NewTp>
    inline static std::vector<_NewTp> valuesTo(const std::vector<_Tp> & vec1, const _NewTp & defaultValue)
    {
        std::vector<_NewTp> r;
        for (size_t i = 0; i < vec1.size(); ++i)
        {
            r.push_back(CxValueType::valueTo(vec1.at(i), defaultValue));
        }
        return r;
    }

    template<typename _Tp>
    inline static void sort(std::vector<_Tp> & vec1, bool bOrder = true)
    {
        if (bOrder)
            std::sort(vec1.begin(), vec1.end(), CxContainer::compare_order<_Tp>);
        else
            std::sort(vec1.begin(), vec1.end(), CxContainer::compare_reverse<_Tp>);
    }

    template<typename _Tp>
    inline static int compare_order(const _Tp& v1, const _Tp& v2)
    {
        return v2 > v1;
    }

    template<typename _Tp>
    inline static int compare_reverse(const _Tp& v1, const _Tp& v2)
    {
        return v2 < v1;
    }

    template<typename _Key, typename _Tp>
    inline static typename std::vector<std::pair<_Key, _Tp> >::const_iterator find(const std::vector<std::pair<_Key, _Tp> > & vec, const _Key& key)
    {
        for (int i = 0; i < vec.size(); ++i)
        {
            if (vec[i]->first == key)
            {
                return vec.begin() + i;
            }
        }
        return vec.end();
    }

































    //map - keys
    template<typename _Key, typename _Tp>
    inline static std::vector<_Key> keys(const std::map<_Key, _Tp> & map1)
    {
        std::vector<_Key> r;
        for(typename std::map<_Key,_Tp>::const_iterator it = map1.begin(); it != map1.end(); ++it)
        {
            r.push_back(it->first);
        }
        return r;
    }

    template<typename _Key, typename _Tp>
    inline static std::vector<_Key> keys(const std::map<_Key, _Tp> & map1, const _Tp & value1)
    {
        std::vector<_Key> r;
        for(typename std::map<_Key,_Tp>::const_iterator it = map1.begin(); it != map1.end(); ++it)
        {
            if (it->second == value1)
                r.push_back(it->first);
        }
        return r;
    }

    //map - contain
    template<typename _Key, typename _Tp>
    inline static bool contain(std::map<_Key, _Tp> & map1, const _Key & key1)
    {
        typename std::map<_Key,_Tp>::const_iterator it = map1.find(key1);
        if (it != map1.end())
            return true;
        else
            return false;
    }

    template<typename _Key, typename _Tp>
    inline static bool contain(const std::map<_Key, _Tp> & map1, const _Key & key1)
    {
        typename std::map<_Key,_Tp>::const_iterator it = map1.find(key1);
        if (it != map1.end())
            return true;
        else
            return false;
    }

    template<typename _Key, typename _Tp>
    inline static bool contain(const std::map<_Key, _Tp> * map1, const _Key & key1)
    {
        typename std::map<_Key,_Tp>::const_iterator it = map1->find(key1);
        if (it != map1->end())
            return true;
        else
            return false;
    }

    template<typename _Key, typename _Tp>
    inline static bool containValue(const std::map<_Key, _Tp> * map1, const _Tp & value1)
    {
        for(typename std::map<_Key,_Tp>::const_iterator it = map1->begin(); it != map1->end(); ++it)
        {
            if (it->second == value1)
                return true;
        }
        return false;
    }

    template<typename _Key, typename _Tp>
    inline static bool containValue(const std::map<_Key, _Tp> & map1, const _Tp & value1)
    {
        for(typename std::map<_Key,_Tp>::const_iterator it = map1.begin(); it != map1.end(); ++it)
        {
            if (it->second == value1)
                return true;
        }
        return false;
    }

    //map - key
    template<typename _Key, typename _Tp>
    inline static const _Key& key(const std::map<_Key, _Tp> & map1, const _Tp& value1, const _Tp& defaultKey)
    {
        for(typename std::map<_Key,_Tp>::const_iterator it = map1.begin(); it != map1.end(); ++it)
        {
            if (it->second == value1)
                return it->first;
        }
        return defaultKey;
    }

    template<typename _Key, typename _Tp>
    inline static _Key key(const std::map<_Key, _Tp> & map1, const _Tp& value1)
    {
        for(typename std::map<_Key,_Tp>::const_iterator it = map1.begin(); it != map1.end(); ++it)
        {
            if (it->second == value1)
                return it->first;
        }
        return CxValueType::defaultValue<_Key>();
    }

    //map - value
    template<typename _Key, typename _Tp>
    inline static const _Tp& value(const std::map<_Key, _Tp> & map1, const _Key& key1, const _Tp& defaultValue)
    {
        typename std::map<_Key,_Tp>::const_iterator it = map1.find(key1);
        if (it != map1.end())
            return it->second;
        else
            return defaultValue;
    }  

    template<typename _Key, typename _Tp>
    inline static const _Tp& value(const std::map<_Key, _Tp> * map1, const _Key& key1, const _Tp& defaultValue)
    {
        if (map1)
        {
            typename std::map<_Key,_Tp>::const_iterator it = map1->find(key1);
            if (it != map1->end())
                return it->second;
            else
                return defaultValue;
        }
        else
        {
            return defaultValue;
        }
    }

    template<typename _Key, typename _Tp>
    inline static _Tp value(const std::map<_Key, _Tp> & map1, const _Key& key1)
    {
        typename std::map<_Key,_Tp>::const_iterator it = map1.find(key1);
        if (it != map1.end())
            return it->second;
        else
            return CxValueType::defaultValue<_Tp>();
    }

    template<typename _Key, typename _Tp>
    inline static _Tp value(const std::map<_Key, _Tp> * map1, const _Key& key1)
    {
        if (map1)
        {
            typename std::map<_Key,_Tp>::const_iterator it = map1->find(key1);
            if (it != map1->end())
                return it->second;
            else
                return CxValueType::defaultValue<_Tp>();
        }
        else
        {
            return CxValueType::defaultValue<_Tp>();
        }
    }

    //map - value
    template<typename _Key, typename _Tp, typename _NewTp>
    inline static std::map<_Key, _NewTp> valuesTo(const std::map<_Key, _Tp> & map1, const _NewTp & defaultValue)
    {
        std::map<_Key, _NewTp> r;
        for(typename std::map<_Key,_Tp>::const_iterator it = map1.begin(); it != map1.end(); ++it)
        {
            _NewTp value = CxValueType::valueTo(it->second, defaultValue);
            r[it->first] = value;
        }
        return r;
    }

    //map - value
    template<typename _Key, typename _Tp, typename _NewTp>
    inline static _NewTp valueTo(const std::map<_Key, _Tp> & map1, const _Key& key1, const _NewTp & defaultValue)
    {
        typename std::map<_Key,_Tp>::const_iterator it = map1.find(key1);
        if (it != map1.end())
        {
            return CxValueType::valueTo(it->second, defaultValue);
        }
        else
        {
            return defaultValue;
        }
    }

    //map - value
    template<typename _Tp>
    inline static _Tp valueCase(const std::map<std::string, _Tp> & map1, const std::string& key1) {
        for(typename std::map<std::string,_Tp>::const_iterator it = map1.begin(); it != map1.end(); ++it)
        {
            if (equalCase(it->first, key1))
                return it->second;
        }
        return CxValueType::defaultValue<_Tp>();
    }

    //map - value
    template<typename _Tp>
    inline static _Tp valueCase(const std::map<std::string, _Tp> & map1, const std::string& key1, const _Tp& defaultValue) {
        for(typename std::map<std::string,_Tp>::const_iterator it = map1.begin(); it != map1.end(); ++it)
        {
            if (CxString::equalCase(it->first, key1))
                return it->second;
        }
        return defaultValue;
    }

    //map - merge
    template<typename _Key, typename _Tp>
    inline static std::map<_Key, _Tp> merge(const std::map<_Key, _Tp> & map1, const std::map<_Key, _Tp> & map2)
    {
        std::map<_Key, _Tp> r = map1;
        r.insert(map2.begin(), map2.end());
        return r;
    }

    //map - update
    template<typename _Key, typename _Tp>
    inline static void update(std::map<_Key, _Tp> & map1, const std::map<_Key, _Tp> & map2)
    {
        for(typename std::map<_Key,_Tp>::const_iterator it = map2.begin(); it != map2.end(); ++it)
        {
            map1[it->first] = it->second;
        }
    }

    template<typename _Key, typename _Tp>
    inline static void remove(std::map<_Key, _Tp> & map1, const _Key& key1)
    {
        typename std::map<_Key,_Tp>::iterator it = map1.find(key1);
        if (it != map1.end()) map1.erase(it);
    }

    template<typename _Key, typename _Tp>
    inline static void remove(std::map<_Key, _Tp> & map1, const std::vector<_Key>& keys)
    {
        for (size_t i = 0; i < keys.size(); ++i)
        {
            const _Key & key1 = keys.at(i);
            typename std::map<_Key,_Tp>::iterator it = map1.find(key1);
            if (it != map1.end()) map1.erase(it);
        }
    }

    //joinToString
    template<typename _Key, typename _Tp>
    inline static std::string joinToString(const std::map<_Key, _Tp> &pairs, const std::string &sMid, const std::string &sJoin)
    {
        std::string r;
        for (typename std::map<_Key, _Tp>::const_iterator it = pairs.begin(); it != pairs.end(); ++it)
        {
            r.append(CxString::toString(it->first) + sMid + CxString::toString(it->second) + sJoin);
        }
        if (r.size()>0) r.resize(r.size()-sJoin.size());
        return r;
    }

    template<typename _Key, typename _Tp>
    inline static std::string joinToString(const std::map<_Key, std::vector<_Tp> > &pairs, const std::string &sMid, const std::string &sJoin1, const std::string &sJoin2)
    {
        std::string r;
        for (typename std::map<_Key, std::vector<_Tp> >::const_iterator it = pairs.begin(); it != pairs.end(); ++it)
        {
            r.append(CxString::toString(it->first) + sMid + CxContainer::joinToString(it->second, sJoin1) + sJoin2);
        }
        if (r.size()>0) r.resize(r.size()-sJoin2.size());
        return r;
    }

    //joinToString
    //iMode : 1 key to hex ; 2 value to hex ; 3 key value to hex
    template<typename _Key, typename _Tp>
    inline static std::string joinToHexstring(const std::map<_Key, _Tp> &pairs, const std::string &sMid, const std::string &sJoin, int iMode = 3)
    {
        if (iMode == 1)
        {
            std::string r;
            for (typename std::map<_Key, _Tp>::const_iterator it = pairs.begin(); it != pairs.end(); ++it)
            {
                r.append(CxString::toHexstring(it->first) + sMid + CxString::toString(it->second) + sJoin);
            }
            if (r.size()>0) r.resize(r.size()-sJoin.size());
            return r;
        }
        else if (iMode == 2)
        {
            std::string r;
            for (typename std::map<_Key, _Tp>::const_iterator it = pairs.begin(); it != pairs.end(); ++it)
            {
                r.append(CxString::toString(it->first) + sMid + CxString::toHexstring(it->second) + sJoin);
            }
            if (r.size()>0) r.resize(r.size()-sJoin.size());
            return r;
        }
        else
        {
            std::string r;
            for (typename std::map<_Key, _Tp>::const_iterator it = pairs.begin(); it != pairs.end(); ++it)
            {
                r.append(CxString::toHexstring(it->first) + sMid + CxString::toHexstring(it->second) + sJoin);
            }
            if (r.size()>0) r.resize(r.size()-sJoin.size());
            return r;
        }
    }

































    template<typename _Tp>
    inline static void clear(std::queue<_Tp>& ss)
    {
        while (!ss.empty())
        {
            ss.pop();
        }
    }

































    template<typename _Tp>
    inline static void clear(std::stack<_Tp>& ss)
    {
        while (!ss.empty())
        {
            ss.pop();
        }
    }






};

#endif // CXCONTAINER_H





/***
 *
//init map
const std::map<int, int>::value_type init_value[] =
{
    std::map<int, int>::value_type( 1, 1),
    std::map<int, int>::value_type( 2, 2),
};

const static std::map<int, int> m_mapPn(init_value, init_value+sizeof(init_value)/sizeof(init_value[0]));

*/

