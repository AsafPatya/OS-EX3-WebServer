#include "pool.h"

pool create_new_pool(int threads_nums)
{
    pool pool = malloc(sizeof pool);
    if (pool == NULL)
    {
        return NULL;
    }
    pool->running_threads = 0;
    pool->threads_nums = threads_nums;

}

void insert_to_pull(pool pool)
{

}

void delete_from_pool(pool pool)
{

}