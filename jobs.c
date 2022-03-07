#include "array.h"
#include "jobs.h"

#include <SDL2/SDL.h>
#include <pthread.h>

#define CUTE_SYNC_IMPLEMENTATION
//#define CUTE_SYNC_SDL
//#define CUTE_SYNC_POSIX
#include "cute_sync.h"

#include <assert.h>
#include <stdlib.h>

static struct
{
    int thread_count;
    cute_threadpool_t* pool;
    cute_mutex_t lock;
    array_t* jobs;
} job_sys;

typedef struct
{
    cute_atomic_int_t done;
    job_fn job_cb;
    job_fn after_cb;
    void* udata;
} job_t;

bool jobs_init(int thread_count)
{
    job_sys.thread_count = thread_count;
    job_sys.pool = cute_threadpool_create(thread_count, NULL);
    job_sys.jobs = array_new(16, sizeof(job_t*));
    job_sys.lock = cute_mutex_create();
    return true; // TODO: error handling
}

void jobs_poll()
{
    cute_lock(&job_sys.lock);

    array_t* jobs = job_sys.jobs;

    size_t i = 0;

    while (i < array_size(jobs))
    {
        job_t** job = array_get(jobs, i);

        int done = cute_atomic_get(&(*job)->done);

        if (done)
        {
            cute_unlock(&job_sys.lock);

            if ((*job)->after_cb)
                (*job)->after_cb((*job)->udata);

            cute_lock(&job_sys.lock);

            free(*job);
            array_swap_remove(jobs, i);
        }
        else
        {
            i++;
        }
    }

    cute_unlock(&job_sys.lock);
}

void jobs_sync()
{
    cute_threadpool_kick_and_wait(job_sys.pool);
}

int jobs_core_count()
{
    return cute_core_count();
}

int jobs_thread_count()
{
    return job_sys.thread_count;
}

void job_runner(void* udata)
{
    job_t* job = udata;
    job->job_cb(job->udata);
    cute_atomic_set(&job->done, 1);
}

void jobs_run(job_fn job_cb, job_fn after_cb, void* udata)
{
    job_t* job = malloc(sizeof(job_t));
    job->job_cb = job_cb;
    job->after_cb = after_cb;
    job->udata = udata;
    cute_atomic_set(&job->done, 0);

    cute_lock(&job_sys.lock);

    array_t* jobs = job_sys.jobs;
    array_push(jobs, &job);

    cute_threadpool_add_task(job_sys.pool, job_runner, job);
    cute_threadpool_kick(job_sys.pool);

    cute_unlock(&job_sys.lock);
}
