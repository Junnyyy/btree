#include <iostream>
#include <algorithm>
#include <string>
#include <vector>
#include <fstream>
using namespace std;

struct bNode{
  int* keys;
  bNode** children;
  int size;
  bool leaf;
  bNode(int degree){
    keys = new int[degree];
    children = new bNode*[degree+1];
    leaf = true;
    size = 0;
    for(int i = 0; i < degree; i++){
      keys[i] = -1;
    }
    for(int i = 0; i < degree+1; i++){
      children[i] = nullptr;
    }
  }
};

class bTree{
private:
  bNode* root;
  int degree;
  int treeHeight;
  vector<int> record;
  void insertAtLeaf(bNode* parent, bNode* cur, int data);
  void splitChild(bNode* parent, bNode* leftNode);
  void printLevel(bNode* cur, int level, int targetLevel, ofstream &fout);
public:
  bTree(int _degree){
    root = nullptr;
    degree = _degree;
    treeHeight = 0;
  }
  int getTreeHeight(){return treeHeight;}
  void insert(int data, ofstream &fout);
  void printLevel(int targetLevel, ofstream &fout);
};

void bTree::insert(int data, ofstream &fout){
  if(root == nullptr){
    root = new bNode(degree);
    root->keys[0] = data;
    root->size += 1;
    treeHeight = 1;
    cout << "inserted " << data << " at root" << endl;
    record.push_back(data);
  }
  insertAtLeaf(nullptr, root, data);
}
void bTree::insertAtLeaf(bNode* parent, bNode* cur, int data){
  for(int i = 0; i < record.size(); i++){
    // avoid duplicates
    if(record[i] == data){
      cout << data << " duplicated" << endl;
      return;
    }
  }
  if(cur->leaf){
    int i = cur->size;
    while(cur->keys[i-1] > data && i != 0){
      cur->keys[i] = cur->keys[i-1];
      i--;
    }
    cout << "added " << data << endl;
    record.push_back(data);
    cur->keys[i] = data;
    cur->size += 1;
  }else{ // cur is not a leaf
    int i = 0;
    while(cur->keys[i] < data && i < cur->size){
      i++;
    }
    insertAtLeaf(cur, cur->children[i], data);
  }
  if(cur->size == degree){
    if(cur == root){
      bNode* temp = new bNode(degree);
      temp->leaf = false;
      temp->children[0] = cur;
      splitChild(temp, cur);
      root = temp;
      treeHeight += 1;
    }else{
      splitChild(parent, cur);
    }
  }
}
void bTree::splitChild(bNode* parent, bNode* leftNode){
  bNode* rightNode = new bNode(degree);
  int mid = (degree-1)/2;

  int leftNodeSize = leftNode->size;
  int promote = parent->keys[mid];

  int j = 0;
  for(int i = mid+1; i < leftNodeSize; i++){
    rightNode->keys[j] = leftNode->keys[i];
    leftNode->keys[i] = -1;
    leftNode->size -= 1;
    rightNode->size += 1;
    j++;
  }
  if(!leftNode->leaf){
    j = 0;
    for(int i = mid+1; i <= leftNodeSize; i++){
      rightNode->children[j] = leftNode->children[i];
      leftNode->children[i] = nullptr;
      j++;
    }
    rightNode->leaf = false;
  }
  leftNode->size -= 1;
  int i = parent->size;
  while(parent->children[i] != leftNode){
    parent->children[i+1] = parent->children[i];
    parent->keys[i] = parent->keys[i-1];
    i--;
  }
  parent->children[i+1] = rightNode;
  parent->keys[i] = leftNode->keys[mid];
  leftNode->keys[mid] = -1;
  parent->size += 1;
}
void bTree::printLevel(bNode* cur, int curLevel, int targetLevel, ofstream &fout){
  if(curLevel == targetLevel){
    for(int i = 0; i < cur->size; i++){
      cout << cur->keys[i] << " ";
      fout << cur->keys[i] << " ";
    }
  }else{
    for(int i = 0; i <= cur->size; i++){
      printLevel(cur->children[i], curLevel+1, targetLevel, fout);
    }
  }
}
void bTree::printLevel(int targetLevel, ofstream &fout){
  if(root == nullptr || targetLevel > treeHeight){
    cout << "Empty" << endl;
    fout << "Empty" << endl;
    return;
  }
  printLevel(root, 1, targetLevel, fout);
  cout << endl;
  fout << endl;
}