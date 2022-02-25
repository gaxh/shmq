#include "shm_helper.h"

#include <stdio.h>
#include <assert.h>
#include <string.h>

static const char *SHM_FILE = "/tmp/shm_queue_test.shm";
static size_t SHM_SIZE = 102400;

static void ShellCommand(const char *cmd) {
    FILE *f = popen(cmd, "r");
    if(f) {
        pclose(f);
    }
}

int main() {
    char cmd[1024];
    snprintf(cmd, sizeof(cmd), "mkdir -p %s", SHM_FILE);
    ShellCommand(cmd);

    void *sm = ShmAttach(SHM_FILE, 0, SHM_SIZE);
    assert(sm != NULL);

    memset(sm, 0, SHM_SIZE);

    return 0;
}

