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

#ifdef CONFIG_gPtrList

#ifndef QPTRLIST_H
#define QPTRLIST_H

#include <list>
#include <algorithm>
#include <gCtrlDllSym.h>

using namespace std;

/*! @file  gPtrList.h
 * @brief  GPtrList 指针列表
 * 
 * @author 明心
 * @version 1.0.0
 * @date 2019-2-6
 */

/*!
 * @class GPtrList
 * @brief 非常便于使用的指针列表
 * 
 */
template<class type> 
class DLL_PUBLIC GPtrList : public list<type*>
{
public:
    typedef  list<type*> LIST_T;
    GPtrList();
    virtual ~GPtrList();
    void inSort ( const type * item );
    bool removeRef ( const type * item );
    bool isEmpty();
    unsigned int count() const;
    type* at( unsigned int i ) const;
    void append ( const type* item );
    bool insert ( unsigned int idx, const type* item );
    void setAutoDelete ( bool  );
    void clear();
    bool remove ( const unsigned int idx );
    int findRef ( const type * item );
    type* front();
    const type* front() const;
    type* back();
    const type* back() const;
//     typename LIST_T::iterator  eraseIterator(typename LIST_T::iterator __position)
//     {
//         if(m_auto_delete)
//         {
//             delete *__position;
//         }
//         return LIST_T::erase( __position );
//     }
    
protected:
    virtual int compareItems ( type* item1, type* item2 )
    {
        return item1 - item2;
    }
    
private:
    bool m_auto_delete;
};

template<class type> 
GPtrList<type>::GPtrList()
:m_auto_delete(false)
{
    
}

template<class type> 
GPtrList<type>::~GPtrList()
{
    clear();
}

// template<class type> 
// bool GPtrList<type>::operator== ( const GPtrList& other )
// {
// 
// }

template<class type> 
void GPtrList<type>::inSort ( const type* pItem )
{
    type* item = const_cast<type*>(pItem);
    int cnt = LIST_T::size();
    if(0 == cnt ) {
        LIST_T::push_back(item);
        return ;
    }
    if(0 <= compareItems(item,  LIST_T::back() )  ) {
        LIST_T::push_back(item);
        return ;
    }
    
    int ret, m;
    int s=0;
    int d=cnt-1;
    typename LIST_T::iterator it, itD;
    while(s<=d) {
        m=(s+d)/2;
        it=LIST_T::begin();
        advance(it, m);
        ret = compareItems(item,  *it);
        if(0 == ret){
            insert(m, item);
            return ;
        }else if(0 > ret){
            d=m-1;
            itD = it;
        }else{
            s=m+1;
        }
    }
    
    LIST_T::insert(itD, item);
}

template<class type> 
type* GPtrList<type>::at ( unsigned int i ) const
{
    if(i >= LIST_T::size() )
    {
        return NULL;
    }
    typename LIST_T::const_iterator it=LIST_T::begin();
    advance(it, i);
    return  *it;
}

template<class type> 
unsigned int GPtrList< type >::count() const
{
    return LIST_T::size();
}

template<class type> 
bool GPtrList< type >::isEmpty()
{
    return LIST_T::empty();
}

template<class type> 
bool GPtrList< type >::removeRef ( const type* item )
{
    LIST_T::remove(const_cast<type*>(item));
    if(m_auto_delete)
    {
        delete item;
    }
    return true;
}

template<class type> 
void GPtrList<type>::append ( const type* item )
{
    LIST_T::push_back(const_cast<type*>(item));
}

template<class type> 
void GPtrList<type>::setAutoDelete ( bool  b)
{
    m_auto_delete = b;
}

template<class type> 
void GPtrList<type>::clear ( )
{
    if(m_auto_delete)
    {
        typename LIST_T::iterator it = LIST_T::begin();
        for(; it != LIST_T::end(); ++it)
        {
            delete  *it;
        }
    }
    LIST_T::clear();
}

template<class type> 
int GPtrList<type>::findRef ( const type* item )
{
    typename LIST_T::iterator  it;
    it = find(LIST_T::begin(), LIST_T::end(),  item);
    if( it == LIST_T::end() )
    {
        return -1;
    }
    return distance(LIST_T::begin(),  it);
}

template<class type> 
bool GPtrList< type >::insert ( unsigned int idx, const type* item )
{
    if(idx >= LIST_T::size() )
    {
        return false;
    }
    typename LIST_T::iterator it=LIST_T::begin();
    advance(it, idx);
    LIST_T::insert(it, const_cast<type*>(item) );
    return true;
}

template<class type> 
bool GPtrList< type >::remove ( const unsigned int idx )
{
    if(idx >= LIST_T::size() )
    {
        return false;
    }
    typename LIST_T::iterator it=LIST_T::begin();
    advance(it, idx);
    if(m_auto_delete)
    {
	 type*  item = *it;
	 delete item;
    }

    LIST_T::erase(it);
    
    return true;
}

template<class type> 
type* GPtrList<type>::front ( )
{
    if( LIST_T::empty() )
    {
        return NULL;
    }

    return  LIST_T::front();
}

template<class type> 
const type* GPtrList<type>::front ( ) const
{
    if( LIST_T::empty() )
    {
        return NULL;
    }

    return  LIST_T::front();
}


template<class type> 
type* GPtrList<type>::back ( )
{
    if( LIST_T::empty() )
    {
        return NULL;
    }
    
    return  LIST_T::back();
}

template<class type> 
const type* GPtrList<type>::back ( ) const
{
    if( LIST_T::empty() )
    {
        return NULL;
    }
    
    return  LIST_T::back();
}

#endif // QPTRLIST_H

#endif  //CONFIG_gPtrList
