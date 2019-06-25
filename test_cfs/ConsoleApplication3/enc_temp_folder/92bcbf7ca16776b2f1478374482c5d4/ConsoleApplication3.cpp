// ConsoleApplication3.cpp : Tento soubor obsahuje funkci main. Provádění programu se tam zahajuje a ukončuje.
//

#include <iostream>

int calcuAsm2(int var1)
{
	__asm
	{
		mov edx, var1
		mov eax,edx
		sar     edx, 1Fh//fffff
		shl     edx, 8//ffff00
		sbb		eax,edx//feced3ff
		sar		eax,8
		//mov eax, edx
	};
}

int my_sign(int var)
{
	if (var < 0)return -1;
	return 0;
}

int main()
{
	int v2; // edx
	v2 = 0;

	//(v1 - (__CFSHL__(v1 >> 31, 8) + (v1 >> 31 << 8))) >> 8)
	for (int v2 = -20; v2 < 1000000000000; v2++)
	{
		int cftemp = 0;
		if ((v2 >> 30) & 1)
			cftemp = 1;
		//Bit8u vara=a1x->byte_0x3E_62;
		//int varb=(64 - (((v2 - ((v2 >> 31) << 2)) - cftemp) >> 2));
		int test1 = (v2-(my_sign(v2)*255/*+ (v2 >> 31 << 8)*/)>>8);// -(__CFSHL__(v1 >> 31, 8) + (v1 >> 31 << 8))) >> 8);
		int test2=calcuAsm2(v2);
		printf("%d %d\n",test1,test2);
	}    
}

// Spuštění programu: Ctrl+F5 nebo nabídka Ladit > Spustit bez ladění
// Ladění programu: F5 nebo nabídka Ladit > Spustit ladění

// Tipy pro zahájení práce:
//   1. K přidání nebo správě souborů použijte okno Průzkumník řešení.
//   2. Pro připojení ke správě zdrojového kódu použijte okno Team Explorer.
//   3. K zobrazení výstupu sestavení a dalších zpráv použijte okno Výstup.
//   4. K zobrazení chyb použijte okno Seznam chyb.
//   5. Pokud chcete vytvořit nové soubory kódu, přejděte na Projekt > Přidat novou položku. Pokud chcete přidat do projektu existující soubory kódu, přejděte na Projekt > Přidat existující položku.
//   6. Pokud budete chtít v budoucnu znovu otevřít tento projekt, přejděte na Soubor > Otevřít > Projekt a vyberte příslušný soubor .sln.
