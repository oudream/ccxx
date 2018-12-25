#include <mach/mach_host.h>
#include <stdio.h>
#include <err.h>
#include <math.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    mach_msg_type_number_t count = HOST_CPU_LOAD_INFO_COUNT;
    mach_port_t port;
    kern_return_t kr;
    host_cpu_load_info_data_t cpuStats, cpuStats2;

    port = mach_host_self();
    kr = host_statistics(port, HOST_CPU_LOAD_INFO, (host_info_t)&cpuStats, &count);
    if (kr != KERN_SUCCESS) {
        err(-1, "error from host_statistics(): %d", kr);
    }

    usleep(500000);
    kr = host_statistics(port, HOST_CPU_LOAD_INFO, (host_info_t)&cpuStats2, &count);
    if (kr != KERN_SUCCESS) {
        err(-1, "error from host_statistics(): %d", kr);
    }

    natural_t user = cpuStats2.cpu_ticks[CPU_STATE_USER] - cpuStats.cpu_ticks[CPU_STATE_USER];
    natural_t system = cpuStats2.cpu_ticks[CPU_STATE_SYSTEM] - cpuStats.cpu_ticks[CPU_STATE_SYSTEM];
    natural_t idle = cpuStats2.cpu_ticks[CPU_STATE_IDLE] - cpuStats.cpu_ticks[CPU_STATE_IDLE];

    double usage = round((double)(user + system) / (system + user + idle) * 100.0);


    if(usage>=50)
        printf("%0.0f%%|color=red",usage);
    else if(usage>=24)
        printf("%0.0f%%|color",usage);
    else
        printf("OK\n");

    return 0;
}