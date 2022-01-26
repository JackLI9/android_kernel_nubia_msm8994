/* Copyright (c) 2012-2013, The Linux Foundation. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/device.h>
#include <linux/init.h>
#include <linux/slab.h>
#include <linux/err.h>
#include <linux/spinlock.h>
#include <linux/of.h>
#include <linux/of_device.h>
#include <linux/delay.h>
#include <linux/spinlock.h>
#include <linux/ztemt_hw_version.h>

//#define CONFIG_ZTEMT_HW_VERSION_DEBUG
//#ifdef CONFIG_ZTEMT_HW_VERSION_DEBUG
//#define ztemt_hw_version_debug(fmt, args...) printk(KERN_DEBUG "[ztemt_hw_version_debug]"fmt, ##args)
//#else
//#define ztemt_hw_version_debug(fmt, args...) do {} while(0)
//#endif

//#define CONFIG_ZTEMT_HW_VERSION_DEBUG
#ifdef CONFIG_ZTEMT_HW_VERSION_DEBUG
static int debug_value=1;
#else
static int debug_value=0;
#endif
#define ztemt_hw_version_debug(fmt, args...) do {if(debug_value==1)printk(KERN_DEBUG "[ztemt_hw_version]"fmt, ##args);} while(0)


#ifdef CONFIG_ZTEMT_HW_VERSION_NX601J
static const struct hardware_id_map_st hardware_id_map[] = {
	{0, 200,NX601J_HW_A,"ZTEMT_NX601J_A"},  //id_mv=0
	{700, 1100,NX601J_HW_B,"ZTEMT_NX601J_B"},  //id_mv=900
};
#elif defined CONFIG_ZTEMT_HW_VERSION_NX504J
static const struct hardware_id_map_st hardware_id_map[] = {
	{0, 200,NX504J_HW_A,"NX504JMB_A"},  //id_mv=0
	{200, 600,NX504J_HW_B,"NX504JMB_B"},  //id_mv=416
	{600, 900,NX504J_HW_C,"NX504JMB_C"},  //id_mv=720
	{900, 1300,NX504J_HW_D,"NX504JMB_D"},  //id_mv=1120
	{1300, 1650,NX504J_HW_E,"NX504JMB_E"},  //id_mv=1475
	{1650, 1800,NX504J_HW_F,"NX504JMB_F"},  //id_mv=1800
};
#elif defined CONFIG_ZTEMT_HW_VERSION_NX505J
static const struct hardware_id_map_st hardware_id_map[] = {
	{0, 300,NX505J_HW_A,"NX505JMB_A"},	//id_mv=0	
	{300, 600,NX505J_HW_B,"NX505JMB_B"},  //id_mv=544
	{600, 900,NX505J_HW_C,"NX505JMB_C"},  //id_mv=720
	{900, 1200,NX505J_HW_D,"NX505JMB_D"},  //id_mv=1120
	{1200, 1500,NX505J_HW_E,"NX505JMB_E"},	//id_mv=1475
	{1500, 1800,NX505J_HW_F,"NX505JMB_F"},	//id_mv=1800	
};
#elif defined CONFIG_ZTEMT_HW_VERSION_NX506J
static const struct hardware_id_map_st hardware_id_map[] = {
	{0, 300,NX506J_HW_A,"NX506JMB_A"},  //id_mv=0	
	{300, 600,NX506J_HW_B,"NX506JMB_B"},  //id_mv=544
	{600, 900,NX506J_HW_C,"NX506JMB_C"},  //id_mv=720
	{900, 1200,NX506J_HW_D,"NX506JMB_D"},  //id_mv=1120
	{1200, 1500,NX506J_HW_E,"NX506JMB_E"},  //id_mv=1475
	{1500, 1800,NX506J_HW_F,"NX506JMB_F"},  //id_mv=1800
};
#elif defined CONFIG_ZTEMT_HW_VERSION_NX507J
static const struct hardware_id_map_st hardware_id_map[] = {
	{0,    200,   0,    200,  NX507J_HW_A,"NX507JMB_A","SC"},  //id_mv=9     id_mv_2=9 
	{1650, 1900,  0,    200,  NX507J_HW_B,"NX507JMB_B","SC"},  //id_mv=1786  id_mv_2=9
	{200,  550,   0,    200,  NX507J_HW_C,"NX507JMB_C","SC"},  //id_mv=416   id_mv_2=9
	{1650, 1900,  1650, 1900, NX507J_HW_D,"NX507JMB_B","JD"},  //id_mv=1786  id_mv_2=1786
	{200,  550,   1650, 1900, NX507J_HW_E,"NX507JMB_C","JD"},  //id_mv=416   id_mv_2=1786
	{1650, 1900,  600,  850,  NX507J_HW_F,"NX507JMB_B","CU"},  //id_mv=1786   id_mv_2=724
};
#elif defined CONFIG_ZTEMT_HW_VERSION_NX508J
//ADC for hw_version , GPIO for wifi
static const struct hardware_id_map_st hardware_id_map[] = {
	{0,    300,  NX508J_HW_A, ZTE_GPIO_FLOAT,    ZTE_GPIO_FLOAT,    ZTE_GPIO_FLOAT,"NX508JMB_A","wifi_samsung","SC"},  //id_mv=0
	{300,  600,  NX508J_HW_B, ZTE_GPIO_FLOAT,    ZTE_GPIO_FLOAT,    ZTE_GPIO_FLOAT,"NX508JMB_B","wifi_samsung","SC"},  //id_mv=544
	{300,  600,  NX508J_HW_B, ZTE_GPIO_PULL_UP,  ZTE_GPIO_FLOAT,    ZTE_GPIO_FLOAT,"NX508JMB_B","wifi_samsung","HC"},  //id_mv=544  (high config 64G  4G)
	{300,  600,  NX508J_HW_B, ZTE_GPIO_FLOAT,    ZTE_GPIO_PULL_UP,  ZTE_GPIO_FLOAT,"NX508JMB_B","wifi_epcos",  "SC"},  //id_mv=544
	{300,  600,  NX508J_HW_B, ZTE_GPIO_PULL_UP,  ZTE_GPIO_PULL_UP,  ZTE_GPIO_FLOAT,"NX508JMB_B","wifi_epcos",  "HC"},  //id_mv=544 (high config 64G  4G)
	{600,  900,  NX508J_HW_C, ZTE_GPIO_FLOAT,    ZTE_GPIO_FLOAT,    ZTE_GPIO_FLOAT,"NX508JMB_C","111",         "SC"},  //id_mv=720
	{900,  1200, NX508J_HW_D, ZTE_GPIO_FLOAT,    ZTE_GPIO_FLOAT,    ZTE_GPIO_FLOAT,"NX508JMB_D","111",         "SC"},  //id_mv=1120
	{1200, 1500, NX508J_HW_E, ZTE_GPIO_FLOAT,    ZTE_GPIO_FLOAT,    ZTE_GPIO_FLOAT,"NX508JMB_E","111",         "SC"},  //id_mv=1475
	{1500, 1800, NX508J_HW_F, ZTE_GPIO_FLOAT,    ZTE_GPIO_FLOAT,    ZTE_GPIO_FLOAT,"NX508JMB_F","111",         "SC"},  //id_mv=1800
};
#elif defined CONFIG_ZTEMT_HW_VERSION_NX508H
//ADC for hw_version , GPIO for wifi
static const struct hardware_id_map_st hardware_id_map[] = {
	{0,    300,  NX508H_HW_A, ZTE_GPIO_FLOAT,    ZTE_GPIO_FLOAT,    ZTE_GPIO_FLOAT,"NX508HMB_A","wifi_samsung","SC"},  //id_mv=0
	{300,  600,  NX508H_HW_B, ZTE_GPIO_FLOAT,    ZTE_GPIO_FLOAT,    ZTE_GPIO_FLOAT,"NX508HMB_B","wifi_samsung","SC"},  //id_mv=544
	{300,  600,  NX508H_HW_B, ZTE_GPIO_PULL_UP,  ZTE_GPIO_FLOAT,    ZTE_GPIO_FLOAT,"NX508HMB_B","wifi_samsung","HC"},  //id_mv=544 (high config 64G  4G)
	{300,  600,  NX508H_HW_B, ZTE_GPIO_FLOAT,    ZTE_GPIO_PULL_UP,  ZTE_GPIO_FLOAT,"NX508HMB_B","wifi_epcos",  "SC"},  //id_mv=544
	{300,  600,  NX508H_HW_B, ZTE_GPIO_PULL_UP,  ZTE_GPIO_PULL_UP,  ZTE_GPIO_FLOAT,"NX508HMB_B","wifi_epcos",  "HC"},  //id_mv=544 (high config 64G  4G)
	{600,  900,  NX508H_HW_C, ZTE_GPIO_FLOAT,    ZTE_GPIO_FLOAT,    ZTE_GPIO_FLOAT,"NX508HMB_C","111",         "SC"},  //id_mv=720
	{900,  1200, NX508H_HW_D, ZTE_GPIO_FLOAT,    ZTE_GPIO_FLOAT,    ZTE_GPIO_FLOAT,"NX508HMB_D","111",         "SC"},  //id_mv=1120
	{1200, 1500, NX508H_HW_E, ZTE_GPIO_FLOAT,    ZTE_GPIO_FLOAT,    ZTE_GPIO_FLOAT,"NX508HMB_E","111",         "SC"},  //id_mv=1475
	{1500, 1800, NX508H_HW_F, ZTE_GPIO_FLOAT,    ZTE_GPIO_FLOAT,    ZTE_GPIO_FLOAT,"NX508HMB_F","111",         "SC"},  //id_mv=1800
};
#elif defined CONFIG_ZTEMT_HW_VERSION_NX509J
static const struct hardware_id_map_st hardware_id_map[] = {
	{0, 300,NX509J_HW_A,"NX509JMB_A"},	//id_mv=0	
	{300, 600,NX509J_HW_B,"NX509JMB_B"},  //id_mv=544
	{600, 900,NX509J_HW_C,"NX509JMB_C"},  //id_mv=720
	{900, 1200,NX509J_HW_D,"NX509JMB_D"},  //id_mv=1120
	{1200, 1500,NX509J_HW_E,"NX509JMB_E"},	//id_mv=1475
	{1500, 1800,NX509J_HW_F,"NX509JMB_F"},	//id_mv=1800	
};
#elif defined CONFIG_ZTEMT_HW_VERSION_NX510J
//GPIO for hw_version , ADC for wifi
static const struct hardware_id_map_st hardware_id_map[] = {
	{0,    80,  NX510J_HW_A, ZTE_GPIO_PULL_UP,   ZTE_GPIO_FLOAT,    ZTE_GPIO_FLOAT,  "NX510JMB_A","wifi_samsung"},
	{0,    80,  NX510J_HW_B, ZTE_GPIO_PULL_DOWN, ZTE_GPIO_FLOAT,    ZTE_GPIO_FLOAT,  "NX510JMB_B","wifi_samsung"},
	{0,    80,  NX510J_HW_C, ZTE_GPIO_PULL_DOWN, ZTE_GPIO_PULL_DOWN,ZTE_GPIO_FLOAT,  "NX510JMB_C","wifi_samsung"},
	{80,   220, NX510J_HW_C, ZTE_GPIO_PULL_DOWN, ZTE_GPIO_PULL_DOWN,ZTE_GPIO_FLOAT,  "NX510JMB_C","wifi_epcos"},
};
#else
static const struct hardware_id_map_st hardware_id_map[] = {
    {0, 300,HW_A,"ZTEMT_UN_A"},	//id_mv=0	
	{300, 600,HW_B,"ZTEMT_UN_B"},  //id_mv=544
	{600, 900,HW_C,"ZTEMT_UN_C"},  //id_mv=720
	{900, 1200,HW_D,"ZTEMT_UN_D"},  //id_mv=1120
	{1200, 1500,HW_E,"ZTEMT_UN_E"},	//id_mv=1475
	{1500, 1800,HW_F,"ZTEMT_UN_F"},	//id_mv=1800
};
#endif

static int ztemt_hw_id = -1;
static int ztemt_hw_mv = 900;
static int ztemt_hw_mv_2 = 900;
char board_type_gpio_str[100]="0,0,0";

static int  ztemt_board_type_setup(char *param)
{
	int magic_num = 0;
    get_option(&param, &magic_num);
	ztemt_hw_mv = magic_num;
    return 0;
}
early_param("board_type", ztemt_board_type_setup);

static int  ztemt_board_type_setup_2(char *param)
{
	int magic_num = 0;
    get_option(&param, &magic_num);
	ztemt_hw_mv_2 = magic_num;
    return 0;
}
early_param("board_type_2", ztemt_board_type_setup_2);


static int  ztemt_board_type_setup_gpio(char *param)
{
	//printk("%s=%s",__func__, param);
    memcpy(board_type_gpio_str, param, strlen(param));
    return 0;
}
early_param("board_type_gpio", ztemt_board_type_setup_gpio);


#ifdef CONFIG_ZTEMT_HW_VERSION_NX510J
static int32_t ztemt_get_hardware_type_gpio(const struct hardware_id_map_st *pts,
		uint32_t tablesize, char * gpio_str)
{
	uint32_t i = 0;
    char * gpio_str_temp=gpio_str;
	int gpio_value_A,gpio_value_B,gpio_value_C;

	//printk("%s=%s\n",__func__, gpio_str_temp);
    sscanf(gpio_str_temp ,"%d,%d,%d", &gpio_value_A, &gpio_value_B, &gpio_value_C);

	if ( pts == NULL )
		return -EINVAL;

	while (i < tablesize) {
		if ( (pts[i].gpio_A == gpio_value_A) && (pts[i].gpio_B == gpio_value_B) && (pts[i].gpio_C == gpio_value_C) ) 
			break;
		else
			i++;
	}

	if ( i < tablesize )
		return pts[i].hw_type;
	else
		return HW_UN;

}
#elif defined CONFIG_ZTEMT_HW_VERSION_NX507J
static int32_t ztemt_get_hardware_type_2(const struct hardware_id_map_st *pts,
		uint32_t tablesize, int input, int input_2)
{
	uint32_t i = 0;

	if ( pts == NULL )
		return -EINVAL;

	while (i < tablesize) {
		if ( (pts[i].low_mv <= input) && (input <= pts[i].high_mv) && (pts[i].low_mv_2 <= input_2) && (input_2 <= pts[i].high_mv_2)) 
			break;
		else 
			i++;
	}

	if ( i < tablesize ) 
		return pts[i].hw_type;
    else 
		return HW_UN;
}
#else
static int32_t ztemt_get_hardware_type(const struct hardware_id_map_st *pts,
		uint32_t tablesize, int input)
{
	uint32_t i = 0;

	if ( pts == NULL )
		return -EINVAL;

	while (i < tablesize) {
		if ( (pts[i].low_mv <= input) && (input <= pts[i].high_mv) ) 
			break;
		else 
			i++;
	}

	if ( i < tablesize ) 
		return pts[i].hw_type;
    else 
		return HW_UN;
}
#endif

int ztemt_get_hw_id(void)
{
	if(ztemt_hw_id >= 0)
	    return ztemt_hw_id;

#ifdef CONFIG_ZTEMT_HW_VERSION_NX510J
    ztemt_hw_id = ztemt_get_hardware_type_gpio(
                        hardware_id_map,
                        ARRAY_SIZE(hardware_id_map),
                        board_type_gpio_str);
    ztemt_hw_version_debug("hw_id=%d",ztemt_hw_id);

#elif defined CONFIG_ZTEMT_HW_VERSION_NX507J
    ztemt_hw_id = ztemt_get_hardware_type_2(
		                hardware_id_map,
						ARRAY_SIZE(hardware_id_map),
						ztemt_hw_mv,ztemt_hw_mv_2);
	
    ztemt_hw_version_debug("hw_id_mv=%d mv , hw_id_mv_2=%d mv ,hw_id=%d ,hw_ver=%s\n",
         ztemt_hw_mv,ztemt_hw_mv_2,ztemt_hw_id,hardware_id_map[ztemt_hw_id].hw_ver);
#else	    
    ztemt_hw_id = ztemt_get_hardware_type(
		                hardware_id_map,
						ARRAY_SIZE(hardware_id_map),
						ztemt_hw_mv);
	
	//printk("hw_id_mv=%d mv hw_id=%d hw_ver=%s\n",
	    //ztemt_hw_mv,ztemt_hw_id,hardware_id_map[ztemt_hw_id].hw_ver);
    ztemt_hw_version_debug("hw_id_mv=%d mv hw_id=%d hw_ver=%s\n",
		    ztemt_hw_mv,ztemt_hw_id,hardware_id_map[ztemt_hw_id].hw_ver);
#endif

	return ztemt_hw_id;
}

EXPORT_SYMBOL_GPL(ztemt_get_hw_id);


#if defined CONFIG_ZTEMT_HW_VERSION_NX508J || defined CONFIG_ZTEMT_HW_VERSION_NX508H
const char* ztemt_get_hw_wifi(void)
{
	uint32_t tablesize;
	int hw_type;
	uint32_t i = 0;
	char * gpio_str_temp=board_type_gpio_str;
	int gpio_value_A,gpio_value_B,gpio_value_C;
	const struct hardware_id_map_st *pts= hardware_id_map;

	if ( pts == NULL )
		return NULL;

	tablesize=sizeof(hardware_id_map)/sizeof(hardware_id_map[0]);
	ztemt_hw_version_debug("tablesize=%d\n",tablesize);

    hw_type=ztemt_get_hw_id();

	ztemt_hw_version_debug("%s=%s\n",__func__, gpio_str_temp);
    sscanf(gpio_str_temp ,"%d,%d,%d", &gpio_value_A, &gpio_value_B, &gpio_value_C);

	while (i < tablesize) {
		if ((pts[i].hw_type == hw_type) &&
			(pts[i].gpio_A == gpio_value_A) && (pts[i].gpio_B == gpio_value_B) && (pts[i].gpio_C == gpio_value_C))
			break;
		else
			i++;
	}

	//ztemt_hw_version_debug("i=%d, hw_wifi=%s\n", i,pts[i].hw_wifi);
	if ( i < tablesize ){
		ztemt_hw_version_debug("hw_wifi=%s\n", pts[i].hw_wifi);
		return pts[i].hw_wifi;
	}
	else
		return "unknow";
}
EXPORT_SYMBOL_GPL(ztemt_get_hw_wifi);

#elif defined CONFIG_ZTEMT_HW_VERSION_NX510J
const char* ztemt_get_hw_wifi(void)
{
	uint32_t tablesize;
	int hw_type;
	uint32_t i = 0;
	const struct hardware_id_map_st *pts= hardware_id_map;

	if ( pts == NULL )
	    return NULL;

	tablesize=sizeof(hardware_id_map)/sizeof(hardware_id_map[0]);
	ztemt_hw_version_debug("tablesize=%d\n",tablesize);

    hw_type=ztemt_get_hw_id();

	ztemt_hw_version_debug("ztemt_hw_mv=%d\n",ztemt_hw_mv);
	while (i < tablesize) {
		if ((pts[i].hw_type == hw_type) &&
		     (pts[i].low_mv <= ztemt_hw_mv) && (ztemt_hw_mv <= pts[i].high_mv))
			break;
		else
			i++;
	}

	if ( i < tablesize ){
		ztemt_hw_version_debug("hw_wifi=%s\n", pts[i].hw_wifi);
		return pts[i].hw_wifi;
	}
	else
		return "unknow";
}
EXPORT_SYMBOL_GPL(ztemt_get_hw_wifi);
#endif

void ztemt_get_hw_version(char* result)
{
    int hw_id;
    if(!result)
		return;

    hw_id = ztemt_get_hw_id();
	
    if(hw_id != HW_UN){
        strcpy(result,hardware_id_map[hw_id].hw_ver); 
     }else
	    sprintf(result, "%s","unknow");
}
EXPORT_SYMBOL_GPL(ztemt_get_hw_version);

static ssize_t ztemt_hw_version_show(struct kobject *kobj,
		struct kobj_attribute *attr, char *buf)
{
    ztemt_get_hw_version(buf);
    //printk("%s : %d : version=%s\n",__func__,__LINE__,buf);
    ztemt_hw_version_debug("version=%s\n",buf);
    return sprintf(buf,"%s",buf);
}
static struct kobj_attribute version_attr=
    __ATTR(version, 0664, ztemt_hw_version_show, NULL);


#ifdef CONFIG_ZTEMT_HW_VERSION_NX507J
void ztemt_get_hw_sc(char* result)
{
    int hw_id;
    if(!result)
		return;

    hw_id = ztemt_get_hw_id();
	
    if(hw_id != HW_UN){
        strcpy(result,hardware_id_map[hw_id].hw_sc); 
     }else
	    sprintf(result, "%s","unknow");
}
EXPORT_SYMBOL_GPL(ztemt_get_hw_sc);

static ssize_t ztemt_hw_sc_show(struct kobject *kobj, 
		struct kobj_attribute *attr, char *buf)
{
    ztemt_get_hw_sc(buf);
    //printk("%s : %d : sc=%s\n",__func__,__LINE__,buf);
    ztemt_hw_version_debug("sc=%s\n",buf);
    return sprintf(buf,"%s",buf);
}
static struct kobj_attribute sc_attr=
    __ATTR(sc, 0664, ztemt_hw_sc_show, NULL);
#endif


#if defined CONFIG_ZTEMT_HW_VERSION_NX508J || defined CONFIG_ZTEMT_HW_VERSION_NX508H
const struct hardware_id_map_st *ztemt_get_table_item(void)
{

	uint32_t tablesize;
	int hw_type;
	uint32_t i = 0;
	char * gpio_str_temp=board_type_gpio_str;
	int gpio_value_A,gpio_value_B,gpio_value_C;
	const struct hardware_id_map_st *pts= hardware_id_map;

	if ( pts == NULL )
		return NULL;

	tablesize=sizeof(hardware_id_map)/sizeof(hardware_id_map[0]);
	ztemt_hw_version_debug("tablesize=%d\n",tablesize);

	hw_type=ztemt_get_hw_id();

	ztemt_hw_version_debug("%s=%s\n",__func__, gpio_str_temp);
	sscanf(gpio_str_temp ,"%d,%d,%d", &gpio_value_A, &gpio_value_B, &gpio_value_C);

	while (i < tablesize) {
		if ((pts[i].hw_type == hw_type) &&
			(pts[i].gpio_A == gpio_value_A) && (pts[i].gpio_B == gpio_value_B) && (pts[i].gpio_C == gpio_value_C))
			break;
		else
			i++;
	}

	if ( i < tablesize ){
		return &pts[i];
	}
	else
		return NULL;

}

static ssize_t ztemt_hw_config_show(struct kobject *kobj,
		struct kobj_attribute *attr, char *buf)
{
    const struct hardware_id_map_st *pts_tmp;

    pts_tmp=ztemt_get_table_item();
	if(pts_tmp==NULL)
		return sprintf(buf,"%s","unknown");

	ztemt_hw_version_debug("hw_config=%s\n", pts_tmp->hw_config);

	return sprintf(buf,"%s",pts_tmp->hw_config);

}

static struct kobj_attribute config_attr=
    __ATTR(config, 0664, ztemt_hw_config_show, NULL);
#endif

static ssize_t debug_value_store(struct kobject *kobj,
	    struct kobj_attribute *attr, const char *buf, size_t count)
{
    sscanf(buf, "%d", &debug_value);
    return count;
}

static ssize_t debug_value_show(struct kobject *kobj,
	   struct kobj_attribute *attr, char *buf)
{
    return sprintf(buf, "%d", debug_value);	
}

static struct kobj_attribute debug_value_attr=
    __ATTR(debug_value, 0664, debug_value_show, debug_value_store);


static struct attribute *ztemt_hw_version_attrs[] = {
    &debug_value_attr.attr,
    &version_attr.attr,
#ifdef CONFIG_ZTEMT_HW_VERSION_NX507J
    &sc_attr.attr,
#endif
#if defined CONFIG_ZTEMT_HW_VERSION_NX508J || defined CONFIG_ZTEMT_HW_VERSION_NX508H
    &config_attr.attr,
#endif
    NULL,
};

static struct attribute_group ztemt_hw_version_attr_group = {
    .attrs = ztemt_hw_version_attrs,
};


struct kobject *hw_version_kobj;

int __init
ztemt_hw_version_init(void)
{
    int rc = 0;

    ztemt_hw_version_debug("ztemt_hw_version creat attributes start \n");
  
    //hw_version__kobj = kobject_create_and_add("ztemt_hw_version", kernel_kobj);
    hw_version_kobj = kobject_create_and_add("ztemt_hw_version", NULL);
    if (!hw_version_kobj){
	printk(KERN_ERR "%s: ztemt_hw_version kobj create error\n", __func__);
	return -ENOMEM;
    }

    rc=sysfs_create_group(hw_version_kobj,&ztemt_hw_version_attr_group);
    if(rc)
      printk(KERN_ERR "%s: failed to create ztemt_hw_version group attributes\n", __func__);

    ztemt_hw_version_debug("ztemt_hw_version creat attributes end \n");
    return rc;
}


static void __exit
ztemt_hw_version_exit(void)
{
    sysfs_remove_group(hw_version_kobj,&ztemt_hw_version_attr_group);
    kobject_put(hw_version_kobj);	
}

module_init(ztemt_hw_version_init);
module_exit(ztemt_hw_version_exit);

MODULE_DESCRIPTION("ztemt_hw_version driver");
MODULE_LICENSE("GPL v2");
MODULE_ALIAS("platform:ztemt_hw_version" );
