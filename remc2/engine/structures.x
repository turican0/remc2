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
x_DWORD_17DED4 za xy_DWORD_17DED4_spritestr
x_DWORD_17DEC0 za xy_DWORD_17DEC0_spritestr
x_DWORD_17DEC8 za xy_DWORD_17DEC8_spritestr

x_WORD_E1F84 - upravit na strukturu
unk_E24BCx - upravit na strukturu

x_DWORD_17DE48c - nahrazeno heap?

x_DWORD_E9B20 =

x_DWORD_E9C2C == BLOCK32DAT_BEGIN_BUFFER tj. pstr[psxadatablock32dat].var28_begin_buffer

x_DWORD_F6ED0);//2c7ed0 == TMAPS00TAB_BEGIN_BUFFER

x_D41A0_BYTEARRAY_0
*(x_BYTE *)(x_D41A0_BYTEARRAY_0 + v9 + 0x365F5) |= 2u;
*(x_BYTE *)(x_D41A0_BYTEARRAY_0 + v10 + 0x365F5) |= 0x20u;
*(x_BYTE *)(x_D41A0_BYTEARRAY_0 + v11 + 0x365F5) |= 0x40u;

*(Bit16u*)(8 * v3+ &x_D41A0_BYTEARRAY_0[0x3647C]))
*(x_BYTE *)(x_D41A0_BYTEARRAY_0 + 8 * (signed __int16)result + 0x365F6) = *(x_BYTE *)(8 * (signed __int16)result + a1 + 0x65AD)
*(_WORD *)(&x_D41A0_BYTEARRAY_0[0x30313	+ 20 * *(_WORD *)(&x_D41A0_BYTEARRAY_0[0x3647c + 8 * v6])])

if (x_D41A0_BYTEARRAY_0[8 * *(unsigned __int8 *)(a1 + 72) + 0x365F6])

i - (&x_D41A0_BYTEARRAY_0[28302])) / 168;

struct_28222{//lenght 10
byte_28222
byte_28223
byte_28224_unk
byte_28225
byte_28226
byte_28227_unk
word_28228
word_28230
}

struct_11230{//lenght 2124
byte_11232 - setting1
12149-rot1?
byte_11286 - setting2 - resolution?
12244-pos1?
12568-rot2?
12573-rot3?
12593-pos2/rot4?
byte_12286

}

struct_222539{//lenght 10
byte_222541
}

struct_222620{//lenght 11
byte_222620
}

x_D41A0_BYTEARRAY_0
{
struct_11232[8]
struct_28222[8]

struct_222539[8]
struct_222620[8]
}

test:
void sub_12100(uint8_t* a1, uint8_t* a2, char a3)//1f3100		
void sub_11EE0(uint8_t* a1)//1f2ee0

void sub_17A00(uint8_t* a1, signed int a2, __int16 a3)//1f8a00

		v3 = x_DWORD_EA3E4[*(unsigned __int16 *)(2124 * D41A0_BYTESTR_0.word_0xc + x_D41A0_BYTEARRAY_0 + 11240)];
&x_D41A0_BYTEARRAY_0[0x6e8e + (168 * 0)]

*(x_WORD *)(a1 + 10) = (v2 - &x_D41A0_BYTEARRAY_0[28302]) / 168;
sub_5C950(v18, (uint8_t*)v113);



*(x_BYTE *)(v1 + 69) = -103; - type?
*(x_BYTE *)(v1 + 63) = 5; - type entity creature
*(x_BYTE *)(v1 + 64) = 19;
*(x_WORD *)(v1 + 132) = 76;
*(x_WORD *)(v1 + 134) = 8;
*(x_DWORD *)(v1 + 4) = 600;
a1+76 x?
a1+77 y?

<entity id = "107">
<prop id = "count">0</prop>
<prop id = "type">5</prop>
<prop id = "subtype">19</prop>
<prop id = "xpos">217</prop>
<prop id = "ypos">126</prop>
<prop id = "disid">6</prop>
<prop id = "swisz">0</prop>
<prop id = "swiid">6</prop>
<prop id = "parent">0</prop>
<prop id = "child">0</prop>
</entity>

<entity id = "133">
<prop id = "count">0</prop>
<prop id = "type">5</prop>
<prop id = "subtype">19</prop>
<prop id = "xpos">213</prop>
<prop id = "ypos">125</prop>
<prop id = "disid">6</prop>
<prop id = "swisz">0</prop>
<prop id = "swiid">6</prop>
<prop id = "parent">5</prop>
<prop id = "child">0</prop>
</entity>

