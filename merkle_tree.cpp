/*
Implementation of the merkle tree and node classes and helper function to read from an inputed file.
*/
#include "merkle_tree.h"

using namespace std;

/* Hash function*/
inline string fnv1a(string const & text) {
    // 64 bit params
    uint64_t constexpr fnv_prime = 1099511628211ULL;
    uint64_t constexpr fnv_offset_basis = 14695981039346656037ULL;

    uint64_t hash = fnv_offset_basis;

    for(auto c: text) {
        hash ^= c;
        hash *= fnv_prime;
    }

    return to_string(hash);
}

//Node Constructor initializes a node to the value passed
Node::Node(const string& val){
    Node* parent = nullptr;
    vector<Node*>* children = nullptr;
    key = val; 
}

/* Merkle Tree method for computing the hash of the concatenated hashes of children nodes
   Using the provided hash function, concatenates child hashes from left to right and returns the hash
   of the concatenation */

//Function list in order given the header file
//Default constructor for merkleTree
merkleTree::merkleTree(){
    root = nullptr;
}

//Custom Constructor
merkleTree::merkleTree(const vector<int>& data){
    root = nullptr;
    this -> data = data;
    constructMerkleTree();
}

string merkleTree::concatenateHash(const vector<Node*>& nodes){
    string merged;
    //concatenate hashes of all inputed nodes
    for(const Node* node: nodes){
        merged += node->key;
    }
    //take hash of concatenated nodes
    string mergedHash = fnv1a(merged);
    return mergedHash;
}

void merkleTree::printTree(const Node* node, int depth){
    if (!node){
        return;
    }

    queue<const Node*> nodes;
    nodes.push(root);

    int currLevel,nextLevel;
    currLevel = 1;
    nextLevel = 0;


    while (!nodes.empty()) {
        const Node* currentNode = nodes.front();
        nodes.pop();
        if (currLevel == 0) {
            depth++;
            currLevel = nextLevel;
            nextLevel = 0;
        }
        for (int i = 0; i < depth; i++){
            cout << " "; 
        }
        cout << "Level " << depth << ": " << currentNode->key << endl;
        currLevel--;
        for (auto child : currentNode->children) {
            nodes.push(child);
            nextLevel++;
        }
    }
}


void merkleTree::verify(){
    Node* firstRoot = root;
    constructMerkleTree();
    if (firstRoot -> key == root -> key){
        cout << "True" << endl;
    }else{
        cout << "False" << endl;
    }
}

void merkleTree::overwrite(int originalValue, int newValue){
    bool replace = false;
    for (int i = 0; i < data.size(); i++){
        if (data[i] == originalValue){
            replace = true;
            data[i] = newValue;
            break;
        }
    }
    if(replace == true){
        emptyMerkleTree();
        constructMerkleTree();
    }
}

void merkleTree::insert(int newValue){
    data.push_back(newValue);
    emptyMerkleTree();
    constructMerkleTree();
}

void merkleTree::printRoot(){ 
    cout << this -> getRoot() -> key << endl; 
}

Node* merkleTree::getRoot(){
    return root;
}

void merkleTree::emptyMerkleTree(){
    emptyMerkleTree(root);
    root = nullptr;
}

void merkleTree::emptyMerkleTree(Node* node){
    if (node == nullptr) {
        return;
    } else {
    for (int i = 0; i < node->children.size(); i++) {
        Node* child = node->children[i];
        emptyMerkleTree(child);
    }
        delete node; 
    }
}

void merkleTree::constructMerkleTree() {
    vector<Node* > leafNodes; 
   for (int i = 0; i < data.size(); i++) {
    int temp = data[i];
    leafNodes.push_back(new Node(fnv1a(to_string(temp))));
    }
    while (leafNodes.size() > 1) {
        vector<Node*> parents; 
        for (int i = 0; i < leafNodes.size(); i += 4) { 
            vector<Node*> children;
            for (int j = i; j < i + 4 && j < leafNodes.size(); ++j) { 
                children.push_back(leafNodes[j]);
            }
            Node* parentNode = new Node(concatenateHash(children));
            parentNode -> children = children;
            parents.push_back(parentNode);
        }
        leafNodes.swap(parents);
    }
    if (leafNodes.empty()) {
    } else {
        root = leafNodes[0];
    }
}

vector<int> readIntsFromFile(const string& filename){
    fstream input; 
    input.open(filename, ios::in);

    vector<int> data_vector;
    if (input.is_open()){
        int data;
        while (input >> data) {
            data_vector.push_back(data);
        }
    }
    input.close();
    return data_vector;
}

