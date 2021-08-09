/*
 * *@*	Description: 
 * *@*	Version: 
 * *@*	Author: yusheng Gao
 * *@*	Date: 2021-08-08 16:49:06
 * *@*	LastEditors: yusheng Gao
 * *@*	LastEditTime: 2021-08-08 16:49:06
 */


#include "src/rb_tree.h"

int main(){

    CLRBTree tree;

    for(int i = 0; i < 10; ++i) {
        tree.insert(i);
        printf("preorder:\t");
        tree.show_preorder();
        printf("inorder:\t");
        tree.show_inorder();
    }
    
    tree.insert(5);

    puts("<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>");
    for(int i = 0; i < 10; ++i) {
        tree.erase(i);
        printf("preorder:\t");
        tree.show_preorder();
        printf("inorder:\t");
        tree.show_inorder();
    }

    return 0;
}