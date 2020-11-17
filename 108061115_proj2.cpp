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
void stack<T>::push(T& item)
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
        bool** cleaned;
        stack<position> dfspoint;
        queue<position> route;
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
                        position adjacent(i , j - 1);
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
    int D = 0;
    sq_node<position>* current;
    queue<position> Q;

    cleaned = new bool* [width];
    for (int i = 0; i < width; i++) {
        cleaned[i] = new bool[length];
    }
    for (int i = 0; i < width; i++) {
        for (int j = 0; j < length; j++) {
            cleaned[i][j] = false;
        }
    }
    cleaned[R.row][R.col] = true;
    room[R.row][R.col].DistanceToR = D;
    Q.push(R);
    while (!Q.IsEmpty()) {
        position node = Q.Front();
        Q.pop();
        for (current = room[node.row][node.col].AdjacentNode.front; current != NULL; current = current->next) {
            if (cleaned[current->value.row][current->value.col] == false) {
                cleaned[current->value.row][current->value.col] = true;
                room[current->value.row][current->value.col].DistanceToR = room[node.row][node.col].DistanceToR + 1;
                room[current->value.row][current->value.col].parent = node;
                Q.push(current->value);
            }
        }
    }
    for (int i = 0; i < width; i++) {
        delete [] cleaned[i];
    }
    delete [] cleaned;
    return;
}


void Robot::DFS(position p)
{
    int b_level = B;
    int x;
    int path;
    stack<position> visit;
    stack<position> through;
    position temp(p.row, p.col);

    cleaned[temp.row][temp.col] = true;
    while (!(temp.row == R.row && temp.col == R.col)) {
        visit.push(temp);
        temp = room[temp.row][temp.col].parent;
    }
    b_level = b_level - visit.size();
    while(!visit.IsEmpty()) {
        through.push(temp);
        temp = visit.Top();
        for (sq_node<position>* n = room[temp.row][temp.col].AdjacentNode.front; n != NULL; n = n->next) {
            if (cleaned[n->value.row][n->value.col] == false) {
                dfspoint.push(n->value);
            }
        }
        cleaned[temp.row][temp.col] = true;
        route.push(temp);
        visit.pop();
    }
    x = room[temp.row][temp.col].DistanceToR;
    while (b_level > x) {
        path = 0;
        for (sq_node<position>* n = room[temp.row][temp.col].AdjacentNode.front; n != NULL; n = n->next) {
            if (cleaned[n->value.row][n->value.col] == false) {
                dfspoint.push(n->value);
                path++;
            }
        }
        if (path != 0) {
            through.push(temp);
            temp = dfspoint.Top();
            if (room[temp.row][temp.col].DistanceToR < b_level) {
                cleaned[temp.row][temp.col] = true;
                b_level--;
                route.push(temp);
                dfspoint.pop();
            } else {
                temp = through.Top();
                break;
            }
        } else if (path == 0) {
            if (!through.IsEmpty()) {
                if (room[through.Top().row][through.Top().col].DistanceToR < b_level) {
                    b_level--;
                    temp = through.Top();
                    route.push(temp);
                    through.pop();
                } else {break;}
            } else {break;}
        }
        x = room[temp.row][temp.col].DistanceToR;
        if (b_level == x) {
            for (sq_node<position>* n = room[temp.row][temp.col].AdjacentNode.front; n != NULL; n = n->next) {
                if (cleaned[n->value.row][n->value.col] == false) {
                    visit.push(n->value);
                    path++;
                }
            }
            break;
        }
    }
    while (!(temp.row == R.row && temp.col == R.col)) {
        temp = room[temp.row][temp.col].parent;
        for (sq_node<position>* n = room[temp.row][temp.col].AdjacentNode.front; n != NULL; n = n->next) {
            if (cleaned[n->value.row][n->value.col] == false) {
                visit.push(n->value);
            }
        }
        cleaned[temp.row][temp.col] = true;
        route.push(temp);
    }
    while (!through.IsEmpty()) {
        through.pop();
    }
    return;
}

void Robot::Cleaning()
{
    cleaned = new bool* [width];
    for (int i = 0; i < width; i++) {
        cleaned[i] = new bool[length];
    }
    for (int i = 0; i < width; i++) {
        for (int j = 0; j < length; j++) {
            cleaned[i][j] = false;
        }
    }
    dfspoint.push(R);
    while (!dfspoint.IsEmpty()) {
        position node = dfspoint.Top();
        dfspoint.pop();
        if (cleaned[node.row][node.col] == false) {
            DFS(node);
        }
    }
    for (int i = 0; i < width; i++) {
        delete [] cleaned[i];
    }
    delete [] cleaned;
    return;
}

void Robot::WriteFile()
{
    ofstream out_file("108061115.path");
    if (!out_file) {
        cout<< "Can't open out_file!" << endl;
        return;
    }
    out_file << route.size() << endl;
    out_file << R.row << " " << R.col << endl;
    while (!route.IsEmpty()) {
        out_file << route.Front().row << " " << route.Front().col << endl;
        route.pop();
    }
    out_file.close();
    return;
}

int main(int argc, char *argv[]) // success
{
    Robot robot;

    robot.ReadFile(argv[1]);
    robot.FindAdjNode();
    robot.FindDisToR();
    robot.Cleaning();
    robot.WriteFile();
    return 0;
}