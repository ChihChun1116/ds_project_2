#include <iostream>
#include <fstream>
#include <stack>
#include <queue>
using namespace std;

int length, width;
int B;
class Robot;
template<class T> class LinkList;

template <class T>
class sq_node {
    friend class Robot;
    friend class LinkList<T>;

    public:
        sq_node(T& a): value(a), next(NULL){}
        ~sq_node() {}

    private:
        T value;
        sq_node<T>* next;  
};

template <class T>
class LinkList {
    friend class Robot;

    public:
        LinkList():head(NULL), tail(NULL) {}
        ~LinkList() {}
        void add(T& item);
        T& Head();
        int size();
        bool IsEmpty();
        
    private:
        sq_node<T>* head;
        sq_node<T>* tail;
};

template <class T>
void LinkList<T>::add(T& item)
{
    sq_node<T>* temp = new sq_node<T>(item);

    if(IsEmpty()) {
        head = temp;
        tail = temp;
    } else {
        tail->next = temp;
        tail = temp;
    }
    return;
}

template <class T>
T& LinkList<T>::Head() {return head->value;}

template <class T>
bool LinkList<T>::IsEmpty()
{
    if (head == NULL) {
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
        LinkList<position> AdjacentNode;
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
                        room[i][j].AdjacentNode.add(adjacent);
                    }
                    if (room[i + 1][j].ch != '1') {
                        position adjacent(i + 1, j);
                        room[i][j].AdjacentNode.add(adjacent);
                    }
                    if (room[i][j - 1].ch != '1') {
                        position adjacent(i, j - 1);
                        room[i][j].AdjacentNode.add(adjacent);
                    }
                    if (room[i][j + 1].ch != '1') {
                        position adjacent(i, j + 1);
                        room[i][j].AdjacentNode.add(adjacent);
                    }
                } else {
                    if ((i == 0 && j == 0) || (i == width - 1 && j == 0) || (i == 0 && j == length - 1) || (i == width - 1 && j == length - 1)) {
                    } else if (i == 0) {
                        position adjacent(i + 1, j);
                        room[i][j].AdjacentNode.add(adjacent);
                    } else if (i == width - 1) {
                        position adjacent(i - 1, j);
                        room[i][j].AdjacentNode.add(adjacent);
                    } else if (j == 0) {
                        position adjacent(i, j + 1);
                        room[i][j].AdjacentNode.add(adjacent);
                    } else if (j == length - 1) {
                        position adjacent(i , j - 1);
                        room[i][j].AdjacentNode.add(adjacent);
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
    while (!Q.empty()) {
        position node = Q.front();
        Q.pop();
        for (current = room[node.row][node.col].AdjacentNode.head; current != NULL; current = current->next) {
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
    while(!visit.empty()) {
        through.push(temp);
        temp = visit.top();
        for (sq_node<position>* n = room[temp.row][temp.col].AdjacentNode.head; n != NULL; n = n->next) {
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
        for (sq_node<position>* n = room[temp.row][temp.col].AdjacentNode.head; n != NULL; n = n->next) {
            if (cleaned[n->value.row][n->value.col] == false) {
                dfspoint.push(n->value);
                path++;
            }
        }
        if (path != 0) {
            through.push(temp);
            temp = dfspoint.top();
            if (room[temp.row][temp.col].DistanceToR < b_level) {
                cleaned[temp.row][temp.col] = true;
                b_level--;
                route.push(temp);
                dfspoint.pop();
            } else {
                temp = through.top();
                break;
            }
        } else if (path == 0) {
            if (!through.empty()) {
                if (room[through.top().row][through.top().col].DistanceToR < b_level) {
                    b_level--;
                    temp = through.top();
                    route.push(temp);
                    through.pop();
                } else {break;}
            } else {break;}
        }
        x = room[temp.row][temp.col].DistanceToR;
        if (b_level == x) {
            for (sq_node<position>* n = room[temp.row][temp.col].AdjacentNode.head; n != NULL; n = n->next) {
                if (cleaned[n->value.row][n->value.col] == false) {
                    dfspoint.push(n->value);
                    path++;
                }
            }
            break;
        }
    }

    while (!(temp.row == R.row && temp.col == R.col)) {
        temp = room[temp.row][temp.col].parent;
        for (sq_node<position>* n = room[temp.row][temp.col].AdjacentNode.head; n != NULL; n = n->next) {
            if (cleaned[n->value.row][n->value.col] == false) {
                dfspoint.push(n->value);
            }
        }
        cleaned[temp.row][temp.col] = true;
        route.push(temp);
    }
    while (!through.empty()) {
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
    while (!dfspoint.empty()) {
        position node = dfspoint.top();
        dfspoint.pop();
        if (cleaned[node.row][node.col] == false) {
            DFS(node);
        }
    }
    for (int i = 0; i < width; i++) {
       for (int j = 0; j < length; j++) {
           if (room[i][j].ch == '0' && cleaned[i][j] == false) {
               cout << i << " " << j << " is not clean" << endl;
               cout << "Distance to R:" << room[i][j].DistanceToR << endl;
           }
       }
    }
    cout << "done" << endl;
    for (int i = 0; i < width; i++) {
        delete [] cleaned[i];
    }
    delete [] cleaned;
    return;
}

void Robot::WriteFile()
{
    ofstream out_file("108061115.path");        // For different output file, change the name here
    if (!out_file) {
        cout<< "Can't open out_file!" << endl;
        return;
    }
    out_file << route.size() << endl;
    out_file << R.row << " " << R.col << endl;
    while (!route.empty()) {
        out_file << route.front().row << " " << route.front().col << endl;
        route.pop();
    }
    out_file.close();
    return;
}

int main(int argc, char *argv[])
{
    Robot robot;

    robot.ReadFile(argv[1]);
    robot.FindAdjNode();
    robot.FindDisToR();
    robot.Cleaning();
    robot.WriteFile();
    return 0;
}