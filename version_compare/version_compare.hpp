#ifndef _HEADER_FILE_VERSION_COMPARE_HPP_
#define _HEADER_FILE_VERSION_COMPARE_HPP_

#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <vector>

namespace vercmp
{

template < typename Type >
inline void strsplit(const Type & s, const Type & delim,std::vector< Type > & ret)
{
    size_t last = 0;
    size_t index=s.find_first_of(delim,last);
    while (index!=Type::npos)
    {
        ret.push_back(s.substr(last,index-last));
        last=index+1;
        index=s.find_first_of(delim,last);
    }
    if (index-last>0)
    {
        ret.push_back(s.substr(last,index-last));
    }
}

typedef std::string T;
inline T strtrim(T & str,T delim, T::size_type pos = 0)
{
    pos = str.find_first_of(delim, pos);
    if (pos == T::npos)
        return str;
    return strtrim(str.erase(pos, delim.size()),delim);
}

class VersionCompare
{
public:
    typedef std::string T;
    typedef std::vector < T > T_VSTR;
    typedef std::vector < int > T_VINT;

    struct VersionSet
    {
        T left;
        int includeLeft;
        T right;
        int includeRight;
    };
    
    // setString is like of "(1.0.1.1, 2.0.0.1]", return 1 ok, <0 failed.
    static int ParseSet(const T & setString, VersionCompare::VersionSet & setParseResult)
    {
        T ss =setString;
        ss =strtrim(ss," ");
        
        std::vector < std::string > ele;
        std::string delim=",";
        strsplit(ss, delim, ele);
        
        
        if(ele.size()<2)
            return -1;
        if(ele[0].size()<2)
        {
            if(ele[1].size()<2)
                return -2;
            else  // (,2.0.0.1]
            {
                ele[0] +="0";
            }
        }
        if(ele[1].size()<2)
        {
            if(ele[0].size()<2)
                return -3;
            else // (1.0.1.1,]
            {
                ele[1] ="2100000000"+ele[1];
            }
        }
        
        
        if(ele[0][0]=='(')
            setParseResult.includeLeft =0;
        else if(ele[0][0]=='[')
            setParseResult.includeLeft =1;
        else
            return -4;
        ele[0].erase(0,1);
        setParseResult.left =ele[0];
        
        size_t lastIdx =ele[1].size()-1;
        if(ele[1][lastIdx]==')')
            setParseResult.includeRight =0;
        else if(ele[1][lastIdx]==']')
            setParseResult.includeRight =1;
        else
            return -5;
        ele[1].erase(lastIdx,1);
        setParseResult.right =ele[1];
        
        return 1;
    }
    
    // > 0 in, < 0 not in, ==0 error.
    static int IsInSet(const T & targetToCompare, const VersionSet & versionSet , const T & delim)
    {
        int rt =Compare(targetToCompare, versionSet.left, delim);
        if(rt<0) return -1; // in the left of the set.
        else if(rt==0)
        {
            if(versionSet.includeLeft) return 1; // left edge.
            else return -1;
        }
        else
        {
            rt =Compare(targetToCompare, versionSet.right, delim);
            if(rt>0) return -2; // in the right of the set.
            else if(rt==0)
            {
                if (versionSet.includeRight) return 2; // right edge.
                else return -2;
            }
            else return 3; // in the middle.
        }
    }
    
    // return value like strcmp.
    static int Compare(const T & left, const T & right, const T & delim)
    {
        std::vector < std::string > lstr;
        std::vector < std::string > rstr;
        strsplit(left, delim, lstr);
        strsplit(right, delim, rstr);
        
        T_VINT lint,rint;
        ConvertVectorString2Int(lstr, lint);
        ConvertVectorString2Int(rstr, rint);
        
        T_VINT::const_iterator it1 =lint.begin();
        T_VINT::const_iterator it2 =rint.begin();
        for(;it1!=lint.end();it1++)
        {
            if(*it1>*it2) return 1; // left>right.
            else if(*it1<*it2) return -1; // left<right.
            else // equal, continue next.
            {
                if(it2!=rint.end())
                {
                    it2++;
                    continue;
                }
                else return 1; // left size > right size.
            }
        }
        if(it2!=rint.end()) return -1; // left size < right size.
        return 0; // left==right.
    }
private:

    static void ConvertVectorString2Int(const T_VSTR & vstr, T_VINT & vint)
    {
        T_VSTR::const_iterator it =vstr.begin();
        for(T_VSTR::const_iterator it =vstr.begin();it!=vstr.end();it++)
        {
            int i =atoi(it->c_str());
            vint.push_back(i);
        }
    }
};

} // namespace vercmp

#endif // _HEADER_FILE_VERSION_COMPARE_HPP_

