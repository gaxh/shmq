#include "shm_queue.h"

#include <assert.h>
#include <string.h>
#include <algorithm>

#define SHM_QUEUE_AVAILABLE_SIZE(cap, used)   ((cap) - 1 - (used))
#define SHM_QUEUE_USED_SIZE(begin_idx, end_idx, cap)    ( (end_idx) >= (begin_idx) ? ((end_idx) - (begin_idx)) : ((end_idx) + (cap) - (begin_idx)) )

class ShmQueue::IMPL {
public:
    void Init(void *m, size_t size) {
        assert(size > sizeof(SHM_QUEUE));

        m_mem = m;
        m_size = size;
        m_q = (SHM_QUEUE *) m;
        m_qcap = size - sizeof(SHM_QUEUE);
    }

    void Destroy() {
    
    }

    ssize_t Read(void *array, size_t count) {
        size_t begin_index = m_q->BEGIN_INDEX;
        size_t end_index = m_q->END_INDEX;
        size_t size = SHM_QUEUE_USED_SIZE(begin_index, end_index, m_qcap);
        size_t read_count = std::min(count, size);

        size_t tail_count = std::min(read_count, m_qcap - begin_index);
        memcpy(array, m_q->QUEUE + begin_index, tail_count);
        if(read_count > tail_count) {
            memcpy((unsigned char *)array + tail_count, m_q->QUEUE, read_count - tail_count);
        }

        __sync_synchronize();

        m_q->BEGIN_INDEX = (begin_index + read_count) % m_qcap;
        
        return read_count;
    }

    ssize_t Write(const void *array, size_t count) {
        size_t begin_index = m_q->BEGIN_INDEX;
        size_t end_index = m_q->END_INDEX;
        size_t available = SHM_QUEUE_AVAILABLE_SIZE(m_qcap, SHM_QUEUE_USED_SIZE(begin_index, end_index, m_qcap));
        size_t write_count = std::min(count, available);

        size_t append_count = std::min(write_count, m_qcap - end_index);
        memcpy(m_q->QUEUE + end_index, array, append_count);
        if(write_count > append_count){
            memcpy(m_q->QUEUE, (const unsigned char *)array + append_count, write_count - append_count);
        }

        __sync_synchronize();

        m_q->END_INDEX = (end_index + write_count) % m_qcap;

        return write_count;
    }

private:
    void *m_mem = NULL;
    size_t m_size = 0;

    struct SHM_QUEUE {
        size_t BEGIN_INDEX;
        size_t END_INDEX;
        unsigned char QUEUE[0];
    };

    SHM_QUEUE *m_q = NULL;
    size_t m_qcap = 0;
};

void ShmQueue::Init(void *m, size_t size) {
    if(m_impl == NULL) {
        m_impl = new IMPL();
        m_impl->Init(m, size);
    }
}

void ShmQueue::Destroy() {
    if(m_impl) {
        m_impl->Destroy();
        delete m_impl;
        m_impl = NULL;
    }
}

ssize_t ShmQueue::Read(void *array, size_t count) {
    return m_impl->Read(array, count);
}

ssize_t ShmQueue::Write(const void *array, size_t count) {
    return m_impl->Write(array, count);
}



