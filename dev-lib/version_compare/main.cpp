
#include "version_compare.hpp"
#include <stdlib.h>
#include <stdio.h>


/*
example:

[test@tmscs version_compare]$ ./a.out 1.0.0.0 1 2.0.0.0 1  2.0.0.0
2.0.0.0 in [ 1.0.0.0,2.0.0.0 ]  rt=2
[test@tmscs version_compare]$ ./a.out 1.0.0.0 1 2.0.0.0 0  2.0.0.0
2.0.0.0 not in [ 1.0.0.0,2.0.0.0 )  rt=-2


[test@192.168.17.153 17:01:35 ~/.git_src/version_compare]$./a.out "[1.0, ]" 1.0
1.0 in [ 1.0,2100000000 ]  rt=1


*/

int main(int argc, char * argv[])
{
    std::string targetToCompare ="1.0.3.1";
    std::string set ="(1.0.1.1, 2.0.0.1]";
    
    
    if(argc<3)
    {
        printf("Usage:\n  %s <set> <targetToCompare> \n",
            argv[0]);
        return 0;
    }    
    set =argv[1];
    targetToCompare =argv[2];
    
    std::string delim=".";
    
    vercmp::VersionCompare::VersionSet vs;
    int rt =0;
    if ((rt=vercmp::VersionCompare::ParseSet(set, vs))<0)
    {
        printf("error! set=%s , rt=%d \n", set.c_str(), rt);
        return 0;
    }
    
    rt =vercmp::VersionCompare::IsInSet(targetToCompare, vs , delim);    
    if(rt>0)
        printf("%s in %c %s,%s %c  rt=%d\n", 
        targetToCompare.c_str(), 
        vs.includeLeft?'[':'(',vs.left.c_str(), 
        vs.right.c_str() , vs.includeRight?']':')' ,
        rt);
        
    else if(rt<0)
        printf("%s not in %c %s,%s %c  rt=%d\n", 
        targetToCompare.c_str(), 
        vs.includeLeft?'[':'(',vs.left.c_str(), 
        vs.right.c_str() , vs.includeRight?']':')' ,
        rt);
        
    else
        printf("error! \n");

    return 0;
}
