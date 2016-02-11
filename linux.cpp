#include <stdio.h>
#include <iostream>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/sysinfo.h>
#include <sys/statfs.h>
#include "functions.h"
#include <sys/utsname.h>
#include <X11/Xlib.h>
#include <pwd.h>

using namespace std;

char* get_cpu_brand() {
    FILE *cpuinf;
    char* buffer;
    buffer = (char*) malloc(128*sizeof(char));

    cpuinf = popen("/bin/cat /proc/cpuinfo | grep 'model name' | cut -d: -f2 | head -1", "r");
    fgets(buffer, 128, cpuinf);
    pclose(cpuinf);
    strtok(buffer, "\n");

    return buffer;
}

float get_total_system_memory()
{
	long int page_size = sysconf(_SC_PAGESIZE);
	long int phys_pages =  sysconf(_SC_PHYS_PAGES);;
	return (float) page_size * phys_pages;
}

float get_used_system_memory()
{
	FILE *meminfo = fopen("/proc/meminfo", "r");

	if(meminfo == NULL)
	{
		printf("MEMINFO = NULL\n");
		exit(1);
	}

	float ram;
    char meminfo_line[256];

	while(fgets(meminfo_line, sizeof(meminfo_line), meminfo))
    {
        if(sscanf(meminfo_line, "MemAvailable: %f kB", &ram) == 1)
        {
            fclose(meminfo);
        }
    }

	return (get_total_system_memory() - (ram*1024));
}

char* get_hostname()
{
	char* hostname = (char*)malloc(128*sizeof(char));
	gethostname(hostname, 128);
	return hostname;
}

float get_uptime()
{
	struct sysinfo si;
	sysinfo (&si);
	return si.uptime;

}

char* get_gpu_model()
{
	FILE *gpu;
	char* buffer = (char*)malloc(128*sizeof(char));
	gpu = popen("lspci | grep VGA | awk -F ':' '{print $3}' | sed -e 's/^\s*//' -e '/^$/d'", "r");

	fgets(buffer, 128, gpu);
	pclose(gpu);
	strtok(buffer, "\n");
	return buffer;
}

char* get_shell()
{
    char* shell = (char*) malloc(128*sizeof(char));
    strncpy(shell, getenv("SHELL"), 128);
    return shell;
}

char* get_terminal()
{
    char* terminal = (char*) malloc(128*sizeof(char));
    strncpy(terminal, getenv("TERM"), 128);
    return terminal;
}

void get_disk_usage(float _disk[2])
{
	struct statfs statf;
	statfs(".", &statf);
	_disk[0] = statf.f_bsize * (statf.f_blocks - statf.f_bavail);
	_disk[1] = statf.f_bsize * statf.f_blocks;
}

char* get_username()
{
	return getpwuid(geteuid())->pw_name;
}

char* get_os_version()
{
	char* release = (char*) malloc(128*sizeof(char));
	struct utsname unameData;
	uname(&unameData);

	if(file_exist("/etc/debian_version"))
		release = "Debian";
	else if(file_exist("/etc/arch-release"))
		release = "Arch Linux";
	else if(file_exist("/etc/gentoo-release"))
		release = "Gentoo";
	else if(file_exist("/etc/knoppix-release"))
		release = "Knoppix";
	else if(file_exist("/etc/mandrake-release"))
		release = "Mandrake";
	else if(file_exist("/etc/puppyversion"))
		release = "Puppy Linux";
	else if(file_exist("/etc/fedora-release"))
		release = "Fedora";
	else if(file_exist("/etc/redhat-release"))
		release = "Redhat Linux";
	else if(file_exist("/etc/sabayon-release"))
		release = "Sabayon";
	else if(file_exist("/etc/slackware-release"))
		release = "Slackware";
	else if(file_exist("/etc/xandros-desktop-version"))
		release = "Xandros";
	else if(file_exist("/etc/zenwalk-version"))
		release = "Zenwalk";
	else
		snprintf(release, 128, "%s %s", unameData.machine, unameData.sysname);



	//std::cout << release;
	char* final = (char*)malloc(128*sizeof(char));

	sprintf(final, "%s (kernel %s)", release, unameData.release);
	return final;
}

char* get_resolution()
{
	Display* disp = XOpenDisplay(NULL);
	if(disp == false)
    {
        return "Unknown (X Server not running?)";
    }
	Screen*  scrn = DefaultScreenOfDisplay(disp);
	int height = scrn->height;
	int width  = scrn->width;

	char* res = (char*) malloc(128*sizeof(char));
	sprintf(res, "%ix%i", width, height);
	return res;
}

char* art()
{


	char* art_txt =
		"          \e[34ma8888b.\e[0m        \e[91m%s\e[0m@%s\n"
		"         \e[34md888888b.\e[0m       \e[1mCPU:\e[0m    %s\n"
		"         \e[34m8P'YP'Y88\e[0m       \e[1mMemory:\e[0m %s/%s (used/total)\n"
		"         \e[34m8|\e[91mo\e[34m||\e[91mo\e[34m|88\e[0m       \e[1mUptime:\e[0m %s\n"
		"         \e[34m8     \e[93m.\e[34m88\e[0m       \e[1mGPU:\e[0m    %s\n"
		"         \e[34m8\e[93m`._.' \e[34mY8.\e[0m      \e[1mShell:\e[0m  %s\n"
		"        \e[34md/      `8b.\e[0m     \e[1mTerm:\e[0m   %s\n"
		"       \e[34mdP\e[0m   .    \e[34mY8b.\e[0m    \e[1mDisk:\e[0m   %s/%s (used/total)\n"
		"      \e[34md8:\e[0m'  ''  `\e[34m::88b\e[0m   \e[1mDistro:\e[0m %s\n"
		"     \e[34md8'\e[0m         \e[34m'Y88b\e[0m   \e[1mResolution:\e[0m %s\n"
		"    \e[34m:8P\e[0m    '      \e[34m:888\e[0m   \n"
		"     \e[34m8a.\e[0m   :     \e[34m_a88P\e[0m   \n"
		"   \e[93m._/'Y\e[0maa_:   \e[93m.| 88P|   \n"
		"   \\    Y\e[0mP'    \e[93m`| 8P  `. \n"
		"   /     \\.___.d|    .'  \n"
		"   `--..__)\e[0m     \e[93m`._.'\e[0m \n";

	return art_txt;
}
