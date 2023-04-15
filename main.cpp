#include <iostream>

template <typename IDType, typename dataType>
class Node;


template <typename IDType, typename dataType>
class Graph;


template <typename IDType, typename dataType>
class Edge {
    friend class Node<IDType, dataType>;
private:
    unsigned short cost;
    Node<IDType, dataType> *ToNode;
    Edge *NextEdgeOfTheNode;
public:
    Edge() {
        cost = 0;
        ToNode = nullptr;
        NextEdgeOfTheNode = nullptr;
    }

    Edge(unsigned short Cost, Node<IDType, dataType> &node) {
        cost = Cost;
        ToNode = &node;
        NextEdgeOfTheNode = nullptr;
    }

    ~Edge() {
        ToNode = nullptr;
        NextEdgeOfTheNode = nullptr;
    }

    void Print() {
        std::cout << "(" << cost << ",  " << ToNode->GetID() << ")";
    }

    void SetNextEdge(Edge *edge) { NextEdgeOfTheNode = edge; }
};


template <typename IDType, typename dataType>
class Node {
    friend class Graph<IDType, dataType>;
private:
    IDType ID;
    dataType data;
    Edge<IDType, dataType> *edgeToNode;
public:
    ~Node() {
        if (edgeToNode != nullptr) {
            Edge<IDType, dataType> *edgePointer;
            while (edgeToNode->NextEdgeOfTheNode != nullptr) {
                edgePointer = edgeToNode;
                while (edgePointer->NextEdgeOfTheNode != nullptr) {
                    edgePointer = edgePointer->NextEdgeOfTheNode;
                }
                delete edgePointer;
            }
            delete edgeToNode;
        }
    }

    Node() = default;

    Node(IDType id, dataType Data) {
        ID = id;
        data = Data;
        edgeToNode = nullptr;
    }

    void SetConnectionTo(Node &N, unsigned short Cost) {
        if (edgeToNode == nullptr) {
            edgeToNode = new Edge<IDType, dataType>(Cost, N);
            return;
        }
        Edge<IDType, dataType> *edgePointer = edgeToNode;
        while (edgePointer->NextEdgeOfTheNode != nullptr) {
            if (edgePointer->ToNode->GetID() == N.GetID()) {
                std::cout << "Node with the same ID had been already added" << std::endl;
                return;
            }
            edgePointer = edgePointer->NextEdgeOfTheNode;
        }
        if (edgePointer->ToNode->GetID() == N.GetID()) {
            std::cout << "Node with the same ID had been already added" << std::endl;
            return;
        }
        edgePointer->NextEdgeOfTheNode = new Edge<IDType, dataType>(Cost, N);
    }

    void RemoveConnectionTo(IDType Id) {
        Edge<IDType, dataType> *edgePointer = edgeToNode;
        if (edgePointer->ToNode->ID == Id) {                    //Если удаляемый элемент оказался первым в списке
            edgeToNode = edgePointer->NextEdgeOfTheNode;
            delete(edgePointer);
            return;
        }
        while (edgePointer->NextEdgeOfTheNode != nullptr) {
            if (edgePointer->NextEdgeOfTheNode->ToNode->ID == Id) {
                Edge<IDType, dataType>* edgeToDelete = edgePointer->NextEdgeOfTheNode;
                edgePointer->SetNextEdge(edgeToDelete->NextEdgeOfTheNode);
                delete(edgeToDelete);
                return;
            }
            edgePointer = edgePointer->NextEdgeOfTheNode;
        }
        std::cout << "Second node hasn't been found!" << std::endl;
    }

    void Print() {
        Edge<IDType, dataType>* edgePointer = edgeToNode;
        while (edgePointer != nullptr) {
            std::cout.width(6);
            std::cout << ID << "|";
            std::cout.width(5);
            std::cout << edgePointer->cost << "|";
            std::cout.width(8);
            std::cout << edgePointer->ToNode->ID << "|\n";
            edgePointer = edgePointer->NextEdgeOfTheNode;
        }
        printf("\n");
    }

    IDType GetID() { return ID; }
};


