/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2018-05-17     armink       the first version
 */

#ifndef _FAL_CFG_H_
#define _FAL_CFG_H_

#include <rtconfig.h>
#include <board.h>

#define NOR_FLASH_DEV_NAME             "norflash0"

/* ===================== Flash device Configuration ========================= */
//extern const struct fal_flash_dev stm32f1_onchip_flash;
extern struct fal_flash_dev flash_nm25q;

/* flash device table */
#define FAL_FLASH_DEV_TABLE                                          \
{                                                                    \
    &flash_nm25q,                                                     \
}
/* ====================== Partition Configuration ========================== */
#ifdef FAL_PART_HAS_TABLE_CFG
/* partition table */
#define FAL_PART_TABLE                                                               \
{                                                                                    \
    {FAL_PART_MAGIC_WORD,  "kvdb1", FAL_USING_NOR_FLASH_DEV_NAME,                        0, 1*1024*1024,         0}, \
    {FAL_PART_MAGIC_WORD,  "kvdb2", FAL_USING_NOR_FLASH_DEV_NAME,              1*1024*1024, 1*1024*1024,         0}, \
    {FAL_PART_MAGIC_WORD,  "kvdb3", FAL_USING_NOR_FLASH_DEV_NAME,              2*1024*1024, 1*1024*1024,         0}, \
    {FAL_PART_MAGIC_WORD,  "kvdb4", FAL_USING_NOR_FLASH_DEV_NAME,              3*1024*1024, 1*1024*1024,         0}, \
    {FAL_PART_MAGIC_WORD,  "tsdb1", FAL_USING_NOR_FLASH_DEV_NAME,              4*1024*1024, 1*1024*1024,         0}, \
    {FAL_PART_MAGIC_WORD,  "tsdb2", FAL_USING_NOR_FLASH_DEV_NAME,              5*1024*1024, 8*1024*1024,         0}, \
    {FAL_PART_MAGIC_WORD,  "tsdb3", FAL_USING_NOR_FLASH_DEV_NAME,              13*1024*1024, 2*1024*1024,         0}, \
    {FAL_PART_MAGIC_WORD, "download", FAL_USING_NOR_FLASH_DEV_NAME,            15*1024*1024, 1*1024*1024,         0}, \
}
#endif /* FAL_PART_HAS_TABLE_CFG */

#endif /* _FAL_CFG_H_ */
