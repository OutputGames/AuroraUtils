#if !defined(FS_HPP)
#define FS_HPP
#include "utils.hpp"

std::string ReadString(const std::string path);

std::string GetFileNameWithoutExt(std::string path);
bool WriteString(std::string path, std::string data);


template <typename T>
// Export data to code (.h), returns true on success
bool ExportDataAsCode(T* data, const char* fileName)
{
	bool success = false;

#ifndef TEXT_BYTES_PER_LINE
#define TEXT_BYTES_PER_LINE     20
#endif

	int dataSize = std::size(data);

	// NOTE: Text data buffer size is estimated considering raw data size in bytes
	// and requiring 6 char bytes for every byte: "0x00, "
	char* txtData = (char*)calloc(dataSize * 6 + 2000, sizeof(char));

	int byteCount = 0;
	byteCount += sprintf(txtData + byteCount, "////////////////////////////////////////////////////////////////////////////////////////\n");
	byteCount += sprintf(txtData + byteCount, "//                                                                                    //\n");
	byteCount += sprintf(txtData + byteCount, "// DataAsCode exporter v1.0 - Raw data exported as an array of bytes                  //\n");
	byteCount += sprintf(txtData + byteCount, "//                                                                                    //\n");
	byteCount += sprintf(txtData + byteCount, "// more info and bugs-report:  github.com/raysan5/raylib                              //\n");
	byteCount += sprintf(txtData + byteCount, "// feedback and support:       ray[at]raylib.com                                      //\n");
	byteCount += sprintf(txtData + byteCount, "//                                                                                    //\n");
	byteCount += sprintf(txtData + byteCount, "// Copyright (c) 2022-2023 Ramon Santamaria (@raysan5)                                //\n");
	byteCount += sprintf(txtData + byteCount, "//                                                                                    //\n");
	byteCount += sprintf(txtData + byteCount, "////////////////////////////////////////////////////////////////////////////////////////\n\n");

	// Get file name from path and convert variable name to uppercase
	char varFileName[256] = { 0 };
	strcpy(varFileName, GetFileNameWithoutExt(fileName).c_str());
	for (int i = 0; varFileName[i] != '\0'; i++) if ((varFileName[i] >= 'a') && (varFileName[i] <= 'z')) { varFileName[i] = varFileName[i] - 32; }

	byteCount += sprintf(txtData + byteCount, "#define %s_DATA_SIZE     %i\n\n", varFileName, dataSize);

	byteCount += sprintf(txtData + byteCount, "static unsigned char %s_DATA[%s_DATA_SIZE] = { ", varFileName, varFileName);
	for (int i = 0; i < (dataSize - 1); i++) byteCount += sprintf(txtData + byteCount, ((i % TEXT_BYTES_PER_LINE == 0) ? "0x%x,\n" : "0x%x, "), data[i]);
	byteCount += sprintf(txtData + byteCount, "0x%x };\n", data[dataSize - 1]);

	// NOTE: Text data size exported is determined by '\0' (NULL) character
	success = WriteString(fileName, txtData);

	free(txtData);

	return success;
}

#endif // FS_HPP
