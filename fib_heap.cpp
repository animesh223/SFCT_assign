#include <iostream>
#include <fstream>
#include <string>

using namespace std;

//Node of Fibonacci heap
class hNode{
public:
    hNode * parent;   //pointer to the parent
    hNode * child;    //pointer to the child
    hNode * left;     //pointer to the left node
    hNode * right;    //pointer to the right node
    int degree;      //Degree of the node
    int key;         //Value of the node
    char mark;       //mark of the node
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
            temp->next = rear;
            rear = temp;
        }
        hNode* dequeue(){
            hNode *temp;
            qNode *tempq;
            if(front == NULL)
                return NULL;
            temp = front->data;
            if(rear == front){
                delete(front);
                front = rear = NULL;
                return temp;
            }
            tempq = rear;
            while(tempq->next != front){
                tempq = tempq->next;
            }
            delete(front);
            front = tempq;
            return temp;
        }
        void display(){
            qNode *temp = rear;
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
    void cut(); //to cut (kashish(1))
    void find_node(); //to find a node with particular key value (Kashish(2))
    bool decrease_key(int old_value,int new_value); //to decrease a particular key (Kashish(3))
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

/*
void FHeap:: cut(){

}
*/

/*
void FHeap:: find_node(){

}
*/

/*
bool FHeap:: decrease_key(int old_value,int new_value){

}
*/

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