template <typename IDType, typename dataType>
class Graph {
private:
    unsigned int nodesCount;
    Node<IDType, dataType> *Nodes;

    Node<IDType, dataType> *ExtendArray() {
        nodesCount++;
        auto *arr2 = new Node<IDType, dataType>[nodesCount];
        if (nodesCount == 1) {
            return arr2;
        }
        for (int i = 0; i < nodesCount-1; i++)
            arr2[i] = Nodes[i];
        delete[] Nodes;
        return arr2;
    }

    Node<IDType, dataType> *ReduceArray(IDType idOfDeleting) {
        nodesCount--;
        if (nodesCount == 0) {
            delete[] Nodes;
            return nullptr;
        }
        auto *arr2 = new Node<IDType, dataType>[nodesCount];
        int i = 0, j = 0;
        while (i < nodesCount) {
            if (j != idOfDeleting) {
                arr2[i] = Nodes[j];
                i++;
                j++;
            }
            else j++;
        }
        delete[] Nodes;
        return arr2;
    }

public:
    Graph() {
        Nodes = nullptr;
        nodesCount = 0;
    }

    void AddWay(IDType IdFrom, unsigned short Cost, dataType IdTo) {
        if (IdFrom == IdTo) {
            std::cout << "You entered two identical IDs!!!" << std::endl;
            return;
        }
        int i = 0, node1Index = -1;
        Node<IDType, dataType> *node2 = nullptr;
        while (i < nodesCount && (node1Index == -1 || node2 == nullptr)) {
            if (Nodes[i].GetID() == IdFrom) {node1Index = i;}
            if (Nodes[i].GetID() == IdTo) {node2 = &Nodes[i];}
            i++;
        }
        if (node1Index == -1 || node2 == nullptr) { std::cout << "At least one of the nodes hasn't been found!" << std::endl; }
        else { Nodes[node1Index].SetConnectionTo(*node2, Cost); }
    }

    void DeleteWay(IDType IdFrom, dataType IdTo) {
        int i = 0;
        while (i < nodesCount) {
            if (Nodes[i].GetID() == IdFrom) {
                Nodes[i].RemoveConnectionTo(IdTo);
                break;
            }
            i++;
        }
        if (i >= nodesCount) { std::cout << "First node hasn't been found!" << std::endl; }
    }

    void CreateNode(IDType id, dataType Data) {
        bool isThereTheSameID = false;
        for (int i = 0; i < nodesCount; i++) {
            if (Nodes[i].GetID() == id){
                isThereTheSameID = true;
                break;
            }
        }
        if (!isThereTheSameID) {
            Nodes = ExtendArray();
            Nodes[nodesCount-1] = Node<IDType, dataType>(id, Data);
        }
    }

    void DeleteNode(IDType id) {
    Node<IDType, dataType> *deletingNode;
    int i = 0;
    while (i < nodesCount) {
        if (Nodes[i].GetID() == id) {
            deletingNode = &Nodes[i];
            break;
        }
        i++;
    }
    if (i < nodesCount) {
        ReduceArray(id);
        delete deletingNode;
    }
    else {std::cout << "Node with such ID hasn't been found!" << std::endl;}
}

    void Print() {
        std::cout << "       ->Graph<-\n";
        std::cout << "    ID|         Data|\n";
        for (int i = 0; i < nodesCount; i++) {
            std::cout.width(6);
            std::cout << Nodes[i].GetID() << "|";
            std::cout.width(13);
            std::cout << Nodes[i].data << "|\n";
        }
        std::cout << "\n";

        std::cout << "       ->Ways<-\n";
        std::cout << "  From| Cost|      To|\n";
        for (int i = 0; i < nodesCount; i++) {
            Nodes[i].Print();
        }
    }
};

int main() {
    Graph<int, int> graph;
    graph.CreateNode(111, 35);
    graph.CreateNode(112, 78);
    graph.CreateNode(113, 34);
    graph.AddWay(111, 5, 112);
    graph.AddWay(111, 9, 113);
    graph.Print();
    graph.DeleteWay(111, 113);
    graph.Print();
    return 0;
}
