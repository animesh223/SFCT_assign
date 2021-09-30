#include <iostream>
#include <fstream>
#include <string>
#include <cmath>

using namespace std;

//Node of Fibonacci heap
class hNode{
public:
    hNode * parent;  //pointer to the parent
    hNode * child;   //pointer to the child
    hNode * left;    //pointer to the left node
    hNode * right;   //pointer to the right node
    int degree;      //Degree of the node
    int key;         //Value of the node
    char mark;       //mark of the node
    hNode(){
    parent = NULL;
    child = NULL;
    left = right = NULL;
    degree = 0;
    mark = 'W';      //W -> unmarked B -> marked
    }
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
        int num;
public:
        Queue(){
            front = NULL;
            rear = NULL;
            num = 0;
        }
        void enqueue(hNode *x){
            qNode *temp = new qNode(x);
            if(front == NULL){
                front = rear = temp;
                num++;
                return;
            }
            rear->next = temp;
            rear = temp;
            num++;
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
            num--;
            delete(tempq);
            return temp;
        }
        bool isempty(){
            if(front == NULL)
                return true;
            else
                return false;
        }
};

// Class of Fibonacci heap
class FHeap{

    hNode *H_min;
    int num_H;
public:
    FHeap(){
        H_min = NULL;
        num_H = 0;
    }
    void fib_insert(int,int);
    void fib_link(hNode*, hNode*); 
    void consolidated();
    void delete_min();  
    void cut(hNode*, hNode*); 
    void cascade_cut(hNode*); 
    hNode* find_node(int value, hNode* ,hNode*,hNode*); 
    void decrease_key(int old_value,int new_value); 
    void print();
    void print(hNode *,hNode *);
    int find_min(); 
    int find_num();
    int find_deg(int);
};

//insert in fibonacci heap
void FHeap:: fib_insert(int value,int node = 0){
    if(node == 0){
        hNode *x = new hNode;
        x->key = value;
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
    return;
    }
    if(node != 0){
        hNode *node_found = NULL;
        node_found = find_node(node,H_min,H_min,node_found);
        if(node_found){
            hNode *x = new hNode;
            x->key = value;
            x->parent = node_found;
            hNode *nd = node_found->child;
            if (nd == NULL){
            x->left = x;
            x->right = x;
            node_found->child = x;   
            }
            else{
            x->right = nd;
            x->left = nd->left;
            (nd->left)->right = x;
            nd->left = x;
            if(x->key < nd->key)
                node_found->child = x;
            }
            node_found->degree++; 
            num_H = num_H + 1;
        }
    return;
    }

}

void FHeap:: fib_link(hNode* y, hNode* x){
    (y->left)->right =y->right;
    (y->right)->left= y->left;
    if(x->right == x)
        H_min=x;
    y->parent=x;
    if(x->child == NULL)
    {  
        y->left=y;
        y->right=y;
        x->child=y;
    }
    else
    {   
        y->right=x->child;
        y->left=x->child->left;
        x->child->left->right=y;
        x->child->left=y;
        if(x->child->key > y->key)
            x->child = y;
    }
    x->degree++;
    y->mark = 'W';
}

void FHeap:: consolidated(){
    int i;
    float size=(log(num_H)) / (log(2));
    int int_size = size+1;
    hNode* D[int_size+1];
    for(int i=0;i <= int_size;i++)
        D[i]=NULL;
    hNode* p = H_min;
    while(D[p->degree] != p)
    {
        if(H_min->key > p->key)      
            H_min=p;
        if(D[p->degree]==NULL)   
        {
            D[p->degree]=p;
            p = p->right;
        }
        else                     
        {
            if((D[p->degree]->key) > (p->key))
            {
                if(D[p->degree]==H_min)
                    H_min=p;
                i=p->degree;
                fib_link(D[p->degree],p);
                D[i]=NULL;
            }
            else if((D[p->degree]->key) < (p->key))
            {
                if(p==H_min)
                    H_min=D[p->degree];
                i=p->degree;
                fib_link(p,D[p->degree]);
                p=D[p->degree];
                D[i]=NULL;
            }
        }

    };
}

