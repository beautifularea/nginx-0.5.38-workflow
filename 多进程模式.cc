/*
ngx 多进程模式
*/

//#define ngx_cdecl
int ngx_cdecl
main(int argc, char *const *argv)
{
	//初始化signal action
	if (ngx_init_signals(cycle->log) != NGX_OK) {
	    return 1;
	}

	//省略其他代码，参考相关模块
    if (ngx_process == NGX_PROCESS_MASTER) {
        ngx_master_process_cycle(cycle);

    } else {
        ngx_single_process_cycle(cycle);
    }

    return 0;
}

void
ngx_master_process_cycle(ngx_cycle_t *cycle)
{
	//创建worker
	ccf = (ngx_core_conf_t *) ngx_get_conf(cycle->conf_ctx, ngx_core_module);
    ngx_start_worker_processes(cycle, ccf->worker_processes,
                               NGX_PROCESS_RESPAWN);
    ngx_start_garbage_collector(cycle, NGX_PROCESS_RESPAWN);

	//进入主循环
	for ( ;; ) {
		if (ngx_new_binary) {
            ngx_start_worker_processes(cycle, ccf->worker_processes,
                                       NGX_PROCESS_RESPAWN);
            ngx_start_garbage_collector(cycle, NGX_PROCESS_RESPAWN);
            ngx_noaccepting = 0;

            continue;
        }

		/*
		sig_atomic_t ngx_change_binary
		原子类型，保证原子操作
		*/
		if (ngx_change_binary) {
            ngx_change_binary = 0;
            ngx_log_error(NGX_LOG_NOTICE, cycle->log, 0, "changing binary");
            ngx_new_binary = ngx_exec_new_binary(cycle, ngx_argv);
        }
	}
}

ngx_pid_t
ngx_exec_new_binary(ngx_cycle_t *cycle, char *const *argv)
{
	//其他代码参考相应的模块
    pid = ngx_execute(cycle, &ctx);
}

ngx_pid_t
ngx_execute(ngx_cycle_t *cycle, ngx_exec_ctx_t *ctx)
{
    return ngx_spawn_process(cycle, ngx_execute_proc, ctx, ctx->name,
                             NGX_PROCESS_DETACHED);
}

ngx_pid_t
ngx_spawn_process(ngx_cycle_t *cycle, ngx_spawn_proc_pt proc, void *data,
    char *name, ngx_int_t respawn)
{
    ngx_process_slot = s;

    pid = fork();

    switch (pid) {
    case 0:
        ngx_pid = ngx_getpid();
        proc(cycle, data);
        break;

    default:
        break;
    }

    return pid;
}
