/*
 * linux/drivers/cpufreq/sunxi-iks-cpufreq.h
 *
 * Copyright(c) 2013-2015 Allwinnertech Co., Ltd.
 *         http://www.allwinnertech.com
 *
 * Author: sunny <sunny@allwinnertech.com>
 *
 * allwinner sunxi iks cpufreq driver header file.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

#ifndef CPUFREQ_SUNXI_IKS_H
#define CPUFREQ_SUNXI_IKS_H

#include <linux/cpufreq.h>
#include <linux/device.h>
#include <linux/types.h>

#define CPUFREQ_DBG(format,args...)   pr_debug("[cpu_freq] DBG: "format,##args)
#define CPUFREQ_ERR(format,args...)   printk(KERN_ERR "[cpu_freq] ERR: "format,##args)

/* sun9i platform support two clusters,
 * cluster0 : cortex-a7,
 * cluster1 : cortex-a15.
 */
#define A7_CLUSTER	0
#define A15_CLUSTER	1
#define MAX_CLUSTERS	2


#ifdef CONFIG_ARCH_SUN9IW1P1
#define SUNXI_CPUFREQ_C0_CPUVDD        "axp22_dcdc3"
#define SUNXI_CPUFREQ_C1_CPUVDD        "axp15_dcdc1"
#elif defined(CONFIG_ARCH_SUN8IW6P1) || defined(CONFIG_ARCH_SUN8IW9P1)
#define SUNXI_CPUFREQ_C0_CPUVDD        "vdd-cpua"
#define SUNXI_CPUFREQ_C1_CPUVDD        "vdd-cpub"
#endif

/* the actual and virtual freq convert */
#define ACTUAL_FREQ(cluster, freq)	((cluster == A7_CLUSTER) ? freq << 1 : freq)
#define VIRT_FREQ(cluster, freq)	((cluster == A7_CLUSTER) ? freq >> 1 : freq)

/* the threshold freq of cpu big little switch */
#define SUNXI_BL_SWITCH_THRESHOLD (600000)

/* config the transition latency, based on ns */
#define SUNXI_FREQTRANS_LATENCY   (2000000)

#ifdef CONFIG_BL_SWITCHER
extern bool bL_switching_enabled;
#define is_bL_switching_enabled()		bL_switching_enabled
#define set_switching_enabled(x) 		(bL_switching_enabled = (x))
#else
#define is_bL_switching_enabled()		false
#define set_switching_enabled(x) 		do { } while (0)
#endif

static inline int cpu_to_cluster(int cpu)
{
#ifdef CONFIG_SMP
	return is_bL_switching_enabled() ? MAX_CLUSTERS : topology_physical_package_id(cpu);
#else
	return 0;
#endif
}

#endif /* CPUFREQ_SUNXI_IKS_H */
