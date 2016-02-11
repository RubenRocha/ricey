#ifndef H_B
#define H_B
    char* get_cpu_brand();
    float get_total_system_memory();
    float get_used_system_memory();
    char* get_hostname();
    float get_uptime();
    char* get_gpu_model();
    char* get_shell();
    char* get_terminal();
    void get_disk_usage(float _disk[2]);
    char* get_username();
    char* get_os_version();
    char* art();
    char* get_resolution();
#endif