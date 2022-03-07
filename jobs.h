#ifndef JOBS_H
#define JOBS_H

#include <stdbool.h>
#include <stddef.h>

typedef void (*job_fn)(void* udata);

bool jobs_init(int thread_count);
void jobs_poll();
void jobs_sync();
void jobs_kick();
int jobs_core_count();
int jobs_thread_count();
void jobs_run(job_fn job_cb, job_fn after_cb, void* udata);

#endif // JOBS_H
