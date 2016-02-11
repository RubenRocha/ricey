#ifdef _WIN32
#include "windows.h"
#elif __APPLE__
#include "osx.h"
#elif __linux__
#include "linux.h"
#elif __FreeBSD__
#include "bsd.h"
#endif

#include <stdio.h>
#include <iostream>
#include "functions.h"

using namespace std;

int main() {
    info _info;
    gather_info(&_info);
    char buf_art[1024];
    sprintf(buf_art, art(), _info.username, _info.hostname, trim(_info.cpu_brand), _info.used_system_memory, _info.total_system_memory, _info.uptime, trim(_info.gpu_model), _info.shell, _info.terminal, _info.used_disk, _info.total_disk, _info.os_version, _info.resolution);
    printf("%s", buf_art);
    return 0;
}
