#include <iostream>
#include <fstream>
using namespace std;

int length, width;
int B;
char** room;
template <class T> class stack;
template <class T> class queue;

template <class T>
class sq_node {
    friend class Robot;
    friend class queue<T>;
    friend class stack<T>;

    public:
        sq_node(T& a): value(a), next(NULL){}
        ~sq_node() {}

    private:
        T value;
        sq_node<T>* next;  
};

template <class T>
class queue {

    public:
        queue(): front(NULL), rear(NULL), q_size(0) {}
        ~queue() {}
        void push(T& item);
        void pop();
        T& Front();
        T& Rear();
        int size();
        bool IsEmpty();

    private:
        sq_node<T>* front;
        sq_node<T>* rear;
        int q_size;
};

template <class T>
void queue<T>::push(T& item)
{
    sq_node<T>* temp = new sq_node<T>(item);

    if(IsEmpty()) {
        front = temp;
        rear = temp;
    } else {
        rear->next = temp;
        rear = temp;
    }
    q_size++;
    return;
}

template <class T>
void queue<T>::pop()
{
    sq_node<T>* temp = front;

    if(IsEmpty()) {
        return;
    } else {
        front = front->next;
        delete temp;
        q_size--;
    }
    return;
}

template <class T>
T& queue<T>::Front() {return front->value;}

template <class T>
T& queue<T>::Rear() {return rear->value;}

template <class T>
int queue<T>::size() {return q_size;}

template <class T>
bool queue<T>::IsEmpty() 
{
    if (front == NULL) {
        return true;
    } else {
        return false;
    }
}

template <class T>
class stack {

    public:
        stack(): top(NULL), s_size(0) {}
        ~stack() {}
        void push(T& item);
        void pop();
        T& Top();
        int size();
        bool IsEmpty();

    private:
        sq_node<T>* top;
        int s_size;
};

template <class T>
void stack<T>::push(T &item)
{
    sq_node<T>* temp = new sq_node<T>(item);

    if (IsEmpty()) {
        top = temp;
    } else {
        temp->next = top;
        top = temp;
    }
    s_size++;
    return;
}

template <class T>
void stack<T>::pop()
{
    sq_node<T>* temp = top;

    if (IsEmpty()) {
        return;
    } else {
        top = top->next;
        delete temp;
        s_size--;
    }
    return;
}

template <class T>
T& stack<T>::Top() {return top->value;}

template <class T>
int stack<T>::size() {return s_size;}

template <class T>
bool stack<T>::IsEmpty() 
{
    if (top == NULL) {
        return true;
    } else {
        return false;
    }
}

class position {

    public:
        position(): row(0), col(0) {}
        position(int r, int c): row(r), col(c) {}
        ~position() {}
    
    private:
        int row, col;
};

class FloorNode {
    public:
        FloorNode() {}
        FloorNode(int r, int c, char d): row(r), col(c), ch(d), DistanceToR(0) {}
        ~FloorNode() {}

    private:
        int DistanceToR;
        int row, col;
        char ch;
        queue<position> AdjacentNode;
        position parent;
};

int main(int argc, char *argv[])
{
    fstream in_file;
    ofstream out_file("108061115.path");
    char d;
    
    in_file.open(argv[1], ios::in);
    if (!in_file) {
         cout << "Fail to open file " << endl;
        return 0;
    }
    in_file >> width >> length >> B;
    room = new char* [width];
    for (int i = 0; i < width; i++) {
        room[i] = new char[length];
    }
    for (int i = 0; i < width; i ++) {
        for (int j = 0; j < length; j++) {
            in_file >> d;
            room[i][j] = d;
        }
    }
    in_file.close();

    // Thoughts: BFS to find the step from the battery to every FloorNode (DistanceToR in FloorNode). 
    // While cleaning, use BFS (keep eyes on the battery level and DistanceToR)

    /*if(!out_file) {
        cout<< "Can't open out_file!" << endl;
        return 0;
    }
    out_file << width << " " << length << " " << B << endl;
    for (int i = 0; i < width; i ++) {
        for (int j = 0; j < length; j++) {
            out_file << room[i][j];
        }
    }
    out_file.close();*/
    return 0;
}
