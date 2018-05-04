#define ngx_signal_value(n)      ngx_signal_helper(n)
#define ngx_signal_helper(n)     SIG##n

#define ngx_value_helper(n)   #n
#define ngx_value(n)          ngx_value_helper(n)

/*
XCPU:超出设定的CPU时间限制
USR2:用户自定义信号2
*/

void
ngx_signal_handler(int signo)
{
    switch (ngx_process) {
    	    case NGX_PROCESS_MASTER:
    		case NGX_PROCESS_SINGLE:
	        switch (signo) {

	        case ngx_signal_value(NGX_CHANGEBIN_SIGNAL):
	            if (getppid() > 1 || ngx_new_binary > 0) {

	                /*
	                 * Ignore the signal in the new binary if its parent is
	                 * not the init process, i.e. the old binary's process
	                 * is still running.  Or ignore the signal in the old binary's
	                 * process if the new binary's process is already running.
	                 */

	                action = ", ignoring";
	                ignore = 1;
	                break;
	            }

	            //改变ngx_change_binary 值
	            ngx_change_binary = 1;
	            action = ", changing binary";
	            break;
			}    	
	}        
}

//设置signal action
ngx_int_t
ngx_init_signals(ngx_log_t *log)
{
    ngx_signal_t      *sig;
    struct sigaction   sa;

    //藏signals数组取数据
    for (sig = signals; sig->signo != 0; sig++) {
        ngx_memzero(&sa, sizeof(struct sigaction));
        sa.sa_handler = sig->handler;
        sigemptyset(&sa.sa_mask);
        if (sigaction(sig->signo, &sa, NULL) == -1) {
            ngx_log_error(NGX_LOG_EMERG, log, ngx_errno,
                          "sigaction(%s) failed", sig->signame);
            return NGX_ERROR;
        }
    }

    return NGX_OK;
}
