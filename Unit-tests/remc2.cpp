//#include "stdafx.h"

#include <boost/bind/bind.hpp>
#include <boost/asio.hpp>

#include "engine/CommandLineParser.h"
#include "engine/engine_support.h"
#include "sub_main.h"

using namespace std;

#ifdef _MSC_VER
int main(int argc, char** argv)
#else
int main(int argc, char** argv)
#endif
//int main()
{
	
	//char *argv[] = { "netherw.exe","-level","2", NULL };
	//int argc = (sizeof(argv) / sizeof(argv[0])) - 1;
	char *envp[] = { NULL };
	//char *envp[] = { "env=xx", NULL };
	//ds:esi - cesta k nazvu
	//mine_texts((char*)"0160-0022A288", 0x2a1000, 1000, (char*)"mined-texts.txt");

	CommandLineParams.Init(argc, argv);

	support_begin();
	int retval = sub_main(argc, argv, envp);
	support_end();
	//saveactstate();
    return 0;
}

