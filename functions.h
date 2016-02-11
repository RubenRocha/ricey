#ifndef FUNC_H
#define FUNC_H

#ifdef _WIN32
#include "windows.h"
#elif __APPLE__
#include "osx.h"
#elif __linux__
#include "linux.h"
#endif


#include <fstream>
#include <stdio.h>

typedef struct {
	char* cpu_brand;
	char* hostname;
	char* username;
	
	char* uptime;
	char* gpu_model;
	
	
	char* shell;
	char* terminal;

	char used_system_memory[128],total_system_memory[128];
	char used_disk[128],total_disk[128];

	float disk[2];

	char* os_version;
	char* resolution;
} info;

void readable_fs(float size, char buf[128]);
char* readable_ut(float time);
bool file_exist(const char *fileName);
char *trim(char *str);
void gather_info(info *_info);
void free_info(info *_info);

#endif
