#ifndef __SHM_QUEUE_H__
#define __SHM_QUEUE_H__

#include <stddef.h>
#include <sys/types.h>

// one-read, one-write lock-free queue
class ShmQueue {
public:
    void Init(void *m, size_t size);

    void Destroy();

    ssize_t Read(void *array, size_t count);

    ssize_t Write(const void *array, size_t count);

private:
    class IMPL;
    IMPL *m_impl = NULL;
};

#endif