void FHeap:: delete_min(){
    if(H_min != NULL){
    int val;
    hNode *head = H_min->child;
    hNode *temp = head;
    hNode *tempo = H_min;
    H_min = H_min->right;
    if(head != NULL)
    {
        Queue q;
        temp->parent = NULL;
        q.enqueue(temp);
        while(temp->right != head)
        {
            temp = temp->right;
            temp->parent = NULL;
            q.enqueue(temp);
        }
        while(q.isempty() != true){
            temp = q.dequeue();
            if(H_min->right == H_min){
                temp->right = H_min;
                temp->left = H_min->left;
                (H_min->left)->right = temp;
                H_min->left = temp;
                H_min = temp;
            }
            else{
            temp->right = H_min;
            temp->left = H_min->left;
            (H_min->left)->right = temp;
            H_min->left = temp;
            if(H_min->key > temp->key)
                H_min = temp;
            }
        }
    }  

    val = tempo->key;
    if(tempo == tempo->right){
        H_min = NULL;
        num_H = num_H - 1;
        free(tempo);
        cout<<"Deleted value is "<<val<<endl;
        return;
    } 
    tempo->left->right = tempo->right;
    tempo->right->left = tempo->left;
    tempo->child = NULL;
    tempo->left = tempo->right = NULL;
    free(tempo);
    consolidated();
    num_H = num_H - 1;
    cout<<"Deleted value is "<<val<<endl;
    }
    else{
        cout<<"Nothing to delete"<<endl;
    }
}

void FHeap:: cut(hNode *node, hNode *parent){

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

hNode* FHeap:: find_node(int value, hNode *temp,hNode *head,hNode *node_found){
    if(temp != NULL){
        
        hNode *node_temp = temp;

    if(node_temp->key == value){
        node_found = node_temp;
        return node_found;
        }

    if (node_found == NULL) {
        if (node_temp->child != NULL)
            node_found = find_node(value,node_temp->child,node_temp->child,node_found);
        if ((node_temp->right)!=head)
            node_found = find_node(value,node_temp->right,head,node_found);
        }
    }
    return node_found;
}

void FHeap:: decrease_key(int old_val,int new_val){
    int old_value = old_val;
    int new_value = new_val;
    hNode *node_index = NULL;
    hNode *node_found = NULL;
    node_index = find_node(old_value,H_min,H_min,node_found);
    if(H_min==NULL)
    {
        cout<<"Heap is empty"<<endl;
        return ;
    }

    else if(node_index == NULL)
    {
        cout<<"Value is not present in Fib Heap"<<endl;
        return ;
    }

    if(old_value < new_value)
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

void FHeap:: print(){
    if(H_min == NULL)
        cout<<"Heap is empty";
    if(H_min != NULL)
        print(H_min,H_min);
    cout<<"\n";
}

void FHeap:: print(hNode *temp,hNode *tempa){
        if(temp != NULL){
        cout<<temp->key<<" ";
        print(temp->child,temp->child);
        if(temp->right!=tempa)
            print(temp->right,tempa);
        }
}

// to get the minimum element in the heap
int FHeap:: find_min(){
    return H_min->key;
};

int FHeap:: find_num(){
    return num_H;
}

int FHeap:: find_deg(int val){
    
        hNode *node_found = NULL;
        node_found = find_node(val,H_min,H_min,node_found);
        return node_found->degree;
}
int main()
{
    FHeap fh;
    //Roots
    fh.fib_insert(10);
    fh.fib_insert(20);
    fh.fib_insert(30);
    fh.fib_insert(40);
    fh.fib_insert(50);
    fh.fib_insert(60);
    fh.fib_insert(70);
    fh.fib_insert(80);
    fh.fib_insert(90);
    //Childs
    fh.fib_insert(100,20);
    fh.fib_insert(200,20);
    fh.fib_insert(300,20);
    fh.fib_insert(110,30);
    fh.fib_insert(120,30);
    fh.fib_insert(130,30);
    fh.print();
    fh.decrease_key(130,1);
    fh.print();
    fh.delete_min();
    fh.print();
    fh.decrease_key(120,4);
    fh.print();
    fh.delete_min();
    fh.print();
    fh.delete_min();
    fh.print();
    fh.delete_min();
    fh.print();
    fh.delete_min();
    fh.print();
    fh.delete_min();
    fh.print();
    fh.delete_min();
    fh.print();
    fh.decrease_key(300,150);
    fh.print();
    fh.delete_min();
    fh.print();
    // cout<<"\n"<<fh.find_deg(40);
    // cout<<"\n"<<fh.find_deg(120);
    // cout<<"\n"<<fh.find_min();
    // cout<<"\n"<<fh.find_num();
}