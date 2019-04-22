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

#ifdef CONFIG_gUdpSocket

#ifndef GUDPSOCKET_H
#define GUDPSOCKET_H

#include <gObject.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

class GUdpSocketPrivate;

/*! @file  gUdpSocket.h
 * @brief  GUdpSocket UDP socket
 * 
 * @author 明心
 * @version 1.0.0
 * @date 2019-2-6
 */

/*!
 * @class GUdpSocket
 * @brief GUdpSocket UDP socket
 * 
 */
class DLL_PUBLIC GUdpSocket : public GObject
{
#ifndef CONFIG_STD_CPP11
    DEFINE_SIGNAL_EX(void (*T_onReceived)(GObject *receiver, char *buf, uint32_t len, const sockaddr_in &addr),
                     T_onReceived, onReceived)
#else
signals:
    ///当接收到数据时，该信号被发射。
    GSignal<void(char *buf, uint32_t len, const sockaddr_in &addr)> onReceived;
#endif
public:
    /**
     * @brief 构造一个默认socket，未bind端口和地址
     * 
     * @param parent ...
     * @param name ...
     */
    explicit GUdpSocket (GObject* parent = 0, const char* name = 0 );
    
    virtual ~GUdpSocket();
    
    /**
     * @brief 发送信息
     * 
     * @param pBuf ...
     * @param length ...
     * @param flags ...
     * @param dest_addr ...
     * @param dest_len ...
     * @return int
     */
    int sendto(const char* pBuf, size_t length, int flags, sockaddr* dest_addr, socklen_t dest_len);
    
    /**
     * @brief 绑定到指定端口
     * 
     * @param hPort ...
     * @param addr ...
     * @return int
     */
    int bind(const uint16_t hPort, const in_addr_t addr);
    
    virtual bool event(GEvent*);
    
    /**
     * @brief 设置接收缓冲区的大小，以字节为单位。默认情况下为2KB
     * 
     * @param bytes 
     * @return void
     */
    void setRecvBufferSize( uint32_t  bytes );
    
    /**
     * @brief 返回接收缓冲区的大小，以字节为单位
     * 
     * @return uint32_t
     */
    uint32_t recvBufferSize() const;
    
private:
    GUdpSocketPrivate *priv;
};

#endif

#endif  //CONFIG_gUdpSocket
