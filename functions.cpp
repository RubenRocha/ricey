#ifdef _WIN32
#include "windows.h"
#elif __APPLE__
#include "osx.h"
#elif __linux__
#include "linux.h"
#elif __FreeBSD__
#include "bsd.h"
#endif

#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <cstring>
#include <iostream>
#include "functions.h"

void readable_fs(float size, char buf[128]) {
	
	int i = 0;
	const char* units[] = {"B", "kB", "MB", "GB", "TB", "PB", "EB", "ZB", "YB"};
	while (size > 1024) {
		size /= 1024;
		i++;
	}
	sprintf(buf, "%.2f %s", size, units[i]);
}

char* readable_ut(float time)
{
    char* test = (char*)malloc(sizeof(char)*200);
    
	int input_seconds = int(time);
	int days = input_seconds / 60 / 60 / 24;
	int hours = (input_seconds / 60 / 60) % 24;
	int minutes = (input_seconds / 60) % 60;
	int seconds = input_seconds % 60;
	
	sprintf(test, "%id:%ih:%im:%is", days, hours, minutes, seconds);
    return test;
	
}

bool file_exist(const char *fileName)
{
	std::ifstream file (fileName);
	return file.good();
}

char *trim(char *str)
{
	size_t len = 0;
	char *frontp = str;
	char *endp = NULL;
	
	if( str == NULL ) { return NULL; }
	if( str[0] == '\0' ) { return str; }
	
	len = strlen(str);
	endp = str + len;
    
	while( isspace(*frontp) ) { ++frontp; }
	if( endp != frontp )
	{
		while( isspace(*(--endp)) && endp != frontp ) {}
	}
	
	if( str + len - 1 != endp )
		*(endp + 1) = '\0';
	else if( frontp != str &&  endp == frontp )
		*str = '\0';
	
	endp = str;
	if( frontp != str )
	{
		while( *frontp ) { *endp++ = *frontp++; }
		*endp = '\0';
	}
	
	
	return str;
}


void gather_info(info *_info)
{
	_info->cpu_brand = get_cpu_brand();
	_info->hostname = get_hostname();
	_info->username = get_username();
	_info->uptime = readable_ut(get_uptime());
	_info->gpu_model = get_gpu_model();
	_info->shell = get_shell();
	_info->terminal = get_terminal();
	_info->os_version = get_os_version();
	_info->resolution = get_resolution();

	readable_fs(get_used_system_memory(),  _info->used_system_memory);
	readable_fs(get_total_system_memory(), _info->total_system_memory);

	get_disk_usage(_info->disk);

	readable_fs(_info->disk[0], _info->used_disk);
	readable_fs(_info->disk[1], _info->total_disk);
}

/*void free_info(info *_info)
{
		//free(_info->cpu_brand);
		free(_info->gpu_model);
		free(_info->os_version);
		//free(_info->resolution);

		#ifdef __APPLE__
		free(_info->hostname);
		free(_info->shell);
		free(_info->terminal);
		#endif
	    
	    #ifdef __linux__
		free(_info->shell);
		free(_info->terminal);
	    #endif

}
*/