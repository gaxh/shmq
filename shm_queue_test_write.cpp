#include "shm_queue.h"
#include "shm_helper.h"

#include <stdio.h>
#include <assert.h>
#include <unistd.h>

#include <random>

static const char *SHM_FILE = "/tmp/shm_queue_test.shm";
static size_t SHM_SIZE = 102400;

static std::mt19937 RNG;

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

static void WriteAndCheck(ShmQueue *q) {
    unsigned char v = 0;
    size_t total_written_nb = 0;

    // 一次写入随机个
    // 写满的话等待有空间了再继续写

    for(unsigned int n = 0;;++n) {
        size_t size = ((size_t)RNG() % (size_t)100) + 1;

        unsigned char buffer[size];

        for(size_t i = 0; i < size; ++i) {
            buffer[i] = v++;
        }

        size_t written_nb = 0;

        for(;;) {
            ssize_t written = q->Write(buffer + written_nb, size - written_nb);

            if(written <= 0) {
                usleep(1000);
                continue;
            }

            written_nb += written;

            total_written_nb += written;
            //printf("total_written_nb=%zu\n", total_written_nb);
            
            if(written_nb >= size) {
                break;
            }
        }
    }
}

int main() {
    RNG.seed(time(NULL));

    ShmQueue q;
    InitShmQueue(&q);

    WriteAndCheck(&q);

    return 0;
}
