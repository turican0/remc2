#include "read_config.h"
/*
typedef struct
{
	int version;
	const char* name;
	const char* email;
} configuration;
*/
/*static int handler(void* user, const char* section, const char* name,
	const char* value)
{
	configuration* pconfig = (configuration*)user;

#define MATCH(s, n) strcmp(section, s) == 0 && strcmp(name, n) == 0
	if (MATCH("protocol", "version")) {
		pconfig->version = atoi(value);
	}
	else if (MATCH("user", "name")) {
		pconfig->name = strdup(value);
	}
	else if (MATCH("user", "email")) {
		pconfig->email = strdup(value);
	}
	else {
		return 0;
	}
	return 1;
}*/
int config_skip_screen;
void readini(char* filename) {
	/*configuration config;

	if (ini_parse(filename, handler, &config) < 0) {
		printf("Can't load '%s'\n", filename);
		return;
	}
	printf("Config loaded from '%s': version=%d, name=%s, email=%s\n", filename,
		config.version, config.name, config.email);
		*/
	INIReader reader(filename);

	if (reader.ParseError() < 0) {
		std::cout << "Can't load 'test.ini'\n";
		return;
	}
	if (reader.GetBoolean("skips", "skipintro", true))
		config_skip_screen = 1;
	else
		config_skip_screen = 0;

	if (reader.GetBoolean("sound", "hqsound", true))
		hqsound = true;
	else
		hqsound = false;

	if (reader.GetBoolean("sound", "oggmusic", true))
	{
		oggmusic = true;
		hqsound = true;//for mp3 music must be activate hqsound
	}
	else
		oggmusic = false;
	if (reader.GetBoolean("sound", "oggmusicalternative", true))
	{
		oggmusicalternative = true;
	}
	else
		oggmusicalternative = false;
	std::string readstr = reader.GetString("sound", "oggmusicpath", "");
	strcpy(oggmusicpath, (char*)readstr.c_str());

	std::string readstr2 = reader.GetString("main", "gamepath", "");
	strcpy((char*)gamepath, (char*)readstr2.c_str());
};