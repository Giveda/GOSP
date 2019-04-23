/*
 * Copyright (C) 2019  明心  <imleizhang@qq.com>
 * All rights reserved.
 * 
 * This program is an open-source software; and it is distributed in the hope 
 * that it will be useful, but WITHOUT ANY WARRANTY; without even the
 * implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
 * PURPOSE. 
 * This program is not a free software; so you can not redistribute it and/or 
 * modify it without my authorization. If you only need use it for personal
 * study purpose(no redistribution, and without any  commercial behavior), 
 * you should accept and follow the GNU AGPL v3 license, otherwise there
 * will be your's credit and legal risks.  And if you need use it for any 
 * commercial purpose, you should first get commercial authorization from
 * me, otherwise there will be your's credit and legal risks. 
 *
 */

#include <config_giveda.h>

#ifdef CONFIG_gRwLock

#ifndef GRWLOCK_H
#define GRWLOCK_H

#ifdef CONFIG_LINUX_POSIX
#include <pthread.h>
typedef pthread_rwlock_t RWLOCK_HANDLE;
#endif

#include <gCtrlDllSym.h>

/*! @file  gRwLock.h
 * @brief  GRWLock 线程锁
 * 
 * @author 明心
 * @version 1.0.0
 * @date 2019-2-6
 */

/*!
 * @class GRWLock
 * @brief GRWLock 线程锁。可用于多线程场景下，对临界资源进行加锁保护
 * 
 */
class DLL_PUBLIC GRWLock
{
public:
    /**
     * @brief ...
     * 
     */
    GRWLock();
    virtual ~GRWLock();

public:
    /**
     * @brief 获取读锁。用于仅需要读取临界资源时。与 releaseReadLock() 配套使用
     * 
     * @return void
     */
    void        acquireReadLock();
    
    /**
     * @brief 释放读锁。用于仅需要读取临界资源时。与 acquireReadLock() 配套使用
     * 
     * @return void
     */
    void        releaseReadLock();

    /**
     * @brief 获取写锁。用于需要写入临界资源时。与 releaseWriteLock() 配套使用
     * 
     * @return void
     */
    void        acquireWriteLock();
    
    /**
     * @brief 释放写锁。用于需要写入临界资源时。与 acquireWriteLock() 配套使用
     * 
     * @return void
     */
    void        releaseWriteLock();

private:
    RWLOCK_HANDLE m_hLock;
};

#endif

#endif  //CONFIG_gRwLock
