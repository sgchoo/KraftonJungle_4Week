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
  node_t *grandNode = childNode->parent->parent;            // 할아버지 노드를 선언한다.
  node_t *parentNode = childNode->parent;                   // 부모 노드를 선언한다.

  if(childNode == tree->root)                               // 만약, 자식 노드가 root노드이면, 색만 바꿔준 후 return한다.
  {
    childNode->color = RBTREE_BLACK;
    return;
  }

  // 부모의 형제 노드를 선언한다.
  node_t *siblingNode = grandNode->left == parentNode ? grandNode->right : grandNode->left;   

  while(parentNode->color == RBTREE_RED)                                        // 부모의 node color가 검은색이 될 때까지 반복해준다.
  {
    if(siblingNode->color == RBTREE_RED && grandNode->color == RBTREE_BLACK)    // case 1번의 경우
    {
      parentNode->color = RBTREE_BLACK;
      siblingNode->color = RBTREE_BLACK;
      grandNode->color = RBTREE_RED;
      RbInsertFixed(tree, grandNode);
    }

    else if(siblingNode->color != RBTREE_RED)                                   // 부모의 색과 형제의 색이 다르면, case 2, 3을 판별한다.
    {
      if(grandNode->left == parentNode)                                         // 부모의 위치가 어디인지 정하여 rotation 방향을 정한다.
      {
        if(parentNode->right == childNode)                                      // case 2
        {
          LeftRotate(tree, parentNode);
          parentNode->color = RBTREE_BLACK;
          grandNode->color = RBTREE_RED;
          RightRotate(tree, grandNode);
          RbInsertFixed(tree, grandNode);
        }
        else                                                                    // case 3
        {
          parentNode->color = RBTREE_BLACK;
          grandNode->color = RBTREE_RED;
          RightRotate(tree, grandNode);
          RbInsertFixed(tree, grandNode);
        }
      }

      else
      {
        if(parentNode->left == childNode)                                       // case 2
        {
          RightRotate(tree, parentNode);
          parentNode->color = RBTREE_BLACK;
          grandNode->color = RBTREE_RED;
          LeftRotate(tree, grandNode);
          RbInsertFixed(tree, grandNode);
        }
        else                                                                    // case 3
        {
          parentNode->color = RBTREE_BLACK;
          grandNode->color = RBTREE_RED;
          RightRotate(tree, grandNode);
          RbInsertFixed(tree, grandNode);
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
  // if(t->root)
  //   free(t->root);
  free(t->nil);
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

  RbInsertFixed(t, newNode);                                    // RB Tree 속성을 위반하였는지 검사한다.

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

void RBTreeTransplant(rbtree *tree, node_t *deleteNode, node_t *childNode)
{
  if(deleteNode->parent == tree->nil)                       // 삭제하려는 노드가 root노드이면, 그 자식 노드를 root노드로 바꿔준다.
    tree->root = childNode;
  else if(deleteNode->parent->left == deleteNode)           // 삭제하려는 노드의 위치에 따라 자식 노드 위치를 설정한다.
    deleteNode->parent->left = childNode;
  else
    deleteNode->parent->right = childNode;

  childNode->parent = deleteNode->parent;                   // 자식 노드의 부모를 삭제하려는 노드의 부모로 바꿔준다.
}

node_t *GetSuccessor(rbtree *tree, node_t *successor)
{
  node_t *tempNode = successor;

  while(tempNode->left != tree->nil)
  {
    tempNode = tempNode->left;
  }

  return tempNode;
}

void RbDeleteFixed(rbtree *tree, node_t *doublyBlack)
{
  // root에 extra black이 생긴 상황은 상관없으므로 root가 아닐때와, doubly black의 상황일때 작동한다.
  // 여기서 sibling은 doubly black의 형제 노드를 뜻한다.
  while(doublyBlack != tree->root && doublyBlack->color == RBTREE_BLACK)
  {
    if(doublyBlack == doublyBlack->parent->left)                                                    // doubly black의 위치를 회전 방향을 위해 파악한다.
    {
      node_t *sibling = doublyBlack->parent->right;

      if(sibling->color == RBTREE_RED)                                                              // case 1
      {
        sibling->color = RBTREE_BLACK;
        doublyBlack->parent->color = RBTREE_RED;
        LeftRotate(tree, doublyBlack->parent);
        sibling = doublyBlack->parent->right;
      }

      if(sibling->left->color == RBTREE_BLACK && sibling->right->color == RBTREE_BLACK)             // case 2
      {
        sibling->color = RBTREE_RED;
        doublyBlack = doublyBlack->parent;
        RbDeleteFixed(tree, doublyBlack);
      }
      else if(sibling->right->color == RBTREE_BLACK)                                               
      {
          sibling->left->color = RBTREE_BLACK;                                                      // case 3
          sibling->color = RBTREE_RED;
          RightRotate(tree, sibling);
          sibling = doublyBlack->parent->right;
        sibling->color = doublyBlack->parent->color;                                                // case 4
        doublyBlack->parent->color = RBTREE_BLACK;
        LeftRotate(tree, doublyBlack->parent);
        doublyBlack = tree->root;
      }
    }
    else
    {
      node_t *sibling = doublyBlack->parent->left;

      if(sibling->color == RBTREE_RED)                                                               // case 1
      {
        sibling->color = RBTREE_BLACK;
        doublyBlack->parent->color = RBTREE_RED;
        LeftRotate(tree, doublyBlack->parent);
        sibling = doublyBlack->parent->left;
      }

      if(sibling->left->color == RBTREE_BLACK && sibling->right->color == RBTREE_BLACK)              // case 2
      {
        sibling->color = RBTREE_RED;
        doublyBlack = doublyBlack->parent;
        RbDeleteFixed(tree, doublyBlack);
      }
      else if(sibling->left->color == RBTREE_BLACK)                                               
      {
          sibling->right->color = RBTREE_BLACK;                                                      // case 3
          sibling->color = RBTREE_RED;
          RightRotate(tree, sibling);
          sibling = doublyBlack->parent->left;
        sibling->color = doublyBlack->parent->color;                                                 // case 4
        doublyBlack->parent->color = RBTREE_BLACK;
        LeftRotate(tree, doublyBlack->parent);
        doublyBlack = tree->root;
      }
    }
  }
}

int rbtree_erase(rbtree *t, node_t *p) {
  // TODO: implement erase

  node_t *backupNode = p;                                             // 삭제용 node 저장
  color_t backupNodeOgColor = p->color;                               // 삭제용 node의 컬러 저장

  node_t *tempNode;

  if(backupNode->left == t->nil)                                      // 만약 nil노드를 제외한 자식이 한명 이하인 경우
  {
    tempNode = backupNode->right;
    RBTreeTransplant(t, backupNode, backupNode->right);
  }
  else if(backupNode->right == t->nil)                                // 만약 nil노드를 제외한 자식이 한명 이하인 경우
  {
    tempNode = backupNode->left;
    RBTreeTransplant(t, tempNode, backupNode->left);
  }
  else                                                                // 만약 자식 노드가 두개라면
  {
    backupNode = GetSuccessor(t, p->right);                           // backupNode successor를 저장한다.
    backupNodeOgColor = backupNode->color;                            // origin color에 successor color를 저장한다.
    tempNode = backupNode->right;                                     // successor의 왼쪽 자식은 없기때문에 오른쪽 자식을 넣어준다.

    if(backupNode->parent == p)                                       // 삭제노드와 backupNode(successor)의 부모가 같을 경우
      tempNode->parent = backupNode;                                  // tempNode가 nil일 경우 backupNode 연결해준다.
    else
    {
      RBTreeTransplant(t, backupNode, backupNode->right);             // backupNode와 nil(or 유효한 값의 child)의 자리를 바꿔준 후
      backupNode->right = p->right;                                   // 바뀐 backupNode와 p의 오른쪽 자식을 연결해준다.
      backupNode->right->parent = backupNode;                         
    }
    RBTreeTransplant(t, p, backupNode);                               // 삭제할 노드(p)와 backupNode와 자리를 교체해준 후
    backupNode->left = p->left;                                       // backupNodedml p의 왼쪽 자식도 연결해준다.
    backupNode->left->parent = backupNode;
    backupNode->color = p->color;                                     // backupNode의 색은 삭제한 노드(p)의 색으로 변경한다.
  }

  if(backupNodeOgColor == RBTREE_BLACK)                               // successor의 색이 black이였다면 extra black이 생기므로 속성 검사를 한다.
    RbDeleteFixed(t, tempNode);
    
  return 0;
}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
  // TODO: implement to_array
  return 0;
}
