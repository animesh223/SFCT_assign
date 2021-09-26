#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>

using namespace std;

//Node of Fibonacci heap
class hNode{
public:
    hNode * parent;   //pointer to the parent
    hNode * child;    //pointer to the child
    hNode * left;     //pointer to the left node
    hNode * right;    //pointer to the right node
    int degree;       //Degree of the node
    int key;          //Value of the node
    char mark;        //mark of the node
    char C;           //indicator
};

//Node of a queue
class qNode{
public:
    hNode *data;
    qNode *next;
    qNode(hNode *data)
    {
        this->data = data;
        next = NULL;
    }
};

//Class of a queue
class Queue{

        qNode *front;
        qNode *rear;
public:
        Queue(){
            front = NULL;
            rear = NULL;
        }
        void enqueue(hNode *x){
            qNode *temp = new qNode(x);
            if(rear == NULL){
                front = rear = temp;
                return;
            }
            rear->next = temp;
            rear = temp;
        }
        hNode* dequeue(){
            hNode *temp;
            qNode *tempq;
            if(front == NULL)
                return NULL;
            temp = front->data;
            tempq = front;
            front = front->next;
            if(front == NULL)
                rear = NULL;
            delete(tempq);
            return temp;
        }
        void display(){
            qNode *temp = front;
            if(front == NULL)
                return;
            while(temp!=NULL){
                cout<<temp->data->key<<" ";
                temp = temp->next;
            }
        }
};

// Class of Fibonacci heap
class FHeap{

    hNode* H_min;
    int num_H;

public:

    void make_heap();  //to initialize a heap (Done)
    void fib_insert(int); //to insert a node in the parent root nodes (Done)
    void fib_link(hNode*, hNode*); // to link (Aman(1))
    void consolidated();// (Aman(2))
    void delete_min();  //to delete the minimum element (Aman(3))
    void cut(hNode*, hNode*); //to cut (kashish(1.1))
    void cascade_cut(hNode*); //to cut (kashish(1.2))
    hNode* find_node(int value, hNode* ); //to find a node with particular key value (Kashish(2))
    void decrease_key(int old_value,int new_value); //to decrease a particular key (Kashish(3))
    void read_heap();  //to read the heap from the file (Animesh(1))
    void fib_union();  //to union (Animesh(2))
    void display();// to display th fibonacci heap (Animesh(3))
    int find_min(); //to find the minimum element (Done)
};

void FHeap:: make_heap(){
    H_min = NULL;
    num_H = 0;
}

//insert in fibonacci heap
void FHeap:: fib_insert(int value){
    hNode *x = new hNode;
    x->degree = 0;
    x->mark = 'W';  //W -> unmarked B -> marked
    x->C='N';
    x->parent = NULL;
    x->child = NULL;
    if (H_min == NULL){
        x->left = x;
        x->right = x;
        H_min = x;    
    }
    else{
        x->right = H_min;
        x->left = H_min->left;
        (H_min->left)->right = x;
        H_min->left = x;
        if(x->key < H_min->key)
            H_min = x;
    }
    num_H = num_H + 1;
}

/*
void FHeap:: fib_link(hNode* Hp, hNode* y, hNode* x){


}
*/

/*
void FHeap:: consolidated(){

}
*/

/*
void FHeap:: delete_min(){

}
*/


void FHeap:: cut(hNode *node, hNode * parent){

if(node==node->right)
parent->child=NULL;
(node->left)->right = node->right;
(node->right)->left= node->left;
if(parent->child == node)
parent->child= node->right;
parent->degree = parent->degree -1;
node->left= node;
node->right=node;
node->right=H_min;
node->left= H_min->left;
(H_min->left)->right = node;
H_min->left =node;
node->parent = NULL;
node->mark='B';

}

void FHeap :: cascade_cut(hNode *ptr1)
{
    hNode *ptr=ptr1;
    hNode *parent= ptr->parent;
    if(parent !=NULL)
    {
        if(parent->mark =='W')
        parent->mark='B';
        else
        {
            cut(ptr, parent);
            cascade_cut(parent);
        }
    }
}


hNode* FHeap:: find_node(int value, hNode *temp){
    hNode *node_found=NULL;
    hNode *node_temp=temp;
    node_temp-> C= 'Y';   //true
    if(node_temp->key==value)
    {
    node_found=node_temp;
    node_temp->C='N';
    return node_found;
    }

  if (node_found == NULL) {
    if (node_temp->child != NULL)
      node_found = find_node(value,node_temp->child);
    if ((node_temp->right)->C != 'T')
      node_found = find_node(value,node_temp->right);
  }

  node_temp->C = 'N';
  return node_found;
}



void FHeap:: decrease_key(int old_val,int new_val){
    int old_value= old_val;
    int new_value= new_val;
    hNode *min= H_min;
    hNode *node_index= find_node(old_value,min);
    if(min==NULL)
    {
    cout<<"Heap is empty"<<endl;
     return ;
    }
    
    else if(node_index == NULL)
    {
        cout<<"Value is not present in Fib Heap"<<endl;
        return ;
    }
    if(old_value< new_value)
    {
        cout<<"Node value is smaller than entered key value"<<endl;
        return ;
    }
    node_index->key= new_value;
    hNode *parent= node_index->parent;
    if(parent!=NULL && parent->key > node_index-> key)
    {
        cut(node_index,parent);
        cascade_cut(parent);
    }
  if (node_index->key < H_min->key)
        H_min = node_index;
}


//Function to read the fibonacci heap from a file 
/*
void FHeap:: read_heap(){
    ifstream F_in;
    string location;
    cout<<"Enter the location of the file "<<endl;
    getline (cin, location);
    F_in.open(location);
}
*/

/*
void FHeap:: fib_union(){


}
*/

/*
void FHeap::display(){

}
*/

// to get the minimum element in the heap
int FHeap:: find_min(){
    return H_min->key;
};

int main()
{
    FHeap fh;
    fh.make_heap();
    //(animesh)
}