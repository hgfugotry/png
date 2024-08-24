/**********************************hashTable.cpp*************************************/
/**********************************与hashTable.hpp一起编译***************************/
/***********************************hgfugotry 2024.8.23*****************************/
#include "hashTable.hpp"
#include<cassert>

#define DEBUG
#ifdef DEBUG
#include<iostream>
int main()
{
    using namespace std;
    HashTable hash(7);

    if(!hash.add(39)){a:cout<<"failed!"<<endl;return -1;}
    if(!hash.add(139))goto a;
    
    cout<<"39的数量:"<<hash.check(39)<<endl;
    cout<<"139的数量:"<<hash.check(139)<<endl;
    if(!hash.del(39))goto a;
    cout<<"39的数量:"<<hash.check(39)<<endl;
    cout<<"139的数量:"<<hash.check(139)<<endl;
    cout<<"139删除成功了吗?"<<(hash.del(139)?"成功":"失败")<<endl;
    cout<<"139删除成功了吗?"<<(hash.del(139)?"成功":"失败")<<endl;
    cout<<"139剩余数量:"<<hash.check(139)<<endl;
    hash.add(239);
    cout<<"239剩余数量:"<<hash.check(239)<<endl;
    hash.add(123);
    hash.add(123);
    cout<<"123剩余数量:"<<hash.check(123)<<endl;
}
#endif

bool HashTable::makeNewNode(Pair** pNode,int data_)//为新项分配内存
{
    if(!((*pNode)=new Pair))
        return false;
    newNodeInit(*pNode,data_);
    ++((*(pNode))->num);
    return true;
}

Pair* HashTable::seekNode(Pair* ppair,int data_)//查找项，存在，返回指向它的指针；否则返回空指针
{
    int index=getIndex(data_);
    ppair=&hash_table[index];
    while ((ppair!=nullptr)&&(ppair->data!=data_))
        ppair=ppair->next;
    return ppair;
}

bool HashTable::delNode(Pair *ppair)//释放内存
{
    if(ppair==nullptr)
        return false;
    delete ppair;
    return true;
}

int HashTable::getIndex(int data_)//返回数据在哈希表中应处的位置链表头索引
{
    return data_%size_;
}

Pair *HashTable::seekEndNode(Pair* ppair)//查找链表中最后一项
{
    if(ppair->next==nullptr)
        return ppair;
    else
        return seekEndNode(ppair->next);
}

bool HashTable::clearAIndex(Pair* ppair)//清理链表
{
    if((ppair->next)==nullptr)
        return delNode(ppair);
    else
        return clearAIndex(ppair->next);
}

void HashTable::newNodeInit(Pair* ppair,int data_)//初始化节点
{
    ppair->data=data_;
    ppair->num=0;
    ppair->next=nullptr;
}

bool HashTable::deleteNode(int data_)//删除项，将上一项的next指针设为nullptr
{
    Pair** pppair;
    Pair* temp=hash_table;
    int index=getIndex(data_);
    pppair=&temp;
    (*pppair)+=index;//此时*pppair指向data存放在的链表
    Pair** lastPppairValue=pppair;
    
    bool returnValue;
    while ((*pppair)!=nullptr)
    {
        lastPppairValue=pppair;
        pppair=&((*pppair)->next);
        if((*pppair)->data=data_)
        {
            Pair* temp=(*pppair)->next;
            returnValue=delNode((*pppair));
            (*lastPppairValue)->next=temp;
            break;
        }
    }
    return returnValue;
}

HashTable::HashTable(int size)//初始化哈希表，分配size个Pair的内存
{
    if(size<=0)
        return;
    hash_table=new Pair[size];
    size_=size;
    for(int i=0;i<size;i++)
        newNodeInit(hash_table+i,i);
}

bool HashTable::add(int data_)//添加项
{
    int position=getIndex(data_);
    Pair* pNode;
    if(!(pNode=seekNode(pNode,data_)))
    {
        pNode=seekEndNode(&hash_table[position]);
        return makeNewNode(&(pNode->next),data_);
    }
    else
        ++(pNode->num);
    return true;
}

bool HashTable::del(int data_)//删除项
{
    Pair* pNode;
    if(!(pNode=seekNode(pNode,data_)))
        return false;
    else if((pNode->num)>1||pNode==&hash_table[getIndex(data_)])
    {
        if((pNode->num)>0)
        {
            --(pNode->num);
            return true;
        }
        else
            return false;
    }
    else
        return deleteNode(data_);
}

int HashTable::check(int data_)//查找项，返回项数
{
    Pair* ppair;
    if(ppair=seekNode(ppair,data_))
        return ppair->num;
    else
        return 0;
}

HashTable::~HashTable()//释放所有内存
{
    Pair* pNode;
    for(int i=0;i<size_;i++)
        if((pNode=hash_table[i].next)!=nullptr)
            clearAIndex(pNode);
    delete[]hash_table;
}
