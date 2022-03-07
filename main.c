#include <SDL2/SDL.h>

#include "cute_sync.h"

#define PU_IMPLEMENTATION
#include "pico_unit.h"

#include "jobs.h"

cute_atomic_int_t job_called;
cute_atomic_int_t after_called;

static void setup()
{
    cute_atomic_set(&job_called, 0);
    cute_atomic_set(&after_called, 0);
}

static void job_cb(void* udata)
{
    (void)udata;
    cute_atomic_add(&job_called, 1);
}

static void after_cb(void* udata)
{
    (void)udata;
    cute_atomic_add(&after_called, 1);
}

static void call_after_cb(void* udata)
{
    (void)udata;
    jobs_run(job_cb, after_cb, NULL);
}

PU_TEST(test_jobs)
{
    jobs_run(job_cb, after_cb, NULL);

    int i = 0;
    while(i < 25)
    {
        jobs_poll();
        SDL_Delay(5);
        i++;
    }

    PU_ASSERT(cute_atomic_get(&job_called) == 1);
    PU_ASSERT(cute_atomic_get(&after_called) == 1);

    return true;
}

PU_TEST(test_nested_jobs)
{
    jobs_run(job_cb, call_after_cb, NULL);

    int i = 0;
    while(i < 25)
    {
        jobs_poll();
        SDL_Delay(5);
        i++;
    }

    PU_ASSERT(cute_atomic_get(&job_called) == 2);;
    PU_ASSERT(cute_atomic_get(&after_called) == 1);

    return true;
}

int main(int argc, char* argv[])
{
    (void)argc;
    (void)argv;

    jobs_init(4);

    pu_display_colors(true);
    pu_setup(setup, NULL);
    PU_RUN_TEST(test_jobs);
    PU_RUN_TEST(test_nested_jobs);
    pu_clear_setup();
    pu_print_stats();

    return 0;
}

