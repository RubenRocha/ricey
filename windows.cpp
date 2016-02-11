#define _WIN32_WINNT  0x0501
#define WINVER 0x0501

#pragma comment(lib,"user32.lib")
#pragma comment(lib,"advapi32.lib")

#include <windows.h>
#include <iostream>

#include <stdio.h>
#include <intrin.h>
#include "functions.h"

using namespace std;

char* get_cpu_brand()
{
    char pName[128];
    HKEY hKey = 0;
    char buf[255] = {0};
    DWORD dwType = 0;
    DWORD dwBufSize = sizeof(buf);
    char* subkey = "HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0";
    if( RegOpenKey(HKEY_LOCAL_MACHINE,subkey,&hKey) == ERROR_SUCCESS)
    {
        dwType = REG_SZ;
        if( RegQueryValueEx(hKey,"ProcessorNameString",0, &dwType, (BYTE*)buf, &dwBufSize) == ERROR_SUCCESS)
        {
            strncpy(pName, buf, 128);
        } else {
        	strncpy(pName, "Unknown Processor", 128);
        }
        RegCloseKey(hKey);
    } else {
        strncpy(pName, "Unknown Processor", 128);
    }

        char * buff;
        char rem[128];
        buff = strtok (pName,"(RTM)");
        while (buff != NULL)
        {
         strcat(rem, buff);
         buff = strtok (NULL, "(RTM)");
        }

        //rem[0] = (const char[2])"\0";
        memmove(rem, rem+1, strlen(rem));

        char* final = (char*) malloc(128 * sizeof(char));
        strncpy(final, rem, 128);

        return final;

}

double get_total_system_memory()
{
    MEMORYSTATUSEX ms;
    ms.dwLength = sizeof (ms);
    GlobalMemoryStatusEx(&ms);
    double totalMem = (ms.ullTotalPhys);

    return totalMem;
}

double get_used_system_memory()
{
    MEMORYSTATUSEX ms;
    ms.dwLength = sizeof (ms);
    GlobalMemoryStatusEx(&ms);
    double totalMem = (ms.ullTotalPhys);
    double availMem = (ms.ullAvailPhys);

    return totalMem - availMem;
}

char* get_hostname()
{
    char* compName = (char*) malloc(255*sizeof(char));
    DWORD size = 40;

    if(GetComputerName(compName,&size)!=0)
        return compName;
    return "Unknown";
}

char* get_username()
{
    char* uName = (char*) malloc(255*sizeof(char));
    DWORD size = 40;

    if(GetUserName(uName,&size) != 0)
        return uName;
    return "Unknown";
}

float get_uptime()
{
    float t;

    t = GetTickCount();
    return t/1000;
}


char* get_shell()
{
return "";
}

char* get_terminal()
{
return "";
}

void get_disk_usage(float _disk[2])
{
    #define wszDrive L"\\\\.\\PhysicalDrive0"
    BOOL  fResult;
    int col1;
    __int64 fbtc, tb, fb;
    

    fResult = GetDiskFreeSpaceEx((LPCSTR) wszDrive, (PULARGE_INTEGER)&fbtc,
                                 (PULARGE_INTEGER)&tb, (PULARGE_INTEGER)&fb);

    if (fResult)
    {
                 _disk[0] = tb - fb;
                 _disk[1] = tb;
    }
}

char* get_gpu_model()
{
    char* aName;
    HKEY hKey = 0;
    char buf[255] = {0};
    DWORD dwType = 0;
    DWORD dwBufSize = sizeof(buf);
    const char* subkey = "SYSTEM\\ControlSet001\\Control\\Class\\{4D36E968-E325-11CE-BFC1-08002BE10318}\\0000\\Settings";
    if( RegOpenKey(HKEY_LOCAL_MACHINE,subkey,&hKey) == ERROR_SUCCESS)
    {
        dwType = REG_SZ;
        if( RegQueryValueEx(hKey,"Device Description",0, &dwType, (BYTE*)buf, &dwBufSize) == ERROR_SUCCESS)
        {
            aName = buf;
        } else {
            aName = "Unknown Adapter";
        }
        RegCloseKey(hKey);
    } else {
        aName = "Unknown Adapter";
    }
     char* final = (char*) malloc(128 * sizeof(char));
     strncpy(final, aName, 128);
     return final;
}

