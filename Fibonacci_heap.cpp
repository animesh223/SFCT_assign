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
    void make_heap();  //to initialize a heap (Done)(+)
    //void fib_insert_root(int); //to insert a node in the parent root nodes (Done)(+)
    void fib_insert(int,int);
    void fib_link(hNode*, hNode*); // to link (Aman(1))(-)
    void consolidated();// (Aman(2))(-)
    void delete_min();  //to delete the minimum element (Aman(3))(-)
    void cut(hNode*, hNode*); //to cut (kashish(1))(+)
    void cascade_cut(hNode*); //to cut (kashish(1))(+)
    hNode* find_node(int value, hNode* ,hNode*,hNode*); //to find a node with particular key value (Kashish(2))(+)
    void decrease_key(int old_value,int new_value); //to decrease a particular key (Kashish(3))(+)
    void print();// to display th fibonacci heap (Animesh(2))(+)
    int find_min(); //to find the minimum element (Done)(+)
    void print(hNode *,hNode *);//(+)
    int find_num();//(+)
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

/*
void FHeap:: fib_insert_root(int value){
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
}
*/

void FHeap:: fib_link(hNode* x, hNode* y){
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
        
        y->right=x->child->right;
        y->left=x->child;
        x->child->right->left=y;
        x->child->right=y;

    }
    if(x->child != NULL)
    {
        if(y->key < (x->child)->key)
            x->child=y; 
    }

    x->degree++;
    y->mark=false;
//cout<<"4"<<endl;
/*
   temp1->left->right=temp1->right;     //unlink temp1 from root nodes
    temp1->right->left=temp1->left;
    if(temp2->right==temp2)
        H_min=temp2;
    temp1->parent=temp2;                     //make parent of temp1=temp2
    if(temp2->child==NULL)                 //if temp2 child is NULL
    {
        temp2->child=temp1;
        temp1->left=temp1;
        temp1->right=temp1;
    }
    else
    {
        temp1->right=temp2->child->right;
        temp1->left=temp2->child;
        temp2->child->right->left=temp1;
        temp2->child->right=temp1;
    }                                           //else
    //if (temp1->key < (temp2->child)->key)
    //    temp2->child = temp1;
    temp1->mark='W';
    temp2->degree++;
*/
}

void FHeap:: consolidated(){
  /*  
   int i;
    float max_size=(log(num_H)) / (log(2));
    int array_size = max_size;
    hNode* arr[array_size+1];
    for(int i=0;i <= array_size;i++)
        arr[i]=NULL;
    hNode* ptr1 = H_min;
    while(arr[ptr1->degree] != ptr1)
    {
        if(H_min->key > ptr1->key)      
            H_min=ptr1;
        if(arr[ptr1->degree]==NULL)   
        {
            arr[ptr1->degree]=ptr1;
            ptr1 = ptr1->right;
        }
        else                     
        {
            if((arr[ptr1->degree]->key) > (ptr1->key))
            {
                if(arr[ptr1->degree]==H_min)
                    H_min=ptr1;
                i=ptr1->degree;
                fib_link(arr[ptr1->degree],ptr1);
                arr[i]=NULL;
            }
            else if((arr[ptr1->degree]->key) < (ptr1->key))
            {
                if(ptr1==H_min)
                    H_min=arr[ptr1->degree];
                i=ptr1->degree;
                fib_link(ptr1,arr[ptr1->degree]);
                ptr1=arr[ptr1->degree];
                arr[i]=NULL ;
            }
        }

    }
    }

*/
    int i;
    float max_size=(log(num_H)) / (log(2));
    int array_size = max_size;
    hNode* arr[array_size+1];
    for(int i=0;i<=array_size;i++)
        arr[i]=NULL;
    hNode* ptr1=H_min;
    do
    {
        if(H_min->key > ptr1->key)      //update minimum
            H_min=ptr1;
        if(arr[ptr1->degree]==NULL)   //check empty
        {
            arr[ptr1->degree]=ptr1;
            ptr1=ptr1->right;
        }
        else                     //union
        {
            if((arr[ptr1->degree]->key) > (ptr1->key))
            {
                if(arr[ptr1->degree]==H_min)
                    H_min=ptr1;
                i=ptr1->degree;
                fib_link(arr[ptr1->degree],ptr1);
                arr[i]=NULL;
            }
            else if((arr[ptr1->degree]->key) < (ptr1->key))
            {
                if(ptr1==H_min)
                    H_min=arr[ptr1->degree];
                i=ptr1->degree;
                fib_link(ptr1,arr[ptr1->degree]);
                ptr1=arr[ptr1->degree];
                arr[i]=NULL;
            }
        }

    }while(arr[ptr1->degree]!=ptr1);
}


