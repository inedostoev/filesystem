#pragma once

#include<cstddef>
#include <iostream>

template<typename K, typename T, size_t N>
struct Node
{
    size_t m_Size;
    K m_Keys[N];
    T m_Data[N];
    Node* m_Next[N + 1];
    ~Node();
    void add(Node* aParent, const K& aKey, const T& aData);
    void print(size_t aLevel);
    void stride(size_t aPos);
    void split(Node* aParent);
};


template<typename K, typename T, size_t N>
void Node<K, T, N>::print(size_t aLevel)
{
    for (size_t i = 0; i < m_Size; ++i)
    {
        if (m_Next[i])
            m_Next[i]->print(aLevel + 1);
        std::cout << "level: " << aLevel << " key = " << m_Keys[i] << " data = " << m_Data[i] << std::endl;
    }
    if (m_Next[m_Size])
        m_Next[m_Size]->print(aLevel + 1);
}

template<typename K, typename T, size_t N>
Node<K, T, N>::~Node()
{
    for (size_t i = 0; i < N; ++i)
    {
        if (!m_Next[i])
            return;
        else
            delete m_Next[i];
    }
}

template<typename K, typename T, size_t N>
void Node<K, T, N>::stride(size_t aPos)
{
    m_Next[N] = m_Next[N - 1];
    for (size_t i = N - 1; i > aPos; --i)
    {
        m_Keys[i] = m_Keys[i - 1];
        m_Data[i] = m_Data[i - 1];
        m_Next[i] = m_Next[i - 1];
    }
    m_Size++;
}

template<typename K, typename T, size_t N>
void Node<K, T, N>::split(Node* aParent)
{
    Node* sTmp = new Node{};
    size_t sSize = 0;
    for (size_t i = m_Size / 2 + 1; i < m_Size; ++i)
    {
        sTmp->m_Keys[sSize] = m_Keys[i];
        sTmp->m_Data[sSize] = m_Data[i];
        sTmp->m_Next[sSize] = m_Next[i];
        sSize++;
        m_Next[i] = nullptr;
    }
    sTmp->m_Next[sSize] = m_Next[m_Size];
    sTmp->m_Size = sSize;
    size_t i = 0;
    for (; i < aParent->m_Size; ++i)
        if (this == aParent->m_Next[i])
            break;
    aParent->stride(i);
    aParent->m_Next[i + 1] = sTmp;
    aParent->m_Next[i] = this;
    aParent->m_Keys[i] = m_Keys[m_Size / 2];
    aParent->m_Data[i] = m_Data[m_Size / 2];
    m_Size /= 2;
}

template<typename K, typename T, size_t N>
void Node<K, T, N>::add(Node* aParent, const K& aKey, const T& aData)
{
    size_t i = 0;
    for (; i < m_Size; ++i)
        if (aKey <= m_Keys[i])
            break;
    if (m_Next[i])
        m_Next[i]->add(this, aKey, aData);
    else
    {
        stride(i);
        m_Keys[i] = aKey;
        m_Data[i] = aData;
    }
    if (N == m_Size && aParent)
        split(aParent);
}


template<typename K, typename T, size_t N = 5>
class BTree
{
    Node<K, T, N>* m_Data;
public:
    BTree(): m_Data(nullptr)
    {}
    ~BTree();
    T* find(const K& aKey);
    void print();
    bool insert(const K& aKey, const T& aData);
};

template<typename K, typename T, size_t N>
T* BTree<K, T, N>::find(const K& aKey)
{
    Node<K, T, N>* sCurrent = m_Data;
    while (sCurrent)
    {
        size_t i = 0;
        for (; sCurrent->m_Keys[i] < aKey; ++i)
            if (i == sCurrent->m_Size - 1)
                return nullptr;
        if (aKey == sCurrent->m_Keys[i])
            return &sCurrent->m_Data[i];
        else
            sCurrent = sCurrent->m_Next[i];
    }
    return nullptr;
}

template<typename K, typename T, size_t N>
bool BTree<K, T, N>::insert(const K& aKey, const T& aData)
{
    if (!m_Data)
        m_Data = new Node<K, T, N>{};
    m_Data->add(nullptr, aKey, aData);
    if (N == m_Data->m_Size)
    {
        Node<K, T, N>* sTmp = new Node<K, T, N>{};
        m_Data->split(sTmp);
        m_Data = sTmp;
    }
    return false;
}

template<typename K, typename T, size_t N>
BTree<K, T, N>::~BTree()
{
    delete m_Data;
}

template<typename K, typename T, size_t N>
void BTree<K, T, N>::print()
{
    m_Data->print(0);
}
