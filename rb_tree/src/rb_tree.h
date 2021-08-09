/*
 * *@*	Description: 
 * *@*	Version: 
 * *@*	Author: yusheng Gao
 * *@*	Date: 2021-08-06 15:37:42
 * *@*	LastEditors: yusheng Gao
 * *@*	LastEditTime: 2021-08-06 15:37:43
 */

#ifndef RB_TREE_H
#define RB_TREE_H

#include <cstdio>

struct CRBNode{

    int m_val;
    bool is_red;
    int m_count;
    int m_treenode_count;

    CRBNode* m_left;
    CRBNode* m_right;
    CRBNode* m_father;

    CRBNode(int _val):
        m_val(_val),
        is_red(true),
        m_count(1),
        m_treenode_count(1),
        m_left(nullptr),
        m_right(nullptr),
        m_father(nullptr)
    {
    }

    ~CRBNode()
    {
        if(this->m_left) delete this->m_left;
        if(this->m_right) delete this->m_right;
    }

};


class CLRBTree final{
public:

    CLRBTree():
        m_root(nullptr)
    {

    }

    CLRBTree(int _val):
        m_root(new CRBNode(_val))
    {

    }

    ~CLRBTree()
    {
        if(this->m_root) delete this->m_root;
    }

    CRBNode* search(int _val);

    void insert(int _val);

    bool erase(int _val);

    void show_inorder();

    void show_preorder();

private:

    CRBNode* m_root;

    void show_inorder(CRBNode* _root);

    void show_preorder(CRBNode* _root);

    CRBNode* search(CRBNode* _root, int _val);

    void LL(CRBNode** _root);

    void RR(CRBNode** _root);

    void insert(CRBNode** _root, int _val);

    void fix_after_insert(CRBNode** _pinsert);

    void fix_after_erase(CRBNode* _preplace, CRBNode* _pfather);

    bool erase(CRBNode** _root, int _val);

    CRBNode* get_prior(CRBNode* _root);

    CRBNode* get_post(CRBNode* _root);
};





#endif