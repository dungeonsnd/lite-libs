
#include <map>
#include <tr1/memory>


int main(int argc,char * argv[])
{	
	typedef std::tr1::shared_ptr < int * > T_sp;
	std::map <int,T_sp> m;
	m.erase(1);
	return 0;
}

