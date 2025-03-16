#include <iostream>
#include <cmath>

using namespace std;

const int MAX_SIZE = 10;

struct Node {
    int x, y, cost, heuristic;
    Node* parent;
    
    Node(int x, int y, int cost, int heuristic, Node* parent = nullptr)
        : x(x), y(y), cost(cost), heuristic(heuristic), parent(parent) {}
    
    int totalCost() const { return cost + heuristic; }
};

class AStar {
public:
    AStar(int width, int height) : width(width), height(height) {
        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                grid[i][j] = 0;
            }
        }
    }
    
    void setObstacle(int x, int y) { grid[y][x] = 1; }
    
    Node* openList[MAX_SIZE * MAX_SIZE];
    int openListSize = 0;
    Node* allNodes[MAX_SIZE * MAX_SIZE];
    int allNodesSize = 0;
    
    void pushOpenList(Node* node) {
        int i = openListSize++;
        while (i > 0 && openList[(i - 1) / 2]->totalCost() > node->totalCost()) {
            openList[i] = openList[(i - 1) / 2];
            i = (i - 1) / 2;
        }
        openList[i] = node;
    }
    
    Node* popOpenList() {
        if (openListSize == 0) return nullptr;
        Node* minNode = openList[0];
        openList[0] = openList[--openListSize];
        int i = 0;
        while (true) {
            int left = 2 * i + 1, right = 2 * i + 2, smallest = i;
            if (left < openListSize && openList[left]->totalCost() < openList[smallest]->totalCost())
                smallest = left;
            if (right < openListSize && openList[right]->totalCost() < openList[smallest]->totalCost())
                smallest = right;
            if (smallest == i) break;
            swap(openList[i], openList[smallest]);
            i = smallest;
        }
        return minNode;
    }
    
    void addNodeToAll(Node* node) {
        allNodes[allNodesSize++] = node;
    }
    
    bool nodeExists(int x, int y, int& index) {
        for (int i = 0; i < allNodesSize; i++) {
            if (allNodes[i]->x == x && allNodes[i]->y == y) {
                index = i;
                return true;
            }
        }
        return false;
    }
    
    Node* findPath(int startX, int startY, int goalX, int goalY) {
        Node* startNode = new Node(startX, startY, 0, heuristic(startX, startY, goalX, goalY));
        pushOpenList(startNode);
        addNodeToAll(startNode);
        
        int directions[4][2] = {{0,1}, {1,0}, {0,-1}, {-1,0}};
        
        while (openListSize > 0) {
            Node* current = popOpenList();
            
            if (current->x == goalX && current->y == goalY)
                return current;
            
            for (int i = 0; i < 4; i++) {
                int nx = current->x + directions[i][0];
                int ny = current->y + directions[i][1];
                
                if (isValid(nx, ny)) {
                    int newCost = current->cost + 1;
                    int index;
                    
                    if (!nodeExists(nx, ny, index) || newCost < allNodes[index]->cost) {
                        Node* neighbor = new Node(nx, ny, newCost, heuristic(nx, ny, goalX, goalY), current);
                        pushOpenList(neighbor);
                        addNodeToAll(neighbor);
                    }
                }
            }
        }
        return nullptr;
    }
    
private:
    int width, height;
    int grid[MAX_SIZE][MAX_SIZE];
    
    int heuristic(int x1, int y1, int x2, int y2) {
        return abs(x1 - x2) + abs(y1 - y2);
    }
    
    bool isValid(int x, int y) {
        return x >= 0 && y >= 0 && x < width && y < height && grid[y][x] == 0;
    }
};

void printPath(Node* node) {
    if (!node) {
        cout << "No path found!\n";
        return;
    }
    int path[MAX_SIZE * MAX_SIZE][2];
    int count = 0;
    while (node) {
        path[count][0] = node->x;
        path[count][1] = node->y;
        node = node->parent;
        count++;
    }
    
    cout << "Path found:\n";
    for (int i = count - 1; i >= 0; i--) {
        cout << "(" << path[i][0] << ", " << path[i][1] << ") -> ";
    }
    cout << "Goal\n";
}

int main() {
    AStar pathfinder(10, 10);
    pathfinder.setObstacle(4, 4);
    pathfinder.setObstacle(4, 5);
    pathfinder.setObstacle(4, 6);
    
    Node* path = pathfinder.findPath(0, 0, 9, 9);
    printPath(path);
    
    return 0;
}
