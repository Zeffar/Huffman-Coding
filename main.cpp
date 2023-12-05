#include <iostream>
#include <fstream>
#include <queue>
#include <string>
#include <unordered_map>
using namespace std;
ifstream f("data.in");
ifstream fin("alphabet.txt");
ofstream g("alphabet.txt"); //self explanatory 
ofstream h("tree.txt"); //huffman tree
const int SIZE = 100; //nb of distinct instructions

class Node {
public:
    string data;
    int freq;
    Node* left;
    Node* right;
    Node(string ID, int freq)
    {
        data = ID;
        freq = freq;
        left = right = NULL;
    }
};

class Compare {
public:
    bool operator()(Node* a, Node* b)
    {return (a->freq) > (b->freq);}
};

Node* generateTree(priority_queue<Node*, vector<Node*>, Compare> pq)
{
    int currentSize=SIZE;
    while (pq.size() != 1) //loop stops when only the root is left in queue
    {//generate parent from 2 nodes, build the tree bottom-up
        Node* left = pq.top(); pq.pop();
        Node* right = pq.top();pq.pop();

        Node* node = new Node(to_string(++currentSize), left->freq + right->freq);
        node->left = left;
        node->right = right;

        pq.push(node);
    }
    return pq.top();
}

void printCodes(Node* root, int arr[], int top=0)
{//arr[] - encoding of the node
 //top - current node if it has unexplored children
 //0 - left move : 1 - right move
    if(root->left || root->right) h<<'0';
    else h<<'1';
    if (root->left)
    {
        arr[top] = 0;
        printCodes(root->left, arr, top + 1);
    }

    if (root->right)
    {
        arr[top] = 1;
        printCodes(root->right, arr, top + 1);
    }

    //if leaf or already explored children, print the encoding
    if (!root->left && !root->right)
    {
        g << root->data << " ";
        for (int i = 0; i < top; ++i)
            g << arr[i];
        g << endl;
    }
}

void huffman(unordered_map<string, int> v)
{
    priority_queue<Node*, vector<Node*>, Compare> pq;

    for (auto i : v)
    {
        Node* newNode = new Node(i.first, i.second);
        pq.push(newNode);
    }
    Node* root = generateTree(pq);
    int arr[SIZE];
    printCodes(root, arr);
}

void read(unordered_map<string, int>& umap)
{
    string s;
    while(true)
    {
        getline(f, s);
        if (umap.find(s)!=umap.end())
            ++umap[s];
        else
            umap[s]=1;
        if(f.eof()) break;
    }
}
void merge_files()
{
    h<<'\n';
    string s;
    while(true)
    {
        getline(fin, s);
        if(fin.eof()) break;
        h<<s<<'\n';
    }
}
int main()
{
    unordered_map<string, int> freq;
    read(freq);
    huffman(freq);
    merge_files();
    return 0;
}