void FHeap:: delete_min(){
hNode *temp =H_min;
    hNode *store_child=temp;
    hNode *x=NULL;
    cout << "Current Min:"<< H_min->key<<endl;
    if(temp->child !=NULL)
    {
        x= temp->child;
        do
        {
    
            store_child= x->right;
            (H_min->left)->right= x;
            x->left= H_min->left;
            x->right= H_min;
            H_min->left= x;
            if(x->key < H_min->key)
                H_min=x;
            x=store_child;
            x->parent=NULL;
        }while(store_child != temp->child); 
    }                                       // add all the nodes into the root list

    (temp->right)->left= temp->left;
    (temp->left)->right= temp->right;       // min is eliminated from the root list

    H_min = temp->right;                 

    if(temp == temp ->right) {              // only root in the list
        H_min= NULL;
    }
    else 
    {
                          
        H_min= temp->right;
        consolidated();
    }

    num_H = num_H - 1;
    cout<<"Min is deleted."<<endl;
   cout<<"New min: "<<H_min->key<<endl;
    free(temp);

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

//Function to read the fibonacci heap from a file 
void FHeap:: make_heap(){
    ifstream F_in;
    string location;
    cout<<"Enter the name of the file "<<endl;
    getline (cin, location);
    F_in.open(location);
    if(!F_in){
        cout << "Error: file could not be opened" << endl;
        exit(0);
    }
    else{
        Queue q;
        int val,num;
        F_in>>num;
        num_H = num;
        hNode *temp,*front,*back;
        temp = back = front = NULL;

        for(int i=0;i<num;i++){
            F_in>>val;
            temp = NULL;
            temp = new hNode;
            temp->key = val;
            if(i != 0){
                temp->right = back;
                back->left = temp;
            }
            back = temp;
            if(i == 0){
                front = temp;
                H_min = temp;
            }
            if(i == (num-1)){
                back->left = front;
                front->right = back;
            }
            if(back->key < H_min->key)
                H_min = back;
            q.enqueue(back);
        }
        while(q.isempty() != true){
            hNode *tempq;
            F_in>>val>>num;
            // cout<<"\nHeres "<<val<<"   "<<num<<endl;
            tempq = q.dequeue();
            num_H += num;
            // cout<<"\n"<<num_H<<endl;
            if(num != 0){
                temp = front = back = NULL;
                tempq->degree = num;
                for(int i=0;i<num;i++){
                    F_in>>val;
                    // cout<<"\n"<<val<<endl;
                    temp = NULL;
                    temp = new hNode;
                    temp->parent = tempq;
                    temp->key = val;
                    if(i!=0){
                        temp->left = back;
                        back->right = temp;
                        if(tempq->child->key > temp->key)
                            tempq->child = temp;
                    }
                    back = temp;
                    if(i==0){
                        front = temp;
                        tempq->child = temp;
                    }
                    if(i==(num-1)){
                        back->right = front;
                        front->left = back;
                    }
                    q.insert(back,i);
                }
            }
        }
    }
    F_in.close();
}

void FHeap:: print(){
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

int FHeap:: find_deg(int val){
    
        hNode *node_found = NULL;
        node_found = find_node(val,H_min,H_min,node_found);
        return node_found->degree;
}

// to get the minimum element in the heap
int FHeap:: find_min(){
    return H_min->key;
};

int FHeap:: find_num(){
    return num_H;
}

int main()
{
    FHeap fh;
    //Roots
        fh.fib_insert(2);
    // fh.fib_insert(7,2);
    // fh.fib_insert(8,2);
    fh.fib_insert(3);
    fh.fib_insert(4);
    fh.fib_insert(5);
    fh.fib_insert(6);
    //Childs
    fh.fib_insert(7,2);
    fh.fib_insert(8,2);
    fh.fib_insert(9,2);
    fh.fib_insert(13,7);
    fh.fib_insert(14,7);
    fh.fib_insert(10,4);
    fh.fib_insert(11,4);
    fh.fib_insert(12,5);
    fh.fib_insert(100);

/*
   fh.fib_insert(1);
    fh.fib_insert(2);
    fh.fib_insert(3);
    fh.fib_insert(4);
    fh.fib_insert(5);
    fh.fib_insert(6);
 //   fh.fib_insert(14);
    cout << "Root Nodes : ";
    fh.print();
    cout << endl;
    //Childs
    fh.fib_insert(9,2);
    fh.fib_insert(7,2);
    fh.fib_insert(8,2);
    fh.fib_insert(17,7);
    fh.fib_insert(14,7);
    fh.fib_insert(13,4);
    fh.fib_insert(16,4);
    fh.fib_insert(15,5);
  */  
    cout << "All nodes : ";
    fh.print();
    cout << endl;
/*
    cout << "Decreasing key 13 to 10 : " << endl ;
    fh.decrease_key(13,10);
    fh.print();
    cout << endl;

    cout << "Decreasing key 14 to 11 : " << endl;
    fh.decrease_key(14,11);
    fh.print();
    cout << endl;
*/
    cout << "Deleting minimum : " << endl;
    fh.delete_min();
    fh.print();
    cout << endl;
    
    cout << "Deleting minimum : " << endl;
    fh.delete_min();
    fh.print();
    cout << endl;

    cout << "Deleting minimum : " << endl;
    fh.delete_min();
    fh.print();
    cout << endl;

    cout << "Deleting minimum : " << endl;
    fh.delete_min();
  //  fh.print();
    cout << endl;
/*for (int i=6;i<15; i++){
    int k=fh.find_deg(i);

    cout << i << " " << k << endl;
}*/

 //   fh.print();
 //   int dig=fh.find_deg(2);
 //   cout << dig << endl;
 //   fh.delete_min();
 //   fh.print();
 //   dig=fh.find_deg(3);
 //   cout << dig << endl;
 //   fh.delete_min();
 //   fh.print();
 //   dig=fh.find_deg(3);
 //   cout << dig << endl;
 //   fh.delete_min();
 //   fh.print();
//    fh.print();
 //   cout<<fh.find_min();
 //   cout<<"\n"<<fh.find_num();
   /*
    int old_val,new_val,c,val;
    int child,node;
    fh.make_heap();
    cout<<"\nFibonacci heap created";
    while(1)
    {
        cout<<"\n\nEnter 1 to print all elements"<<endl;
        cout<<"Enter 2 to get total number of nodes"<<endl;
        cout<<"Enter 3 to print minimum value"<<endl;
        cout<<"Enter 4 to decrease value of a node"<<endl;
        cout<<"Enter 5 to delete minimum value"<<endl;
        cout<<"Enter 6 to insert a node at root or child"<<endl;
        cout<<"Enter 7 to exit"<<endl;
        cin>>c;
        switch(c)
        {
            case 1:
                fh.print();
                break;
            case 2:
                cout<<"\n\nTotal number of nodes in heap = "<<fh.find_num();
                break;
            case 3:
                cout<<"\n\nMinimum value in heap = "<<fh.find_min();
                break;
            case 4:
                cout<<"\nEnter value of node to be decreased = ";
                cin>>old_val;
                cout<<"Enter new value of node = ";
                cin>>new_val;
                fh.decrease_key(old_val,new_val);
                break;
            case 5:
                fh.delete_min();
                cout<<"\nMinimum value deleted";
                break;
            case 6:
                cout<<"\nEnter where the value to be inserted : ";
                cout<<"\nEnter the node where child node to be inserted (Enter 0 if adding to the main roots ) : ";
                cin>>node;
                cout<<"\nEnter the value to be inserted : ";
                cin>>val;
                fh.fib_insert(val,node);
                cout<<"\nValue is inserted ";
                break;
            case 7:
                exit(0);
            default:
                cout<<"\nWrong value entered"<<endl;
                
        }
        
    }
    */
    return 0;   
}