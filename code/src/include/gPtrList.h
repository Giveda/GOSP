/*
 * Copyright (C) 2019  明心  <imleizhang@qq.com>
 * All rights reserved.
 *
 * This program is an open-source software; and it is distributed in the hope
 * that it will be useful, but WITHOUT ANY WARRANTY; without even the
 * implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 * PURPOSE.
 * This program is not a free software; so you can not redistribute it(include
 * binary form and source code form) without my authorization. And if you
 * need use it for any commercial purpose, you should first get commercial
 * authorization from me, otherwise there will be your's legal&credit risks.
 *
 */

#ifndef GPTRLIST_H
#define GPTRLIST_H

#include <list>
#include <algorithm>
#include <gConstDefine.h>
#include <gGlobal.h>

using namespace std;

template<class type>
class GPtrList
{
public:
    typedef  list<type*> LIST_T;
    GPtrList();
    virtual ~GPtrList();
    void inSort ( const type * item );
    bool removeRef ( const type * item );
    bool isEmpty();
    unsigned int count() const;
    type* first ();
    type* next();
    type* at( unsigned int i );
    void append ( const type* item );
    bool insert ( unsigned int idx, const type* item );
    void setAutoDelete ( bool  );
    void clear();
    bool remove ( const unsigned int idx );
    type* getLast () const;
    int findRef ( const type * item );

protected:
    virtual int compareItems ( type* item1, type* item2 )
    {
        return item1 - item2;
    }

private:
    bool m_auto_delete;
    LIST_T   m_lst;
    typename LIST_T::iterator m_it;
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


template<class type>
void GPtrList<type>::inSort ( const type* pItem )
{
    type* item = const_cast<type*>(pItem);
    int size = m_lst.size();
    if(0 == size ) {
        m_lst.push_back(item);
        return ;
    }
    if(0 <= compareItems(item,  m_lst.back() )  ) {
        m_lst.push_back(item);
        return ;
    }

    int ret, m;
    int s=0;
    int d=size-1;
    typename LIST_T::iterator it, itD;
    while(s<=d) {
        m=(s+d)/2;
        it=m_lst.begin();
        advance(it, m);
        ret = compareItems(item,  *it);
        if(0 == ret) {
            insert(m, item);
            return ;
        } else if(0 > ret) {
            d=m-1;
            itD = it;
        } else {
            s=m+1;
        }
    }

    m_lst.insert(itD, item);
}

template<class type>
type* GPtrList<type>::at ( unsigned int i )
{
    if(i >= m_lst.size() )
    {
        return NULL;
    }
    typename LIST_T::iterator it=m_lst.begin();
    advance(it, i);
    return  *it;
}

template<class type>
unsigned int GPtrList< type >::count() const
{
    return m_lst.size();
}

template<class type>
type* GPtrList< type >::first()
{
    m_it = m_lst.begin();
    return m_lst.front();
}

template<class type>
type* GPtrList< type >::next()
{
    if(m_it == m_lst.end() )
    {
        return NULL;
    }
    advance(m_it, 1);
    if(m_it == m_lst.end() )
    {
        return NULL;
    }

    return *m_it;
}

template<class type>
bool GPtrList< type >::isEmpty()
{
    return m_lst.empty();
}

template<class type>
bool GPtrList< type >::removeRef ( const type* item )
{
    m_lst.remove(const_cast<type*>(item));
    if(m_auto_delete)
    {
        delete item;
    }
    return true;
}

template<class type>
type* GPtrList< type >::getLast() const
{
    return m_lst.back();
}

template<class type>
void GPtrList<type>::append ( const type* item )
{
    m_lst.push_back(const_cast<type*>(item));
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
        typename LIST_T::iterator it = m_lst.begin();
        for(; it != m_lst.end(); ++it)
        {
            delete  *it;
        }
    }
    m_lst.clear();
}

template<class type>
int GPtrList<type>::findRef ( const type* item )
{
    typename LIST_T::iterator  it;
    it = find(m_lst.begin(), m_lst.end(),  item);
    if( it == m_lst.end() )
    {
        return -1;
    }
    return distance(m_lst.begin(),  it);
}

template<class type>
bool GPtrList< type >::insert ( unsigned int idx, const type* item )
{
    if(idx >= m_lst.size() )
    {
        return false;
    }
    typename LIST_T::iterator it=m_lst.begin();
    advance(it, idx);
    m_lst.insert(it, const_cast<type*>(item) );
    return true;
}

template<class type>
bool GPtrList< type >::remove ( const unsigned int idx )
{
    if(idx >= m_lst.size() )
    {
        return false;
    }
    typename LIST_T::iterator it=m_lst.begin();
    advance(it, idx);
    m_lst.erase(it);

    if(m_auto_delete)
    {
        type*  item = *it;
        delete item;
    }
    return true;
}

#endif 


// a nice day ^_^
// a nice day ^_^
