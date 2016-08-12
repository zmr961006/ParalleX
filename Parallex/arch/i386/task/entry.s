

;内核线程统一入口方式

[GLOBAL kthread_entry]
[EXTERN do_exit]

kthread_entry:

        push edx       ;push args
        call ebx       ;call fn
        push eax       ;get fn return code
        call do_exit
