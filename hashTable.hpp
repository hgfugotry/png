/**********************************hashTable.hpp*************************************/
/**********************************与hashTable.cpp一起编译*****************************/
/***********************************hgfugotry 2024.8.23*****************************/
#ifndef hashTable_hpp_
#define hashTable_hpp_

struct Pair
{
    int data;
    int num;
    Pair* next;
};

//***************************哈希表结构*********************************
//Pair*数组 0   1   2 ... (size-2) (size_-1)
//         |       |   |    |        |
//         Pair    PairPairPair      Pair
//         |       |        |
//         Pair    Pair    Pair
//                 |
//                 Pair
//         链表链表链表链表链表链表链表链表链表链表
//数组的每个元素都是一个链表头
//data%size_得出数据所在的链表
class HashTable
{
private:
    Pair* hash_table;
    int size_;
    bool makeNewNode(Pair** ppair,int data_);
    Pair* seekNode(Pair* pppair,int data_);
    bool delNode(Pair* ppair);
    int getIndex(int data);
    Pair* seekEndNode(Pair* start);
    bool clearAIndex(Pair* ppair);
    void newNodeInit(Pair* ppair,int data_);
    bool deleteNode(int data_);
public:
    HashTable(int size);//size:Pair数组大小
    bool add(int data);//添加数据data，数据数量+1，成功返回true,否则返回false,下同
    bool del(int data);//删除数据，数据数量-1
    int check(int data);//查找数据，返回数据数量
    ~HashTable();
};
#endif