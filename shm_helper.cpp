#include "shm_helper.h"

#include <stdio.h>
#include <errno.h>

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define SHM_HELPER_ERROR(fmt, args...) fprintf(stderr, "[%s:%d:%s]" fmt "\n", __FILE__, __LINE__, __FUNCTION__, ##args)

void *ShmAttach(const char *path, int id, size_t size) {
    key_t k = ftok(path, id);

    if(k == -1) {
        SHM_HELPER_ERROR("ftok failed, path=%s, id=%d, size=%zu, errno=%d", path, id, size, errno);
        return NULL;
    }

    int sid = shmget(k, size, 0666 | IPC_CREAT);

    if(sid == -1) {
        SHM_HELPER_ERROR("shmget failed, path=%s, id=%d, size=%zu, errno=%d", path, id, size, errno);
        return NULL;
    }

    void *p = shmat(sid, NULL, 0);

    if(p == (void *)-1) {
        SHM_HELPER_ERROR("shmat failed, path=%s, id=%d, size=%zu, errno=%d", path, id, size, errno);
        return NULL;
    }

    return p;
}

void ShmDetach(const void *p) {
    if(shmdt(p) == -1) {
        SHM_HELPER_ERROR("shmdt failed, p=%p, errno=%d", p, errno);
    }
}

void *ShmCreateOnly(const char *path, int id, size_t size) {
    key_t k = ftok(path, id);

    if(k == -1) {
        SHM_HELPER_ERROR("ftok failed, path=%s, id=%d, size=%zu, errno=%d", path, id, size, errno);
        return NULL;
    }

    int sid = shmget(k, size, 0666 | IPC_CREAT | IPC_EXCL);

    if(sid == -1) {
        SHM_HELPER_ERROR("shmget failed, path=%s, id=%d, size=%zu, errno=%d", path, id, size, errno);
        return NULL;
    }

    void *p = shmat(sid, NULL, 0);

    if(p == (void *)-1) {
        SHM_HELPER_ERROR("shmat failed, path=%s, id=%d, size=%zu, errno=%d", path, id, size, errno);
        return NULL;
    }

    return p;
}

void *ShmAttachOnly(const char *path, int id, size_t size) {
    key_t k = ftok(path, id);

    if(k == -1) {
        SHM_HELPER_ERROR("ftok failed, path=%s, id=%d, size=%zu, errno=%d", path, id, size, errno);
        return NULL;
    }

    int sid = shmget(k, size, 0666);

    if(sid == -1) {
        SHM_HELPER_ERROR("shmget failed, path=%s, id=%d, size=%zu, errno=%d", path, id, size, errno);
        return NULL;
    }

    void *p = shmat(sid, NULL, 0);

    if(p == (void *)-1) {
        SHM_HELPER_ERROR("shmat failed, path=%s, id=%d, size=%zu, errno=%d", path, id, size, errno);
        return NULL;
    }

    return p;
}


