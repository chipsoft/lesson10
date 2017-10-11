#include <linux/init.h>
#include <linux/module.h>
#include <linux/printk.h>
#include <linux/kthread.h>
#include <linux/delay.h>
#include <linux/rwsem.h>

#define MAX_LIST_COUNT 100000

MODULE_AUTHOR("Denis Suprunenko <chipsoft@gmail.com");
MODULE_DESCRIPTION("Thread demo, Module 10");
MODULE_LICENSE("Dual BSD/GPL");

static struct rw_semaphore mr_rwsem;

struct stDataForChange {
	u32 a, b, c, d, e, f, g, h;
};

static struct stDataForChange dataForChange;

static void myDelay(u32 divider)
{
	if (!(divider % 100)) {
		mdelay(1);
		schedule();
	}
}

static int changeData(void *data)
{
	u32 i;
	for(i = 0; i < MAX_LIST_COUNT; ++i) {
		down_write(&mr_rwsem);
		dataForChange.a++;
		dataForChange.b++;
		dataForChange.c++;
		dataForChange.d++;
		dataForChange.e++;
		dataForChange.f++;
		dataForChange.g++;
		dataForChange.h++;
		up_write(&mr_rwsem);
		myDelay(i);
	}
	return 0;
}

static int showData(void* data)
{
	down_read(&mr_rwsem);
	printk(KERN_INFO "Enter\n");
	printk(KERN_INFO "Data: a = %d, b = %d, c = %d, d = %d, e = %d, f = %d, g = %d, h = %d\n", dataForChange.a, dataForChange.b, dataForChange.c, dataForChange.d, dataForChange.e, dataForChange.f, dataForChange.g, dataForChange.h);
	mdelay(5);
	schedule();
	up_read(&mr_rwsem);
	printk(KERN_INFO "Exit\n");
	return 0;
}

static int __init hello_init(void)
{
	printk(KERN_INFO "-> Module 10 load!\n");
	init_rwsem(&mr_rwsem);
	kthread_run(changeData, NULL, "changeData");
	kthread_run(showData, NULL, "showData1");
	kthread_run(showData, NULL, "showData2");
	kthread_run(showData, NULL, "showData3");
	return 0;
}

static void __exit hello_exit(void)
{
	printk(KERN_INFO "<- Module 10 unload!\n");
}

module_init(hello_init);
module_exit(hello_exit);
