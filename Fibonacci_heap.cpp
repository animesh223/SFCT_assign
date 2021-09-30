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
        void insert(hNode *node,int pos){
            qNode *temp = new qNode(node);
            qNode *q = front;
            int p = 0;
            if(q == NULL){
                front = temp;
                rear = temp;
                num++;
                return;
            }
            if(pos == 0){
                temp->next = front;
                front = temp;
                num++;
                return;
            }
            while(p!=(pos-1)){
                q = q->next;
                p++;
            }
            temp->next = q->next;
            q->next = temp;
            if(temp->next==NULL)
                rear = temp;
            num++;
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
    void cut(hNode*, hNode*); //to cut 
    void cascade_cut(hNode*); //to cut
    hNode* find_node(int value, hNode* ,hNode*,hNode*); //to find a node with particular key value 
    void decrease_key(int old_value,int new_value); //to decrease a particular key 
    void print();// to display th fibonacci heap 
    int find_min(); //to find the minimum element 
    void print(hNode *,hNode *);//
    int find_num();//
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

//To make x parent of y
void FHeap:: fib_link(hNode* y, hNode* x){

    (y->left)->right =y->right;               //remove y from the root list.
    (y->right)->left= y->left;

    if(x->right == x)                   //check if x is the only root node
        H_min=x;

    y->parent=x;                    //assign x as parent of y.

    if(x->child == NULL)         //check if x already has child or not it no make y as the first child.
    {  
        y->left=y;
        y->right=y;
        x->child=y;
    }

    else                       //if x has children add y in the child list.
    {   
        y->right=x->child;
        y->left=x->child->left;
        x->child->left->right=y;
        x->child->left=y;
        if(x->child->key > y->key)       //if y is smallest in the child list make x's child pointer to y
            x->child = y;
    }

    x->degree++;           //increase degree of x.
    y->mark = 'W';
}

//To avoid having 2 nodes with same degree in root list
void FHeap:: consolidated(){
    int i;
    float max=num_H/2;                     //declaring size of auxillary array
    int array_size = max;                     
    hNode* arr[array_size+1];              
    for(int i=0;i <= array_size;i++)        //initializing array elements to NULL
        arr[i]=NULL;
    hNode* ptr1 = H_min;
    while(arr[ptr1->degree] != ptr1)         //Run till we get circled back to the first node.
    {   
        if(H_min->key > ptr1->key)             //If the current node is smaller than minimum make it minimum
            H_min=ptr1;
        if(arr[ptr1->degree]==NULL)           //if the index is NULL make it ptr1
        {    
            arr[ptr1->degree]=ptr1;
            ptr1 = ptr1->right;

        }
        else                                                
        {  
            if((arr[ptr1->degree]->key) > (ptr1->key))     //if some node is already present at index check whether it is smaller
            {                                              // make the greater node chil of smaller node.
                if(arr[ptr1->degree]==H_min)            
                    H_min=ptr1;
                i=ptr1->degree;
                fib_link(arr[ptr1->degree],ptr1);
            }
            else if((arr[ptr1->degree]->key) < (ptr1->key))  
            {
                if(ptr1==H_min)
                    H_min=arr[ptr1->degree];
                i=ptr1->degree;
                fib_link(ptr1,arr[ptr1->degree]);
                ptr1=arr[ptr1->degree];                //now since the ptr1 has become child make it point to the parent.
            }
             arr[i]=NULL;                         //As the degree has changed make the current degree index NULL.
        }

    };
}

//To delete the minimum element from the heap
void FHeap:: delete_min(){
    if(H_min != NULL){                     //check if heap is empty or not
    int val;
    hNode *head = H_min->child;               //point head to the child of minimum
    hNode *temp = head;
    hNode *tempo = H_min;
    H_min = H_min->right;

    if(head != NULL)                     //if the minimus has child
    {
        Queue q;
        temp->parent = NULL;
        q.enqueue(temp);
        while(temp->right != head)       //take all the child and put them inside queue
        {
            temp = temp->right;
            temp->parent = NULL;
            q.enqueue(temp);
        }
        while(q.isempty() != true){      //empty the queue and addd the childs to the root list.
            temp = q.dequeue();
            temp->right = H_min;
            temp->left = H_min->left;
            (H_min->left)->right = temp;
            H_min->left = temp;
            if(H_min->right != H_min)
            if(H_min->key > temp->key)
                H_min = temp;
        }
    }  

    val = tempo->key;
    if(tempo == tempo->right){          //check if it is the last node of the heap.
        H_min = NULL;
        num_H = num_H - 1;
        free(tempo);
        cout<<"Deleted value is "<<val<<endl;
        return;
    }  

    tempo->left->right = tempo->right;        //remove minimum from the root list.
    tempo->right->left = tempo->left;
    tempo->child = NULL;
    H_min=tempo->right;                    
    tempo->left = tempo->right = NULL;
    val = tempo->key;
    

    if(H_min != NULL){
        consolidated();             //call consolidate to reorganize heap
    }
    free(tempo);
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
    //Childs
    fh.fib_insert(100,20);
    fh.fib_insert(200,20);
    fh.fib_insert(300,20);
    fh.fib_insert(110,30);
    fh.fib_insert(120,30);
    fh.fib_insert(130,50);
    fh.fib_insert(150,50);
    fh.fib_insert(210,40);
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
          fh.delete_min();
    fh.print();
           fh.delete_min();
    fh.print();
           fh.delete_min();
    fh.print();
 
}