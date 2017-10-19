/*
    Makefile
        obj-m	:= file_trace.o
    Build
        # make -C /lib/modules/`uname -r`/build M=`pwd` modules
 */
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/kprobes.h>
#include <linux/types.h>

/* kprobe object*/
static struct kprobe kp = {
    .symbol_name    = "sys_open",
};

static char *file = "/tmp/file_trace_test";
module_param(file, charp, 0400);
MODULE_PARM_DESC(file, "trace file name");

/* Call pre_handler before call probe symbol function. */
static int handler_pre(struct kprobe *p, struct pt_regs *regs)
{
    char *filename = regs->di;
    if (strcmp(filename, file) == 0) {
        /* 在这里可以调用内核接口函数dump_stack打印出栈的内容*/
        dump_stack();
    }

    return 0;
}

/* Call post_handler after call probe symbol function. */
static void handler_post(struct kprobe *p, struct pt_regs *regs,
                unsigned long flags)
{

}

/*
 * When pre-handler or post-handler cause error, it will call fault_handler
 */
static int handler_fault(struct kprobe *p, struct pt_regs *regs, int trapnr)
{
    /*
    printk(KERN_DEBUG "fault_handler: p->addr = 0x%p, trap #%dn",
        p->addr, trapnr);
	*/
    return 0;
}

static int __init kprobe_init(void)
{
    int ret;
    kp.pre_handler = handler_pre;
    kp.post_handler = handler_post;
    kp.fault_handler = handler_fault;

    ret = register_kprobe(&kp);
    if (ret < 0) {
        printk(KERN_DEBUG "register_kprobe failed, returned %d\n", ret);
        return ret;
    }
    printk(KERN_DEBUG "trace file name is %s\n", file);
    printk(KERN_DEBUG "Planted kprobe at %p\n", kp.addr);
    
    return 0;
}

static void __exit kprobe_exit(void)
{
    unregister_kprobe(&kp);
    printk(KERN_DEBUG "kprobe at %p unregistered\n", kp.addr);
}

module_init(kprobe_init)
module_exit(kprobe_exit)
MODULE_LICENSE("GPL");
