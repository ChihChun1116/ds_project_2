#include <iostream>
#include <fstream>
using namespace std;

int length, width;
int B;
template <class T> class stack;
template <class T> class queue;
class Robot;

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
    friend class Robot;

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
    friend class Robot;

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
    friend class Robot;

    public:
        position(): row(0), col(0) {}
        position(int r, int c): row(r), col(c) {}
        ~position() {}
    
    private:
        int row, col;
};

class FloorNode {
    friend class Robot;

    public:
        FloorNode() {}
        FloorNode(char d): ch(d), DistanceToR(0) {}
        ~FloorNode() {}

    private:
        int DistanceToR;
        char ch;
        queue<position> AdjacentNode;
        position parent;
};

class Robot {
    public:
        Robot() {}
        ~Robot() {}
        void ReadFile(char* file);
        void FindAdjNode();
        void FindDisToR();
        void DFS(position p);
        void Cleaning();
        void WriteFile();

    private:
        position R;
        FloorNode** room;
        // Not done yet
};

void Robot::ReadFile(char* file)
{
    fstream in_file;
    char d;
    FloorNode temp;
    
    in_file.open(file, ios::in);
    if (!in_file) {
         cout << "Fail to open file " << endl;
        return;
    }
    in_file >> width >> length >> B;
    room = new FloorNode*[width];
    for (int i = 0; i < width; i++) {
        room[i] = new FloorNode[length];
    }
    for (int i = 0; i < width; i ++) {
        for (int j = 0; j < length; j++) {
            in_file >> d;
            temp = *new FloorNode(d);
            if (d == 'R') {
                R.row = i;
                R.col = j;
            }
            room[i][j] = temp;
        }
    }
    in_file.close();
    return;
}

void Robot::FindAdjNode()
{
    for (int i = 0; i < width; i++) {
        for (int j = 0; j < length; j++) {
            if (room[i][j].ch != '1') {
                if (i != 0 && i != width - 1 && j != 0 && j != length - 1) {
                    if (room[i - 1][j].ch != '1') {
                        position adjacent(i - 1, j);
                        room[i][j].AdjacentNode.push(adjacent);
                    }
                    if (room[i + 1][j].ch != '1') {
                        position adjacent(i + 1, j);
                        room[i][j].AdjacentNode.push(adjacent);
                    }
                    if (room[i][j - 1].ch != '1') {
                        position adjacent(i, j - 1);
                        room[i][j].AdjacentNode.push(adjacent);
                    }
                    if (room[i][j + 1].ch != '1') {
                        position adjacent(i, j + 1);
                        room[i][j].AdjacentNode.push(adjacent);
                    }
                } else {
                    if ((i == 0 && j == 0) || (i == width - 1 && j == 0) || (i == 0 && j == length - 1) || (i == width - 1 && j == length - 1)) {
                    } else if (i == 0) {
                        position adjacent(i + 1, j);
                        room[i][j].AdjacentNode.push(adjacent);
                    } else if (i == width - 1) {
                        position adjacent(i - 1, j);
                        room[i][j].AdjacentNode.push(adjacent);
                    } else if (j == 0) {
                        position adjacent(i, j + 1);
                        room[i][j].AdjacentNode.push(adjacent);
                    } else if (j == length - 1) {
                        position adjacent(i , j + 1);
                        room[i][j].AdjacentNode.push(adjacent);
                    }
                }
            }
        }
    }
    return;
}

void Robot::FindDisToR()
{

}

void Robot::DFS(position p)
{

}

void Robot::Cleaning()
{

}

void Robot::WriteFile()
{
    ofstream out_file("108061115.path");
    if(!out_file) {
        cout<< "Can't open out_file!" << endl;
        return;
    }
    //out_file << width << length << B << endl;
    for (int i = 0; i < width; i ++) {
        for (int j = 0; j < length; j++) {
            if (j == length -1) {
                out_file << "(" << room[i][j].AdjacentNode.Front().row << " " << room[i][j].AdjacentNode.Front().col << ")"<< endl;
            } else {
                out_file << "(" << room[i][j].AdjacentNode.Front().row << " " << room[i][j].AdjacentNode.Front().col << ")";
            }
        }
    }
    // need adjustment
    out_file.close();
    return;
}

int main(int argc, char *argv[])
{
    Robot robot;

    robot.ReadFile(argv[1]);
    robot.FindAdjNode();
    robot.WriteFile();
    return 0;
}
