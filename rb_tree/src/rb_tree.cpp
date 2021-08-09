/*
 * *@*	Description: 
 * *@*	Version: 
 * *@*	Author: yusheng Gao
 * *@*	Date: 2021-08-06 16:32:30
 * *@*	LastEditors: yusheng Gao
 * *@*	LastEditTime: 2021-08-06 16:32:30
 */

#include "rb_tree.h"

void CLRBTree::insert(int _val){
    this->insert(&(this->m_root), _val);
}

bool CLRBTree::erase(int _val){
    return this->erase(&(this->m_root), _val);
}


CRBNode* CLRBTree::search(int _val){
    return this->search(this->m_root, _val);
}

CRBNode* CLRBTree::search(CRBNode* _root, int _val){
    if(!_root || _root->m_val == _val) return _root;

    if(_val > _root->m_val) return search(_root->m_right, _val);

    return search(_root->m_left, _val);
}

void CLRBTree::show_inorder(){
    this->show_inorder(this->m_root);
    puts("");
}

void CLRBTree::show_inorder(CRBNode* _root){
    if(!_root) return;
    this->show_inorder(_root->m_left);

    printf("%d:%c\t", _root->m_val, _root->is_red?'R':'B');

    this->show_inorder(_root->m_right);
}

void CLRBTree::show_preorder(){
    this->show_preorder(this->m_root);
    puts("");
}

void CLRBTree::show_preorder(CRBNode* _root){
    if(!_root) return;
    printf("%d:%c\t", _root->m_val, _root->is_red?'R':'B');

    this->show_preorder(_root->m_left);

    this->show_preorder(_root->m_right);
}

CRBNode* CLRBTree::get_prior(CRBNode* _root){
    if(!_root->m_left) return nullptr;

    CRBNode* res = _root->m_left;

    while(res->m_right) res = res->m_right;

    return res;
}

CRBNode* CLRBTree::get_post(CRBNode* _root){
    if(!_root->m_right) return nullptr;

    CRBNode* res = _root->m_right;

    while(res->m_left) res = res->m_left;

    return res;
}

void CLRBTree::RR(CRBNode** _root){  // 左旋
    
    CRBNode* newroot = (*_root)->m_right;
    newroot->m_father = (*_root)->m_father;
    int newcount = (*_root)->m_treenode_count;

    (*_root)->m_treenode_count -= newroot->m_treenode_count;

    (*_root)->m_right = newroot->m_left;
    if((*_root)->m_right){
        (*_root)->m_treenode_count += (*_root)->m_right->m_treenode_count;
        (*_root)->m_right->m_father = (*_root);
    }

    newroot->m_left = (*_root);
    (*_root)->m_father = newroot;

    (*_root) = newroot;

    (*_root)->m_treenode_count = newcount;

}

void CLRBTree::LL(CRBNode** _root){   // 右旋

    CRBNode* newroot = (*_root)->m_left;
    newroot->m_father = (*_root)->m_father;
    int newcount = (*_root)->m_treenode_count;

    (*_root)->m_treenode_count -= newroot->m_treenode_count;

    (*_root)->m_left = newroot->m_right;
    if((*_root)->m_left){
        (*_root)->m_treenode_count += (*_root)->m_left->m_treenode_count;
        (*_root)->m_left->m_father = (*_root);
    }

    newroot->m_right = (*_root);
    (*_root)->m_father = newroot;

    (*_root) = newroot;

    (*_root)->m_treenode_count = newcount;
}


void CLRBTree::insert(CRBNode** _root, int _val){

    CRBNode* pos = this->search(*_root, _val);
    if(pos){
        pos->m_count ++;
        return;
    }

    // 插入
    CRBNode** _cur = _root, *cur_father = nullptr;
    while(*_cur){
        cur_father = *_cur;
        (*_cur)->m_treenode_count ++;

        if(_val > (*_cur)->m_val) _cur = &((*_cur)->m_right);
        else _cur = &((*_cur)->m_left);
    }
    (*_cur) = new CRBNode(_val);
    (*_cur)->m_father = cur_father;

    // 修正
    this->fix_after_insert(_cur);
}


