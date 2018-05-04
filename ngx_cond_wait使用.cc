static void
ngx_start_worker_processes(ngx_cycle_t *cycle, ngx_int_t n, ngx_int_t type)
{
    for (i = 0; i < n; i++) {
        cpu_affinity = ngx_get_cpu_affinity(i);

        ngx_spawn_process(cycle, ngx_worker_process_cycle, NULL,
                          "worker process", type);
	}
}

static void
ngx_worker_process_cycle(ngx_cycle_t *cycle, void *data)
{
	if (ngx_create_thread((ngx_tid_t *) &ngx_threads[n].tid,
                          ngx_worker_thread_cycle,
                          (void *) &ngx_threads[n], cycle->log)
        != 0)
    {
        /* fatal */
        exit(2);
    }
}

static ngx_thread_value_t
ngx_worker_thread_cycle(void *data)
{
	//加锁
	ngx_mutex_lock(ngx_posted_events_mutex);

    for ( ;; ) {
        thr->state = NGX_THREAD_FREE;

        //等待
        if (ngx_cond_wait(thr->cv, ngx_posted_events_mutex) == NGX_ERROR) {
            return (ngx_thread_value_t) 1;

		if (ngx_terminate) {
            thr->state = NGX_THREAD_EXIT;

            //解锁
            ngx_mutex_unlock(ngx_posted_events_mutex);

            ngx_log_debug1(NGX_LOG_DEBUG_CORE, cycle->log, 0,
                           "thread " NGX_TID_T_FMT " is done",
                           ngx_thread_self());

            return (ngx_thread_value_t) 0;
        }
	}

}
