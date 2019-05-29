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

#ifdef CONFIG_gTcpSocket

#ifndef GTCPSOCKET_H
#define GTCPSOCKET_H

#include <gObject.h>
#include <gSocketInterface.h>
#include <gString.h>

#ifdef CONFIG_LINUX_POSIX
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#endif

class GTcpSocketPrivate;

/*! @file  gTcpSocket.h
 * @brief  GTcpSocket TCP client端
 * 
 * @author 明心
 * @version 1.0.0
 * @date 2019-2-6
 */

/*!
 * @class GTcpSocket
 * @brief GTcpSocket TCP client端
 * 
 */
class DLL_PUBLIC GTcpSocket : public GObject, public GSocketInterface
{
public:
    ///调用connect之后，系统会反馈给开发者的连接状态
    enum E_ON_CONNECT
    {
        ///连接成功
        E_ON_CONNECT_SUCCESS,
        
        ///连接失败
        E_ON_CONNECT_FAILED,
    };
#ifndef CONFIG_STD_CPP11
    DEFINE_SIGNAL_EX(void (*T_onReceived)(GObject *receiver, char *buf, uint32_t len),
                     T_onReceived, onReceived)
    DEFINE_SIGNAL_EX(void (*T_onDisconnect)(GObject *receiver, int fd, const sockaddr_in & addr, const socklen_t addr_len),
                     T_onDisconnect, onDisconnect)
    DEFINE_SIGNAL(T_pirv, onConnect)
#else
signals:
    ///当接收到数据时，该信号被发射。
    GSignal<void(char* buf, uint32_t len)> onReceived;
    
    ///当网站出错、或者服务端主动断开连接时，该信号被发射
    GSignal<void(int fd, const sockaddr_in & addr, const socklen_t addr_len)> onDisconnect;
    
    ///当客户端connect服务端成功、或者失败时，该信号被发射。开发者通过连接此信号，来获取connect是否成功
    GSignal<void(E_ON_CONNECT)> onConnect;
#endif
public:
    /**
     * @brief 构造一个默认socket，未bind端口和地址
     * 
     * @param parent ...
     * @param name ...
     */
    explicit GTcpSocket (GObject* parent = 0, const char* name = 0 );
    
    /**
     * @brief 使用指定的信息来构造一个 tcp socket。该函数可以配合 GTcpServer::onAccept 来进行使用。
     * 
     * @param fd ...
     * @param addr ...
     * @param addr_len ...
     * @param parent ...
     * @param name ...
     */
    explicit GTcpSocket (int fd, const sockaddr_in &addr, socklen_t addr_len, GObject* parent = 0, const char* name = 0 );
    
    virtual ~GTcpSocket();

    /**
     * @brief 使用指定的信息去连接tcp 服务端。开发者需要通过 onConnect 信号来获取是否连接成功
     * 
     * @param hPort ...
     * @param addr int格式的ip地址
     * @param msec 超时
     * @return void
     */
    void connect(const uint16_t hPort, const in_addr_t addr, int msec=10000);
    
    /**
     * @brief 使用指定的信息去连接tcp 服务端。开发者需要通过 onConnect 信号来获取是否连接成功
     * 
     * @param hPort ...
     * @param domainName 点分十进制的字符串IP地址，或者域名
     * @param msec 超时
     * @return void
     */
    void connect(const uint16_t hPort, const GString& domainName, int msec=10000);
    
    /**
     * @brief 发送数据
     * 
     * @param pBuf ...
     * @param length ...
     * @param flags ...
     * @return uint32_t
     */
    virtual uint32_t send(const char* pBuf, size_t length, int flags = 0 );
    
    /**
     * @brief 绑定到指定端口
     * 
     * @param hPort ...
     * @param addr ...
     * @return int
     */
    int bind(const uint16_t hPort, const in_addr_t addr);
    
    virtual bool event(GEvent* evt);
    
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
    GTcpSocketPrivate *priv;
};

#endif

#endif  //CONFIG_gTcpSocket