// 插入红节点
void CLRBTree::fix_after_insert(CRBNode** _pinsert){
    CRBNode* father = (*_pinsert)->m_father, *grandFather;

    // 只有连续两个红节点才需要旋转
    while(father && father->is_red){

        // 红节点必然不是根节点
        grandFather = father->m_father;
        if(grandFather == nullptr) break;

        // 父节点是左孩子
        if(father == grandFather->m_left){

            // 叔叔节点是红色
            if(grandFather->m_right && grandFather->m_right->is_red){

                // 颜色翻转
                grandFather->m_left->is_red = grandFather->m_right->is_red = false;
                grandFather->is_red = true;

                // 递归判断祖父节点
                _pinsert = &(father->m_father);
                father = grandFather->m_father;

            }
            // 叔叔节点是黑色(空节点是黑节点)
            else{
                // 当前节点是右孩子
                // LR旋转
                if((*_pinsert) == father->m_right){
                    // RR旋转
                    this->RR(&(grandFather->m_left));
                    father = grandFather->m_left;
                    
                }
                // 当前节点是左孩子
                // LL旋转

                // 父节点变黑，祖父节点变红，LL旋转
                father->is_red = false;
                grandFather->is_red = true;
                if(grandFather != this->m_root){
                    if(grandFather->m_father->m_left == grandFather){
                        this->LL(&(grandFather->m_father->m_left));
                    }else{
                        this->LL(&(grandFather->m_father->m_right));
                    }
                }
                else this->LL(&(this->m_root));
            }
        }
        // 父节点是右孩子
        else{
            // 叔叔节点是红色
            if(grandFather->m_left && grandFather->m_left->is_red){

                // 颜色翻转
                grandFather->m_left->is_red = grandFather->m_right->is_red = false;
                grandFather->is_red = true;

                // 递归判断祖父节点
                _pinsert = &(father->m_father);
                father = grandFather->m_father;

            }
            // 叔叔节点是黑色(空节点是黑节点)
            else{
                // 当前节点是左孩子
                // RL旋转
                if((*_pinsert) == father->m_left){
                    // LL旋转
                    this->LL(&(grandFather->m_right));
                    father = grandFather->m_right;
                }
                // 当前节点是右孩子
                // RR旋转

                // 父节点变黑，祖父节点变红，RR旋转
                father->is_red = false;
                grandFather->is_red = true;
                if(grandFather != this->m_root){
                    if(grandFather->m_father->m_left == grandFather){
                        this->RR(&(grandFather->m_father->m_left));
                    }else{
                        this->RR(&(grandFather->m_father->m_right));
                    }
                }
                else this->RR(&(this->m_root));
            }

        }
    }
    // 根节点是黑色
    m_root->is_red = false;
}


bool CLRBTree::erase(CRBNode** _root, int _val){
    // 查找节点
    CRBNode* pos = this->search(*_root, _val);
    if(pos == nullptr) return false;

    if(pos->m_count > 1){
        pos->m_count --;
        return true;
    } 

    // 删除树节点计数
    CRBNode* _cur = *_root;
    while(_cur != pos){
        _cur->m_treenode_count --;
        if(pos->m_val > _cur->m_val) _cur = _cur->m_right;
        else _cur = _cur->m_left;
    }


    // 左右子树都存在
    if(_cur->m_left && _cur->m_right){

        CRBNode* replace_node = nullptr;

        // 如果左子树节点大于右子树节点，则用前驱替代，否则用后继替代
        if(_cur->m_left->m_treenode_count > _cur->m_right->m_treenode_count){
            replace_node = this->get_prior(_cur);
        }
        // 用后继替代
        else{
            replace_node = this->get_post(_cur);
        }

        // 保存替换节点的状态
        int cnt = replace_node->m_count;
        int val = replace_node->m_val;

        // 删除替换节点
        bool res = this->erase(&(_cur), replace_node->m_val);

        // 替换节点信息
        _cur->m_count = cnt;
        _cur->m_val = val;

        // _cur不能再次delete
        return res;

    }
    // 仅左子树存在
    else if(_cur->m_left){
        _cur->m_left->m_father = _cur->m_father;
        if(_cur->m_father){
            if(_cur->m_father->m_left == _cur) _cur->m_father->m_left = _cur->m_left;
            else _cur->m_father->m_right = _cur->m_left;
        }else{
            this->m_root = _cur->m_left;
        }
    }
    // 仅右子树存在
    else if(_cur->m_right){
        _cur->m_right->m_father = _cur->m_father;
        if(_cur->m_father){
            if(_cur->m_father->m_left == _cur) _cur->m_father->m_left = _cur->m_right;
            else _cur->m_father->m_right = _cur->m_right;
        }else{
            this->m_root = _cur->m_right;
        }

    }
    // 是叶节点
    else{
        // 直接删除即可
        if(_cur->m_father){
            if(_cur->m_father->m_left == _cur) _cur->m_father->m_left = nullptr;
            else _cur->m_father->m_right = nullptr;
        }else{
            this->m_root = nullptr;
        }
    }
    

    // 如果删除的是黑节点, 修正
    if(_cur->is_red == false){
        // 是左子树替代
        if(_cur->m_left){
            if(_cur->m_left == this->m_root) this->m_root->is_red = false;
            else this->fix_after_erase(_cur->m_left, _cur->m_father);
        }
        // 是右子树替代
        else if(_cur->m_right){
            if(_cur->m_right == this->m_root) this->m_root->is_red = false;
            else this->fix_after_erase(_cur->m_right, _cur->m_father);
        }
        // 是一个黑色叶节点
        else{
            if(this->m_root) this->fix_after_erase(nullptr, _cur->m_father);
        }
    }

    _cur->m_left = _cur->m_right = nullptr;
    delete _cur;
    return true;
}


