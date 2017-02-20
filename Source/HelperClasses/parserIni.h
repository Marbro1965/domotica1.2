#ifndef PARSER_INI_H_
#define PARSER_INI_H_

	static long ReadFromFile(unsigned long ulToken, long lFileHandle);

	static long WriteToFile(unsigned long *ulToken, long *lFileHandle);

	static char* ini_buffer_reader(char* str, int num, void* stream);

	static int handler(void* user, const char* section, const char* name,const char* value);

	static int parseFile();

	static int outputFile();



#endif