char* get_os_version()
{
    OSVERSIONINFO verInfo = {sizeof(OSVERSIONINFO)};
    GetVersionEx(&verInfo);
    char* buf = (char*) malloc(sizeof(char) * 128);

    if(verInfo.dwMajorVersion == 4 && verInfo.dwMinorVersion == 10)
        sprintf(buf, "Windows 98 %s",verInfo.szCSDVersion);
    if(verInfo.dwMajorVersion == 5 && verInfo.dwMinorVersion == 0)
        sprintf(buf, "Windows 2000 %s",verInfo.szCSDVersion);
    if(verInfo.dwMajorVersion == 5 && verInfo.dwMinorVersion == 1)
        sprintf(buf, "Windows XP %s",verInfo.szCSDVersion);
    if(verInfo.dwMajorVersion == 5 && verInfo.dwMinorVersion == 2)
        sprintf(buf, "Windows Server 2003 %s",verInfo.szCSDVersion);
    if(verInfo.dwMajorVersion == 6 && verInfo.dwMinorVersion == 0)
        sprintf(buf, "Windows Vista %s",verInfo.szCSDVersion);
    if(verInfo.dwMajorVersion == 6 && verInfo.dwMinorVersion == 1)
        sprintf(buf, "Windows 7 %s",verInfo.szCSDVersion);
    if(verInfo.dwMajorVersion == 6 && verInfo.dwMinorVersion == 2)
        sprintf(buf, "Windows 8 %s", verInfo.szCSDVersion);
    if(verInfo.dwMajorVersion == 6 && verInfo.dwMinorVersion == 3)
        sprintf(buf, "Windows 8.1 %s", verInfo.szCSDVersion);
    if(verInfo.dwMajorVersion == 10 && verInfo.dwMinorVersion == 0)
        sprintf(buf, "Windows 10 %s", verInfo.szCSDVersion);

    return buf;
}

char* get_resolution()
{
    int width,height;

    width = GetSystemMetrics(SM_CXSCREEN);
    height = GetSystemMetrics(SM_CYSCREEN);

    char* res = (char*) malloc(128*sizeof(char));
    sprintf(res, "%ix%i", width, height);
    return res;
}

char* art()
{


    char* art = 
    "         ,.=:!!t3Z3z.,                  \n"
    "        :tt:::tt333EE3                  %s@%s\n"
    "        Et:::ztt33EEEL @Ee.,      ..,   CPU: %s\n"
    "       ;tt:::tt333EE7 ;EEEEEEttttt33#   Memory: %s/%s (used/total)\n"
    "      :Et:::zt333EEQ. $EEEEEttttt33QL   Uptime:  %s\n"
    "      it::::tt333EEF @EEEEEEttttt33F    GPU:     %s%s%s\n"
    "     ;3=*^```'*4EEV :EEEEEEttttt33@.    Disk:    %s/%s\n"
    "     ,.=::::!t=., ` @EEEEEEtttz33QF     Version: %s\n"
    "    ;::::::::zt33)   '4EEEtttji3P*      Resolution: %s\n"
    "   :t::::::::tt33.:Z3z..  `` ,..g.      \n"
    "   i::::::::zt33F AEEEtttt::::ztF       \n"
    "  ;:::::::::t33V ;EEEttttt::::t3        \n"
    "  E::::::::zt33L @EEEtttt::::z3F        \n"
    " {3=*^```'*4E3) ;EEEtttt:::::tZ`        \n"
    "              ` :EEEEtttt::::z7         \n"
    "                  'VEzjt:;;z>*`         \n";

    return art;

}