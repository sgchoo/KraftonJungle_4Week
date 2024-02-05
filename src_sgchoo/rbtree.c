#include "rbtree.h"

#include <stdlib.h>

void LeftRotate(rbtree *tree, node_t *x)
{
  node_t *subNode = x->right;             // subNode of (parent) x status initialize
  x->right = subNode->left;               

  if (subNode->left != tree->nil)         // subNode 왼쪽 자식이 nil노드가 아니라면 x의 오른쪽 자식으로 연결해준다.
    subNode->left->parent = x;

  subNode->parent = x->parent;            // subNode 부모를 x의 부모로 변경해준다.

  if (x->parent == tree->nil)             // tree의 root 부모는 nil로 설정한다. 즉 x의 부모가 nil이면 x가 root인 상태이다.
    tree->root = subNode;
  else if (x == x->parent->right)         // x노드가 x 노드 부모의 오른쪽 자식로 연결되있다면, x 노드 부모의 오른쪽 자식으로 subNode 연결해준다.
    x->parent->right = subNode;
  else                                    // x노드가 x 노드 부모의 왼쪽 자식로 연결되있다면, x 노드 부모의 왼쪽 자식으로 subNode 연결해준다.
    x->parent->left = subNode;

  subNode->left = x;                      // subNode 왼쪽 자식으로 x를 연결해준다.
  x->parent = subNode;                    // x의 부모를 subNode 변경해준다.
}

// LeftRotate와 동일하다.
void RightRotate(rbtree *tree, node_t *x)
{
  node_t *subNode = x->left;
  x->left = subNode->right;

  if(subNode->right != tree->nil)
    subNode->right->parent = x;

  subNode->parent = x->parent;

  if(x->parent == tree->nil)
    tree->root = subNode;
  else if(x == x->parent->left)
    x->parent->left = subNode;
  else
    x->parent->right = subNode;

  subNode->right = x;
  x->parent = subNode;
}

void RbInsertFixed(rbtree *tree, node_t *childNode)
{
  node_t *grandNode = childNode->parent->parent;
  node_t *parentNode = childNode->parent;
  node_t *siblingNode = grandNode->left == parentNode ? grandNode->right : grandNode->left;

  while(parentNode->color == RBTREE_RED)
  {
    if(siblingNode->color == RBTREE_RED && grandNode->color == RBTREE_BLACK)
    {
      parentNode->color = RBTREE_BLACK;
      siblingNode->color = RBTREE_BLACK;
      grandNode->color = RBTREE_RED;
    }

    else if(siblingNode->color != RBTREE_RED)
    {
      if(grandNode->left == parentNode)
      {
        if(parentNode->right == childNode)
        {
          LeftRotate(&tree, &parentNode);
          parentNode->color = RBTREE_BLACK;
          grandNode->color = RBTREE_RED;
          RightRotate(&tree, &grandNode);
        }
        else
        {
          parentNode->color = RBTREE_BLACK;
          grandNode->color = RBTREE_RED;
          RightRotate(&tree, &grandNode);
        }
      }

      else
      {
        if(parentNode->left == childNode)
        {
          RightRotate(&tree, &parentNode);
          parentNode->color = RBTREE_BLACK;
          grandNode->color = RBTREE_RED;
          LeftRotate(&tree, &grandNode);
        }
        else
        {
          parentNode->color = RBTREE_BLACK;
          grandNode->color = RBTREE_RED;
          RightRotate(&tree, &grandNode);
        }
      }
    }
  }

  tree->root->color = RBTREE_BLACK;
}

rbtree *new_rbtree(void) {
  rbtree *p = (rbtree *)calloc(1, sizeof(rbtree));
  node_t *nil = (node_t *)calloc(1, sizeof(node_t));

  nil->color=RBTREE_BLACK;

  p->nil = p->root = nil;

  return p;
}

void delete_rbtree(rbtree *t) {
  // TODO: reclaim the tree nodes's memory
  free(t);
}

node_t *rbtree_insert(rbtree *t, const key_t key) {
  // TODO: implement insert
  node_t *newNode = (node_t *)malloc(sizeof(node_t));           // 생성할 노드 메모리 할당
  newNode->key = key;                                           // 할당된 노드의 key값 초기화

  node_t *parentNode = t->nil;                                  // 부모 노드 저장용 노드 변수
  node_t *tempNode = t->root;                                   // 생성 노드 위치 탐색용 노드 변수

  while(tempNode != t->nil)                                     // 이진 탐색 하여 leaf노드(nil 노드)까지 탐색
  {
    parentNode = tempNode;

    if(key < tempNode->key)
      tempNode = tempNode->left;
    else
      tempNode = tempNode->right;
  }

  newNode->parent = parentNode;                                 // 생성 노드에 부모를 연결해준다.

  if(parentNode == t->nil)                                      // 만약 parentNode의 부모가 t.nill이라면 즉 아무 노드도 삽입되어 있지 않다면 newNode를 root로 설정해준다.
    t->root = newNode;
  else if(parentNode->key > newNode->key)                       // 부모 왼쪽, 오른쪽 연결 위치 탐색
    parentNode->left = newNode;
  else
    parentNode->right = newNode;

  newNode->left = t->nil;                                       // 생성된 노드의 자식을 모두 nil로 연결한다.
  newNode->right = t->nil;
  newNode->color = RBTREE_RED;                                  // 삽입된 노드의 색을 red로 설정한다.

  RbInsertFixed(&t, &newNode);                                              // RB Tree 속성을 위반하였는지 검사한다.

  return t->root;
}

node_t *rbtree_find(const rbtree *t, const key_t key) {
  // TODO: implement find
  return t->root;
}

node_t *rbtree_min(const rbtree *t) {
  // TODO: implement find
  return t->root;
}

node_t *rbtree_max(const rbtree *t) {
  // TODO: implement find
  return t->root;
}

int rbtree_erase(rbtree *t, node_t *p) {
  // TODO: implement erase
  return 0;
}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
  // TODO: implement to_array
  return 0;
}
