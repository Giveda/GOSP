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

#ifdef CONFIG_gTcpServer

#ifndef GTCPSERVER_H
#define GTCPSERVER_H

#include <gObject.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

class GTcpServerPrivate;

/*! @file  gTcpServer.h
 * @brief  GTcpServer TCP server端
 * 
 * @author 明心
 * @version 1.0.0
 * @date 2019-2-6
 */

/*!
 * @class GTcpServer
 * @brief GTcpServer TCP server端
 * 
 */
class DLL_PUBLIC GTcpServer : public GObject
{
#ifndef CONFIG_STD_CPP11
    DEFINE_SIGNAL_EX(void (*T_onAccept)(GObject *receiver,  int fd, const sockaddr_in & addr, const socklen_t addr_len),
                  T_onAccept, onAccept)
#else
signals:
    ///当有客户端连接上来时，该信号被立即发射。
    GSignal<void(int fd, const sockaddr_in & addr, const socklen_t addr_len)> onAccept;
#endif
public:
    explicit GTcpServer (GObject* parent = 0, const char* name = 0 );
    virtual ~GTcpServer();
    
    /**
     * @brief 绑定到指定端口
     * 
     * @param hPort ...
     * @param addr ...
     * @return int
     */
    int bind(const uint16_t hPort, const in_addr_t addr);
    
    /**
     * @brief 启动监听
     * 
     * @param backlog 参见 man 2 listen
     * @return int
     */
    int listen(int backlog);
    
    virtual bool event(GEvent* evt);
    
private:
    GTcpServerPrivate *priv;
};

#endif

#endif  //CONFIG_gTcpServer
