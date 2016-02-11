#ifndef H_L
#define H_L

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
	char* get_resolution();
	char* art();

#endif