<entity id = "134">
<prop id = "count">0</prop>
<prop id = "type">5</prop>
<prop id = "subtype">19</prop>
<prop id = "xpos">211</prop>
<prop id = "ypos">132</prop>
<prop id = "disid">6</prop>
<prop id = "swisz">0</prop>
<prop id = "swiid">6</prop>
<prop id = "parent">0</prop>
<prop id = "child">0</prop>
</entity>

<entity id = "188">
<prop id = "count">0</prop>
<prop id = "type">5</prop>
<prop id = "subtype">19</prop>
<prop id = "xpos">209</prop>
<prop id = "ypos">129</prop>
<prop id = "disid">6</prop>
<prop id = "swisz">0</prop>
<prop id = "swiid">6</prop>
<prop id = "parent">0</prop>
<prop id = "child">0</prop>
</entity>

<entity id = "192">
<prop id = "count">0</prop>
<prop id = "type">5</prop>
<prop id = "subtype">19</prop>
<prop id = "xpos">215</prop>
<prop id = "ypos">129</prop>
<prop id = "disid">6</prop>
<prop id = "swisz">0</prop>
<prop id = "swiid">6</prop>
<prop id = "parent">0</prop>
<prop id = "child">0</prop>
</entity>

<entity id = "305">
<prop id = "count">0</prop>
<prop id = "type">5</prop>
<prop id = "subtype">19</prop>
<prop id = "xpos">217</prop>
<prop id = "ypos">210</prop>
<prop id = "disid">33</prop>
<prop id = "swisz">0</prop>
<prop id = "swiid">33</prop>
<prop id = "parent">0</prop>
<prop id = "child">0</prop>
</entity>