void CLRBTree::fix_after_erase(CRBNode* _preplace, CRBNode* _pfather){
    CRBNode* pbrother; // 被删除节点的兄弟节点必然存在，否则两边子树路径的黑节点数量不一致

    // 替代节点是黑节点，且不是根节点(father存在)
    while((!_preplace || !_preplace->is_red) && _pfather){
        // 被删除节点是左子树
        if(_pfather->m_left == _preplace){
            pbrother = _pfather->m_right;

            // 兄弟节点是红色
            if(pbrother->is_red){
                // 父节点必然会是黑色
                _pfather->is_red = true;
                pbrother->is_red = false;

                // RR左旋
                if(_pfather == this->m_root) this->RR(&(this->m_root));
                else{
                    if(_pfather->m_father->m_left == _pfather) this->RR(&(_pfather->m_father->m_left));
                    else this->RR(&(_pfather->m_father->m_right));
                }

                // 左旋后，兄弟节点可能变化了。递归判断
                _preplace = _pfather->m_left;
                pbrother = _pfather->m_right;

            }
            // 兄弟节点是黑色
            else{

                // 兄弟节点的左孩子和右孩子也都是黑色的
                if((!pbrother->m_left || !pbrother->m_left->is_red) && 
                   (!pbrother->m_right || !pbrother->m_right->is_red))
                {  
                    pbrother->is_red = true;
                    if(_pfather->is_red){
                        _pfather->is_red = false;
                        break;
                    }
                    _preplace = _pfather;
                    _pfather = _pfather->m_father;
                    // 递归检查
                }
                else{
                    // 兄弟节点的右孩子是黑色的
                    if(!pbrother->m_right || !pbrother->m_right->is_red){
                    
                        pbrother->m_left->is_red = false;
                        pbrother->is_red = true;

                        // LL右旋
                        this->LL(&(_pfather->m_right));
                        pbrother = _pfather->m_right;
                    }
                    // 兄弟节点的右孩子是红色的
                    pbrother->is_red = _pfather->is_red;  
                    _pfather->is_red = false;
                    pbrother->m_right->is_red = false;

                    // RR左旋
                    if(_pfather == this->m_root) this->RR(&(this->m_root));
                    else{
                        if(_pfather->m_father->m_left == _pfather) this->RR(&(_pfather->m_father->m_left));
                        else this->RR(&(_pfather->m_father->m_right));
                    }

                    _preplace = this->m_root;
                    break;

                }
            }
        }
        // 被删除节点是右子树
        else{
            pbrother = _pfather->m_left;

            // 兄弟节点是红色
            if(pbrother->is_red){
                // 父节点必然会是黑色
                _pfather->is_red = true;
                pbrother->is_red = false;

                // LL右旋
                if(_pfather == this->m_root) this->LL(&(this->m_root));
                else{
                    if(_pfather->m_father->m_left == _pfather) this->LL(&(_pfather->m_father->m_left));
                    else this->LL(&(_pfather->m_father->m_right));
                }

                // 右旋后，兄弟节点可能变化了。递归判断
                _preplace = _pfather->m_right;
                pbrother = _pfather->m_left;
            }
            // 兄弟节点是黑色
            else{

                // 兄弟节点的左孩子和右孩子也都是黑色的
                if((!pbrother->m_left || !pbrother->m_left->is_red) && 
                   (!pbrother->m_right || !pbrother->m_right->is_red))
                {   

                    pbrother->is_red = true;
                    if(_pfather->is_red){
                        _pfather->is_red = false;
                        break;
                    }
                    _preplace = _pfather;
                    _pfather = _preplace->m_father;
                    
                    // 递归检查
                }
                else{
                    // 兄弟节点的右孩子是黑色的
                    if(!pbrother->m_right || !pbrother->m_right->is_red){
                    
                        pbrother->m_left->is_red = false;
                        pbrother->is_red = true;

                        // RR左旋
                        this->RR(&(_pfather->m_left));
                        pbrother = _pfather->m_left;
                    }
                    // 兄弟节点的右孩子是红色的
                    pbrother->is_red = _pfather->is_red;  
                    _pfather->is_red = false;
                    pbrother->m_left->is_red = false;

                    // LL右旋
                    if(_pfather == this->m_root) this->LL(&(this->m_root));
                    else{
                        if(_pfather->m_father->m_left == _pfather) this->LL(&(_pfather->m_father->m_left));
                        else this->LL(&(_pfather->m_father->m_right));
                    }

                    _preplace = this->m_root;
                    break;

                }
            }
        }
    }

    // 替代节点修改为黑节点
    if(_preplace) _preplace->is_red = false;
}