#ifndef __SHM_HELPER_H__
#define __SHM_HELPER_H__

#include <stddef.h>

void *ShmAttach(const char *path, int id, size_t size);
void ShmDetach(const void *p);

void *ShmCreateOnly(const char *path, int id, size_t size);
void *ShmAttachOnly(const char *path, int id, size_t size);

#endif
