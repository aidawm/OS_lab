#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/list.h>
#include <linux/slab.h>

struct birthday{
        int day;
        int month;
        int year;
        struct list_head list;
};


static LIST_HEAD(birthday_list);

static void add_node (int day , int month , int year){
        
        struct birthday *person;
        person = kmalloc(sizeof(person), GFP_KERNEL);
        person->day = day;
        person->month = month;
        person->year = year;


        INIT_LIST_HEAD(&person->list);
        list_add_tail(&person->list,&birthday_list);

}

static void itterate_on_list(void){
        struct birthday *ptr;
        int cnt=1;
        list_for_each_entry(ptr,&birthday_list,list){
                printk(KERN_INFO "person %i = %i,%i,%i",cnt,ptr->year , ptr->month , ptr->day);
	cnt++;
        }
}

int simple_init(void)
{
	printk(KERN_INFO "_________birthday list_________");
	add_node(1,1,1380);
        add_node(2,5,1375);
        add_node(10,10,1390);
	itterate_on_list();
	printk(KERN_INFO "_______________________________");

	printk(KERN_INFO "_______________________________");

	return 0;
}
void simple_exit(void)
{
    printk(KERN_INFO "Removing Module\n");
}


module_init(simple_init);
module_exit(simple_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("simple module");
MODULE_AUTHOR("SGG");