case 1:sprintf_s(buffer, "Type:1 N/A ");i += 2;break;
		case 2:sprintf_s(buffer, "Type:Scenery ");
			fwrite(buffer, strlen(buffer), 1, fptw);
			i += 2;
			outsubtype = ((unsigned int)content[i] & 0xff + ((unsigned int)content[i + 1] & 0xff << 8));
			switch ((unsigned int)content[i] & 0xff + ((unsigned int)content[i + 1] & 0xff << 8)) {
			case 0:sprintf_s(buffer, "Subtype:Tree ");i += 2;break;
			case 1:sprintf_s(buffer, "Subtype:Standing Stone ");i += 2;break;
			case 2:sprintf_s(buffer, "Subtype:Dolmen ");i += 2;break;
			case 3:sprintf_s(buffer, "Subtype:Bad Stone ");i += 2;break;
			default:sprintf_s(buffer, "Subtype:Error%u ", ((unsigned int)content[i] & 0xff + ((unsigned int)content[i + 1] & 0xff << 8)));i += 2;break;
			}
			break;
		case 3:sprintf_s(buffer, "Type:Player Spawn ");
			fwrite(buffer, strlen(buffer), 1, fptw);
			i += 2;
			outsubtype = ((unsigned int)content[i] & 0xff + ((unsigned int)content[i + 1] & 0xff << 8));
			switch ((unsigned int)content[i] & 0xff + ((unsigned int)content[i + 1] & 0xff << 8)) {
			case 0:sprintf_s(buffer, "Subtype:0 ? ");i += 2;break;
			case 1:sprintf_s(buffer, "Subtype:1 ? ");i += 2;break;
			case 2:sprintf_s(buffer, "Subtype:2 ? ");i += 2;break;
			case 3:sprintf_s(buffer, "Subtype:3 ? ");i += 2;break;
			case 4:sprintf_s(buffer, "Subtype:Flyer1 ");i += 2;break;
			case 5:sprintf_s(buffer, "Subtype:Flyer2 ");i += 2;break;
			case 6:sprintf_s(buffer, "Subtype:Flyer3 ");i += 2;break;
			case 7:sprintf_s(buffer, "Subtype:Flyer4 ");i += 2;break;
			case 8:sprintf_s(buffer, "Subtype:Flyer5 ");i += 2;break;
			case 9:sprintf_s(buffer, "Subtype:Flyer6 ");i += 2;break;
			case 0xa:sprintf_s(buffer, "Subtype:Flyer7 ");i += 2;break;
			case 0xb:sprintf_s(buffer, "Subtype:Flyer8 ");i += 2;break;
			case 0xc:sprintf_s(buffer, "Subtype:12 ? ");i += 2;break;
			case 0xd:sprintf_s(buffer, "Subtype:13 ? ");i += 2;break;
			case 0xe:sprintf_s(buffer, "Subtype:14 ? ");i += 2;break;
			case 0xf:sprintf_s(buffer, "Subtype:15 ? ");i += 2;break;
			default:sprintf_s(buffer, "Subtype:Error%u ", ((unsigned int)content[i] & 0xff + ((unsigned int)content[i + 1] & 0xff << 8)));i += 2;break;
			}
			break;
		case 4:sprintf_s(buffer, "Type:N/A ");i += 2;break;
		case 5:sprintf_s(buffer, "Type:Creatures ");
			fwrite(buffer, strlen(buffer), 1, fptw);
			i += 2;
			outsubtype = ((unsigned int)content[i] & 0xff + ((unsigned int)content[i + 1] & 0xff << 8));
			switch ((unsigned int)content[i] & 0xff + ((unsigned int)content[i + 1] & 0xff << 8)) {
			case 0:sprintf_s(buffer, "Subtype:Dragon ");i += 2;break;
			case 1:sprintf_s(buffer, "Subtype:Vulture ");i += 2;break;// goat - kamzik
			case 2:sprintf_s(buffer, "Subtype:Bee ");i += 2;break;
			case 3:sprintf_s(buffer, "Subtype:Worm ");i += 2;break;
			case 4:sprintf_s(buffer, "Subtype:Archer ");i += 2;break;//ok
			case 5:sprintf_s(buffer, "Subtype:Crab ");i += 2;break;
			case 6:sprintf_s(buffer, "Subtype:Kraken ");i += 2;break;
			case 7:sprintf_s(buffer, "Subtype:Troll ");i += 2;break;
			case 8:sprintf_s(buffer, "Subtype:Griffon ");i += 2;break;
			case 9:sprintf_s(buffer, "Subtype:Skeleton ");i += 2;break;
			case 0xa:sprintf_s(buffer, "Subtype:Emu ");i += 2;break;
			case 0xb:sprintf_s(buffer, "Subtype:Genie ");i += 2;break;
			case 0xc:sprintf_s(buffer, "Subtype:Builder ");i += 2;break;
			case 0xd:sprintf_s(buffer, "Subtype:Townie ");i += 2;break;//ok
			case 0xe:sprintf_s(buffer, "Subtype:Trader ");i += 2;break;
			case 0xf:sprintf_s(buffer, "Subtype:? Ape? ");i += 2;break;
			case 0x10:sprintf_s(buffer, "Subtype:Wyvern ");i += 2;break;
			0x13//FireFly
			default:sprintf_s(buffer, "Subtype:Error%u ", ((unsigned int)content[i] & 0xff + ((unsigned int)content[i + 1] & 0xff << 8)));i += 2;break;
			}
			break;
		case 6:sprintf_s(buffer, "Type:N/A ");i += 2;break;
		case 7:sprintf_s(buffer, "Type:Weather ");
			fwrite(buffer, strlen(buffer), 1, fptw);
			i += 2;
			outsubtype = ((unsigned int)content[i] & 0xff + ((unsigned int)content[i + 1] & 0xff << 8));
			switch ((unsigned int)content[i] & 0xff + ((unsigned int)content[i + 1] & 0xff << 8)) {
			case 0:sprintf_s(buffer, "Subtype:0 N/A ");i += 2;break;
			case 1:sprintf_s(buffer, "Subtype:1 N/A ");i += 2;break;
			case 2:sprintf_s(buffer, "Subtype:2 N/A ");i += 2;break;
			case 3:sprintf_s(buffer, "Subtype:3 N/A ");i += 2;break;
			case 4:sprintf_s(buffer, "Subtype:Wind ");i += 2;break;
			default:sprintf_s(buffer, "Subtype:Error%u ", ((unsigned int)content[i] & 0xff + ((unsigned int)content[i + 1] & 0xff << 8)));i += 2;break;
			}
			break;
		case 8:sprintf_s(buffer, "Type:8 N/A ");i += 2;break;
		case 9:sprintf_s(buffer, "Type:9 N/A ");i += 2;break;
		case 0xa:sprintf_s(buffer, "Type:Effects ");
			fwrite(buffer, strlen(buffer), 1, fptw);
			i += 2;
			outsubtype = ((unsigned int)content[i] & 0xff + ((unsigned int)content[i + 1] & 0xff << 8));
			switch ((unsigned int)content[i] & 0xff + ((unsigned int)content[i + 1] & 0xff << 8)) {
			case 0:sprintf_s(buffer, "Subtype:0 ");i += 2;break;
			case 1:sprintf_s(buffer, "Subtype:Big explosion ");i += 2;break;
			case 2:sprintf_s(buffer, "Subtype:2 ");i += 2;break;
			case 3:sprintf_s(buffer, "Subtype:3 ");i += 2;break;
			case 4:sprintf_s(buffer, "Subtype:4 ");i += 2;break;
			case 5:sprintf_s(buffer, "Subtype:splash ");i += 2;break;
			case 6:sprintf_s(buffer, "Subtype:Fire ");i += 2;break;
			case 7:sprintf_s(buffer, "Subtype:7 ");i += 2;break;
			case 8:sprintf_s(buffer, "Subtype:Mini Volcano ");i += 2;break;
			case 9:sprintf_s(buffer, "Subtype:Volcano ");i += 2;break;
			case 0xa:sprintf_s(buffer, "Subtype:Mini crater ");i += 2;break;
			case 0xb:sprintf_s(buffer, "Subtype:Crater ");i += 2;break;
			case 0xc:sprintf_s(buffer, "Subtype:12 ");i += 2;break;
			case 0xd:sprintf_s(buffer, "Subtype:White smoke ");i += 2;break;
			case 0xe:sprintf_s(buffer, "Subtype:Black smoke ");i += 2;break;
			case 0xf:sprintf_s(buffer, "Subtype:Earthquake ");i += 2;break;
			case 0x10:sprintf_s(buffer, "Subtype:16 ");i += 2;break;
			case 0x11:sprintf_s(buffer, "Subtype:Meteor ");i += 2;break;
			case 0x12:sprintf_s(buffer, "Subtype:18 ");i += 2;break;
			case 0x13:sprintf_s(buffer, "Subtype:19 ");i += 2;break;
			case 0x14:sprintf_s(buffer, "Subtype:20 ");i += 2;break;
			case 0x15:sprintf_s(buffer, "Subtype:Steal Mana ");i += 2;break;
			case 0x16:sprintf_s(buffer, "Subtype:22 ");i += 2;break;
			case 0x17:sprintf_s(buffer, "Subtype:Lighting ");i += 2;break;
			case 0x18:sprintf_s(buffer, "Subtype:Rain of Fire ");i += 2;break;
			case 0x19:sprintf_s(buffer, "Subtype:not listed in INF? another steal mana? ");i += 2;break;
			case 0x1a:sprintf_s(buffer, "Subtype:26 ");i += 2;break;
			case 0x1b:sprintf_s(buffer, "Subtype:27 ");i += 2;break;
			case 0x1c:sprintf_s(buffer, "Subtype:wall ");i += 2;break;
			case 0x1d:sprintf_s(buffer, "Subtype:path ");i += 2;break;
			case 0x1e:sprintf_s(buffer, "Subtype:Black smoke ");i += 2;break;
			case 0x1f:sprintf_s(buffer, "Subtype:Canyon ");i += 2;break;
			case 0x20:sprintf_s(buffer, "Subtype:32 ");i += 2;break;
			case 0x21:sprintf_s(buffer, "Subtype:33 ");i += 2;break;
			case 0x22:sprintf_s(buffer, "Subtype:Teleport ");i += 2;break;
			case 0x23:sprintf_s(buffer, "Subtype:35 ");i += 2;break;
			case 0x24:sprintf_s(buffer, "Subtype:36 ");i += 2;break;
			case 0x25:sprintf_s(buffer, "Subtype:37 ");i += 2;break;
			case 0x26:sprintf_s(buffer, "Subtype:38 ");i += 2;break;
			case 0x27:sprintf_s(buffer, "Subtype:Mana Ball ");i += 2;break;
			case 0x28:sprintf_s(buffer, "Subtype:40 ");i += 2;break;
			case 0x29:sprintf_s(buffer, "Subtype:41 ");i += 2;break;
			case 0x2a:sprintf_s(buffer, "Subtype:42 ");i += 2;break;
			case 0x2b:sprintf_s(buffer, "Subtype:43 ");i += 2;break;
			case 0x2c:sprintf_s(buffer, "Subtype:44 ");i += 2;break;
			case 0x2d:sprintf_s(buffer, "Subtype:45-no in INF ");i += 2;break;
			case 0x2e:sprintf_s(buffer, "Subtype:46 ");i += 2;break;
			case 0x2f:sprintf_s(buffer, "Subtype:47 ");i += 2;break;
			case 0x30:sprintf_s(buffer, "Subtype:48 ");i += 2;break;
			case 0x31:sprintf_s(buffer, "Subtype:49-no in INF ");i += 2;break;
			case 0x32:sprintf_s(buffer, "Subtype:Ridge Node ");i += 2;break;
			case 0x33:sprintf_s(buffer, "Subtype:51 ");i += 2;break;
			case 0x34:sprintf_s(buffer, "Subtype:Crab Egg ");i += 2;break;
			case 0x3c:sprintf_s(buffer, "Subtype:?Gray smoke ");i += 2;break;

			default:sprintf_s(buffer, "Subtype:Error%u ", ((unsigned int)content[i] & 0xff + ((unsigned int)content[i + 1] & 0xff << 8)));i += 2;break;
			}
			break;
		case 0xb:sprintf_s(buffer, "Type:Switches ");
			fwrite(buffer, strlen(buffer), 1, fptw);
			i += 2;
			outsubtype = ((unsigned int)content[i] & 0xff + ((unsigned int)content[i + 1] & 0xff << 8));
			switch ((unsigned int)content[i] & 0xff + ((unsigned int)content[i + 1] & 0xff << 8)) {
			case 0:sprintf_s(buffer, "Subtype:Hidden Inside ");i += 2;break;
			case 1:sprintf_s(buffer, "Subtype:Hidden outside ");i += 2;break;
			case 2:sprintf_s(buffer, "Subtype:Hidden Inside re ");i += 2;break;
			case 3:sprintf_s(buffer, "Subtype:3 ");i += 2;break;
			case 4:sprintf_s(buffer, "Subtype:On victory ");i += 2;break;
			case 5:sprintf_s(buffer, "Subtype:Death Inside ");i += 2;break;
			case 6:sprintf_s(buffer, "Subtype:Death Outside ");i += 2;break;
			case 7:sprintf_s(buffer, "Subtype:Death inside re ");i += 2;break;
			case 8:sprintf_s(buffer, "Subtype:8 ");i += 2;break;
			case 9:sprintf_s(buffer, "Subtype:Obvious Inside ");i += 2;break;
			case 0xa:sprintf_s(buffer, "Subtype:Obvious outside ");i += 2;break;
			case 0xb:sprintf_s(buffer, "Subtype:11 ");i += 2;break;
			case 0xc:sprintf_s(buffer, "Subtype:12 ");i += 2;break;
			case 0xd:sprintf_s(buffer, "Subtype:Dragon ");i += 2;break;
			case 0xe:sprintf_s(buffer, "Subtype:Vulture ");i += 2;break;
			case 0xf:sprintf_s(buffer, "Subtype:Bee ");i += 2;break;
			case 0x10:sprintf_s(buffer, "Subtype:None ");i += 2;break;
			case 0x11:sprintf_s(buffer, "Subtype:Archer ");i += 2;break;
			case 0x12:sprintf_s(buffer, "Subtype:Crab ");i += 2;break;
			case 0x13:sprintf_s(buffer, "Subtype:Kraken ");i += 2;break;
			case 0x14:sprintf_s(buffer, "Subtype:Troll ");i += 2;break;
			case 0x15:sprintf_s(buffer, "Subtype:Griffon ");i += 2;break;
			case 0x16:sprintf_s(buffer, "Subtype:22 ");i += 2;break;
			case 0x17:sprintf_s(buffer, "Subtype:23 ");i += 2;break;
			case 0x18:sprintf_s(buffer, "Subtype:Genie ");i += 2;break;
			case 0x19:sprintf_s(buffer, "Subtype:25 ");i += 2;break;
			case 0x1a:sprintf_s(buffer, "Subtype:26 ");i += 2;break;
			case 0x1b:sprintf_s(buffer, "Subtype:27 ");i += 2;break;
			case 0x1c:sprintf_s(buffer, "Subtype:28 ");i += 2;break;
			case 0x1d:sprintf_s(buffer, "Subtype:Wyvern ");i += 2;break;
			case 0x1e:sprintf_s(buffer, "Subtype:Creature all ");i += 2;break;

			default:sprintf_s(buffer, "Subtype:Error%u ", ((unsigned int)content[i] & 0xff + ((unsigned int)content[i + 1] & 0xff << 8)));i += 2;break;
			}
			break;
		case 0xc:sprintf_s(buffer, "Type:Spells ");
			fwrite(buffer, strlen(buffer), 1, fptw);
			i += 2;
			outsubtype = ((unsigned int)content[i] & 0xff + ((unsigned int)content[i + 1] & 0xff << 8));
			switch ((unsigned int)content[i] & 0xff + ((unsigned int)content[i + 1] & 0xff << 8)) {
			case 0:sprintf_s(buffer, "Subtype:Fireball ");i += 2;break;
			case 1:sprintf_s(buffer, "Subtype:Heal ");i += 2;break;
			case 2:sprintf_s(buffer, "Subtype:Speed Up ");i += 2;break;
			case 3:sprintf_s(buffer, "Subtype:Posession ");i += 2;break;
			case 4:sprintf_s(buffer, "Subtype:Shield ");i += 2;break;
			case 5:sprintf_s(buffer, "Subtype:Beyond Sight ");i += 2;break;
			case 6:sprintf_s(buffer, "Subtype:Earthquake ");i += 2;break;
			case 7:sprintf_s(buffer, "Subtype:Meteor ");i += 2;break;
			case 8:sprintf_s(buffer, "Subtype:Volcano ");i += 2;break;
			case 9:sprintf_s(buffer, "Subtype:Crater ");i += 2;break;
			case 0xa:sprintf_s(buffer, "Subtype:Teleport ");i += 2;break;
			case 0xb:sprintf_s(buffer, "Subtype:Rubber Band ");i += 2;break;
			case 0xc:sprintf_s(buffer, "Subtype:Invisible ");i += 2;break;
			case 0xd:sprintf_s(buffer, "Subtype:Steal Mana ");i += 2;break;
			case 0xe:sprintf_s(buffer, "Subtype:Rebound ");i += 2;break;
			case 0xf:sprintf_s(buffer, "Subtype:Lightning ");i += 2;break;
			case 0x10:sprintf_s(buffer, "Subtype:Castle ");i += 2;break;
			case 0x11:sprintf_s(buffer, "Subtype:Skeleton ");i += 2;break;
			case 0x12:sprintf_s(buffer, "Subtype:Thunderbolt ");i += 2;break;
			case 0x13:sprintf_s(buffer, "Subtype:Mana Magnet ");i += 2;break;
			case 0x14:sprintf_s(buffer, "Subtype:Fire Wall ");i += 2;break;
			case 0x15:sprintf_s(buffer, "Subtype:Reverse Speed ");i += 2;break;
			case 0x16:sprintf_s(buffer, "Subtype:Smart Bomb ");i += 2;break;
			case 0x17:sprintf_s(buffer, "Subtype:Mini Fireball ");i += 2;break;
			default:sprintf_s(buffer, "Subtype:Error%u ", ((unsigned int)content[i] & 0xff + ((unsigned int)content[i + 1] & 0xff << 8)));i += 2;break;
			}
		case 0xd: {sprintf_s(buffer, "Type:13 ? ");
			fwrite(buffer, strlen(buffer), 1, fptw);
			i += 2;
			outsubtype = ((unsigned int)content[i] & 0xff + ((unsigned int)content[i + 1] & 0xff << 8));
			sprintf_s(buffer, "Subtype:Error%u ", ((unsigned int)content[i] & 0xff + ((unsigned int)content[i + 1] & 0xff << 8)));
			i += 2;
			break;
		}
		case 0xe: {sprintf_s(buffer, "Type:14 ? ");
			fwrite(buffer, strlen(buffer), 1, fptw);
			i += 2;
			outsubtype = ((unsigned int)content[i] & 0xff + ((unsigned int)content[i + 1] & 0xff << 8));
			sprintf_s(buffer, "Subtype:Error%u ", ((unsigned int)content[i] & 0xff + ((unsigned int)content[i + 1] & 0xff << 8)));
			i += 2;
			break;
		}
		case 0xf: {sprintf_s(buffer, "Type:15 ? ");
			fwrite(buffer, strlen(buffer), 1, fptw);
			i += 2;
			outsubtype = ((unsigned int)content[i] & 0xff + ((unsigned int)content[i + 1] & 0xff << 8));
			sprintf_s(buffer, "Subtype:Error%u ", ((unsigned int)content[i] & 0xff + ((unsigned int)content[i + 1] & 0xff << 8)));
			i += 2;
			//if (outsubtype == 2)
				shift = -2;
			break;
		}


a1 + 4 - max life 
a1 + 8 - act life
a1 + 76 - position