#include "Mc1Install.h"

#include <cstdio>
#include <cstring>

#include "Mc1Files.h"
#include "Mc1Game.h"
#include "../../remc2/portability/port_filesystem.h"
#include "../../remc2/utilities/DataFileIO.h"

static const char* progressText = "";

std::string Mc1InstallPath()
{
	return mc1DataPath + "/CARPET.CD";
}

int16_t Access_3EC80_3EFC0(const char* path)
{
	// DOS access: 0 when the path exists, -1 otherwise (myaccess returns 1 on success)
	return myaccess(path, 0) >= 0 ? 0 : -1;
}

uint8_t CreateInstallDirectory_3EC90_3EFD0(char /*drive*/, const char* /*basePath*/, const char* subDirectory)
{
	// the original first checks the drive with _dos_getdiskfree (returns 1 when it fails); C:\carpet.cd is the install path
	std::string basePath = Mc1InstallPath();
	if (Access_3EC80_3EFC0(basePath.c_str()) < 0 && mymkdir(basePath.c_str()))
		return 2;
	std::string path = basePath + "/" + subDirectory;
	if (Access_3EC80_3EFC0(path.c_str()) < 0 && mymkdir(path.c_str()))
		return 2;
	return 0;
}

bool FileMissing_3EF20_3F260(const char* /*targetPath*/, const char* /*sourcePath*/)
{
	// the shipped game always returns 1, so the files are copied at every start
	return true;
}

bool PathExists_3EF30_3F270(const char* path)
{
	return Access_3EC80_3EFC0(path) == 0;
}

uint8_t CopyDataFiles_3EF50_3F290(const char* sourceDirectory, const char* targetDirectory, const char* name)
{
	char sourcePath[144];
	char targetPath[144];
	uint8_t* buffer = *workScreen_12EFF4_12EFE4;

	if (!PathExists_3EF30_3F270(sourceDirectory))
		return 2;
	if (!PathExists_3EF30_3F270(targetDirectory))
		return 1;

	snprintf(sourcePath, sizeof(sourcePath), "%s/%s.tab", sourceDirectory, name);
	snprintf(targetPath, sizeof(targetPath), "%s/%s.tab", targetDirectory, name);
	FILE* source = DataFileIO::CreateOrOpenFile(sourcePath, 0x200);
	if (!source)
		return 3;
	FILE* target = DataFileIO::CreateOrOpenFile(targetPath, 0x222);
	if (!target)
	{
		//!!!!! the original leaves the source file open here
		DataFileIO::Close(source);
		return 3;
	}
	uint32_t length = (uint32_t)DataFileIO::FileLengthBytes(source);
	size_t read = DataFileIO::Read(source, buffer, length);
	WriteFile_62ED0_633E0(target, buffer, (uint32_t)read);
	DataFileIO::Close(source);
	DataFileIO::Close(target);

	snprintf(sourcePath, sizeof(sourcePath), "%s/%s.dat", sourceDirectory, name);
	snprintf(targetPath, sizeof(targetPath), "%s/%s.dat", targetDirectory, name);
	source = DataFileIO::CreateOrOpenFile(sourcePath, 0x200);
	if (source)
	{
		target = DataFileIO::CreateOrOpenFile(targetPath, 0x222);
		if (target)
		{
			int32_t copied = 0;
			int32_t total = (int32_t)DataFileIO::FileLengthBytes(source);
			int32_t remaining = total;
			while (remaining)
			{
				int32_t chunk = remaining <= 64000 ? remaining : 64000;
				int32_t done = (int32_t)DataFileIO::Read(source, buffer, chunk);
				if (done <= 0)
					break; // a read error loops forever in the original
				remaining -= done;
				WriteFile_62ED0_633E0(target, buffer, done);
				copied += done;
				int16_t percent = (int16_t)(100 * copied / total);
				if (percent > 100)
					percent = 100;
				// the original writes the percent at the text position saved after the label
				printf("\r%s%d%c", progressText, percent, '%');
			}
			DataFileIO::Close(target);
		}
		//!!!!! the original leaves the source file open when the target cannot be created
		DataFileIO::Close(source);
	}
	memset(buffer, 0, 64000);
	return 0;
}

static void SetupInstallFile(const char* directory, const char* name, const char* label)
{
	std::string target = Mc1InstallPath() + "/" + directory;
	std::string targetFile = target + "/" + name + ".dat";
	std::string sourceFile = std::string(directory) + "/" + name + ".dat";
	if (!FileMissing_3EF20_3F260(targetFile.c_str(), sourceFile.c_str()))
		return;
	progressText = label;
	printf("%s", label);
	CopyDataFiles_3EF50_3F290(Mc1DataFilePath(directory).c_str(), target.c_str(), name);
}

void SetupInstallFiles_3F1D0_3F510()
{
	bool hw = mc1Variant == MC1_VARIANT_HW;
	SetupInstallFile("data", hw ? "tmaps1-0" : "tmaps0-0", "Creating Setup Directories..");
	printf("\n");
	SetupInstallFile("levels", hw ? "ddlevels" : "levels", "Setup levels..");
}
