#include "shm_queue.h"
#include "shm_helper.h"

#include <stdio.h>
#include <assert.h>
#include <unistd.h>

static const char *SHM_FILE = "/tmp/shm_queue_test.shm";
static size_t SHM_SIZE = 102400;

static void ShellCommand(const char *cmd) {
    FILE *f = popen(cmd, "r");
    if(f) {
        pclose(f);
    }
}

static void InitShmQueue(ShmQueue *q) {
    char cmd[1024];
    snprintf(cmd, sizeof(cmd), "mkdir -p %s", SHM_FILE);
    ShellCommand(cmd);

    void *sm = ShmAttach(SHM_FILE, 0, SHM_SIZE);
    assert(sm != NULL);

    q->Init(sm, SHM_SIZE);
}

static void ReadAndCheck(ShmQueue *q) {
    unsigned char v = 0;
    size_t total_read_nb = 0;

    while(q->Read(&v, 1) <= 0) {
        usleep(1000);
    }

    for(;;) {
        unsigned char buffer[1024];

        ssize_t read_nb = q->Read(buffer, sizeof(buffer));

        if(read_nb <= 0) {
            usleep(1000);
        }

        total_read_nb += read_nb;

        /*
        if(read_nb > 0){
            printf("total_read_nb=%zu\n", total_read_nb);
        }
        */

        for(ssize_t i = 0; i < read_nb; ++i) {
            if(buffer[i] != ++v) {
                printf("got error, stop\n");
                return;
            }
        }
    }
}

int main() {
    ShmQueue q;
    InitShmQueue(&q);

    ReadAndCheck(&q);

    return 0;
}
