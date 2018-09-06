int /*__cdecl*/ sub_main(int argc, char **argv, char **envp)//236F70
	void sub_5B8D0_initialize()//23c8d0
	void sub_46830_main_loop(signed __int16 *a1, signed int a2, unsigned __int16 a3)//227830
		signed int /*__fastcall*/ sub_76930(int a2, signed __int16 *a3)//257930
			int sub_76A40()//257A40
				char /*__cdecl*/ sub_779E0(int a1)//2589E0


x_DWORD_17DED4 -[34eed4]->48c73d mozna je to path struct
5e3a45000000
realzny puvod 48c80f
nastavuje se zde:0x25b110

struktura:
[0-32]-adresa
[1-16]-lenght


eax=1a6f44
eax=[eax] ...0

6f005d3f3a13
eax=[ebp-1c]...453a5e
edx=[ebp+14]..48c73d
add [edx],eax     [edx]=000 + 453a5e
eax=[ebp+14]..1a6f44
[ebp+14]+=6

5d3f3a13423a
eax=[ebp-1c]...453a5e
edx=[ebp+14]..48c743
add [edx],eax     [edx]=002 + 453a60
eax=[ebp+14]..1a6f44
[ebp+14]+=6

5d3f3a1342
eax=[ebp-1c]...453a5e
edx=[ebp+14]..48c749
add [edx],eax     [edx]=c22200000 + 455d45
eax=[ebp+14]..1a6f44
[ebp+14]+=6

zameny
x_DWORD_17DED4 za x_DWORD_17DED4_spritestr
x_DWORD_17DEC0 za x_DWORD_17DEC0_spritestr
		