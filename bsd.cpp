#include <stdio.h>
#include <iostream>
#include <sys/types.h>
#include <sys/sysctl.h>
#include <sys/param.h>
#include <sys/mount.h>
#include <string.h>
#include <pwd.h>
#include <unistd.h>
#include <sys/vmmeter.h>
#include <vm/vm_param.h>
#include <memstat.h>
#include <X11/Xlib.h>
#include <stdlib.h>

using namespace std;

char* get_cpu_brand(void) {
    char* brand = (char*) malloc(128);
    
    char final[128];
    size_t size_b = sizeof(final);


    if (sysctlbyname("hw.model", &final, &size_b, NULL, 0) < 0)
    {
        perror("sysctl");
    }
    
    strncpy(brand, final, 128);
    return brand;
}



float get_total_system_memory()
{
    int mib[2] = { CTL_HW, HW_REALMEM };
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
    float mem_used;
    
    int mem_wire, mem_inactive,mem_cache;
    int mem_free;

    size_t size_ty = sizeof(128);

    if (sysctlbyname("vm.stats.vm.v_wire_count", &mem_wire, &size_ty, NULL, 0) < 0) { perror("sysctl"); }
    if (sysctlbyname("vm.stats.vm.v_inactive_count", &mem_inactive, &size_ty, NULL, 0) < 0) { perror("sysctl"); }
    if (sysctlbyname("vm.stats.vm.v_cache_count", &mem_cache, &size_ty, NULL, 0) < 0) { perror("sysctl"); }
    if (sysctlbyname("vm.stats.vm.v_free_count", &mem_free, &size_ty, NULL, 0) < 0) { perror("sysctl"); }

    mem_used = get_total_system_memory() - (4096 * (mem_inactive + mem_cache + mem_free));

    return mem_used;
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
    gpu = popen("pciconf -l -v | grep -A2 vga | cut -d \\' -f 2 | tail -n +2 | tr '\n' ' '", "r");

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
    char* osstring = (char*) malloc(256);
    
    char finaltype[128];
    char finalrelease[128];
    size_t size_ty = sizeof(finaltype);
    size_t size_re = sizeof(finaltype);


    if (sysctlbyname("kern.ostype", &finaltype, &size_ty, NULL, 0) < 0)
    {
        perror("sysctl");
    }

    if (sysctlbyname("kern.osrelease", &finalrelease, &size_re, NULL, 0) < 0)
    {
        perror("sysctl");
    }

    snprintf(osstring, 256, "%s %s", finaltype, finalrelease);
    return osstring;
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
  "\e[31;1m  ```                        `   \n "  
  "\e[31;1m s` `.....---.......--.```  -/  \e[91m%s\e[0m@%s\n"
  "\e[31;1m +o   .--`         /y:`      +.  \e[0;1mCPU:\e[0m %s\n" 
  "\e[31;1m  yo`:.            :o      `+-   \e[0;1mMemory:\e[0m  %s/%s (used/total)\n"  
  "\e[31;1m   y/               -/`   -o/    \e[0;1mUptime:\e[0m  %s\n"   
  "\e[31;1m  .-                  ::/sy+:.   \e[0;1mGPU:\e[0m %s\n" 
  "\e[31;1m  /                     `--  /   \e[0;1mShell:\e[0m %s\n" 
  "\e[31;1m `:                          :`  \e[0;1mTerminal:\e[0m %s\n" 
  "\e[31;1m `:                          :`  \e[0;1mDisk:\e[0m %s/%s (used/total)\n" 
  "\e[31;1m  /                          /   \e[0;1mVersion:\e[0m %s\n"  
  "\e[31;1m  .-                        -.   \e[0;1mResolution:\e[0m %s\n" 
  "\e[31;1m   --                      -.    \e[0m\n"   
  "\e[31;1m    `:`                  `:`     \e[0m\n"    
  "\e[31;1m      .--             `--.       \e[0m\n"      
  "\e[31;1m        .---.....----.      \e[0m\n";         

	return art_txt;
    
}
