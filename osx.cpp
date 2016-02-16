#include <stdio.h>
#include <iostream>
#include <sys/types.h>
#include <sys/sysctl.h>
#include <mach/mach.h>
#include <sys/param.h>
#include <sys/mount.h>
#include <string.h>
#import <CoreServices/CoreServices.h>
#import <CoreGraphics/CoreGraphics.h>
#include <pwd.h>
#import "display.h"
#include <regex>



using namespace std;

char* get_cpu_brand(void) {
    char* brand = (char*) malloc(128);
    
    char final[128];
    size_t size_b = sizeof(final);

    if (sysctlbyname("machdep.cpu.brand_string", &final, &size_b, NULL, 0) < 0)
    {
        perror("sysctl");
    }
    

    regex reg("\\(TM\\)|\\(R\\)");

    if (regex_search(final, reg)) {
        brand = strdup(regex_replace(final, reg, "").c_str());
    } else {
        strncpy(brand, final, 128);
    }

    return brand;
}


float get_total_system_memory()
{
    int mib[2] = { CTL_HW, HW_MEMSIZE };
    u_int namelen = sizeof(mib) / sizeof(mib[0]);
    long long size;
    size_t len = sizeof(size);

    if (sysctl(mib, namelen, &size, &len, NULL, 0) < 0)
    {
        perror("sysctl");
    }

    return size;
}

float get_used_system_memory()
{
	mach_msg_type_number_t count = HOST_VM_INFO_COUNT;
	vm_statistics_data_t vmstat;
	if(KERN_SUCCESS == host_statistics(mach_host_self(), HOST_VM_INFO, (host_info_t)&vmstat, &count)) {
		double mtotal = vmstat.wire_count + vmstat.active_count + vmstat.inactive_count + vmstat.free_count;
		double mfree = vmstat.free_count;
		return (mtotal - mfree) * 4096;
	}
	return 0;
}

char* get_hostname()
{
    char* hostname = (char*) malloc(128*sizeof(char));
    
    char final[128];
    
    size_t size_h = sizeof(final);

    if (sysctlbyname("kern.hostname", &final, &size_h, NULL, 0) < 0)
    {
        perror("sysctl");
    }
    

    strncpy(hostname, final, 128);
    return hostname;
}

float get_uptime()
{
    struct timeval boottime;
    size_t len = sizeof(boottime);
    int mib[2] = { CTL_KERN, KERN_BOOTTIME };
    if( sysctl(mib, 2, &boottime, &len, NULL, 0) < 0 )
    {
        return -1.0;
    }
    time_t bsec = boottime.tv_sec, csec = time(NULL);

    return difftime(csec, bsec);
}

char* get_gpu_model()
{
    FILE *gpu;
    char* buffer = (char*)malloc(128*sizeof(char));
    gpu = popen("system_profiler SPDisplaysDataType | awk -F': ' '/Chipset Model/ {print $2}' | tail -1", "r");

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
    char* buf = (char*) malloc(128*sizeof(char));
    int versionMajor=0, versionMinor=0, versionBugFix=0;
    Gestalt(gestaltSystemVersionMajor, &versionMajor);
    Gestalt(gestaltSystemVersionMinor, &versionMinor);
    Gestalt(gestaltSystemVersionBugFix, &versionBugFix);
    sprintf(buf, "%d.%d.%d", versionMajor, versionMinor, versionBugFix);
    return buf;
}

char* get_resolution()
{
	CGRect mainMonitor = CGDisplayBounds(CGMainDisplayID());
	CGFloat monitorHeight = CGRectGetHeight(mainMonitor);
	CGFloat monitorWidth = CGRectGetWidth(mainMonitor);
    float scale = get_resolution_scale();

	char* res = (char*) malloc(128*sizeof(char));
	sprintf(res, "%.fx%.f", monitorWidth*scale, monitorHeight*scale);
	return res;
}

char* art()
{


char* art_txt =
"\033[00;32m                    ,            \e[0m\n"
"\033[00;32m                  ,##;           \e[0m\n"
"\033[00;32m                 ####            \e[0m\e[91m%s\e[0m@%s\n"
"\033[00;32m                ;#'              \e[0m\e[1mCPU:\e[0m %s\n"
"\033[00;32m       ,#####;,   ,;#####;,      \e[0m\e[1mMemory:\e[0m  %s/%s (used/total)\n"
"\033[00;32m     ######################'     \e[0m\e[1mUptime:\e[0m  %s\n"
"\033[00;32m    #####################'       \e[0m\e[1mGPU:\e[0m %s\n"
"\033[00;33m    #####################        \e[0m\e[1mShell:\e[0m %s\n"
"\033[00;33m    #####################        \e[0m\e[1mTerminal:\e[0m %s\n"
"\033[01;31m    ######################,      \e[0m\e[1mDisk:\e[0m %s/%s (used/total)\n"
"\033[00;31m     #######################     \e[0m\e[1mVersion:\e[0m OS X %s\n"
"\033[00;31m      ####################'      \e[0m\e[1mResolution:\e[0m %s\n"
"\033[00;35m        #################'       \e[0m\n"
"\033[00;34m         '####''''####'          \e[0m\n";

	return art_txt;
}