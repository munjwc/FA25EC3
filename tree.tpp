#pragma once
#include <queue>
#include <algorithm>

// --------------------------
// Node constructor
// --------------------------
template <typename U>
Node<U>::Node(const string &nodeID, const U &value) {
    // just store id and data
    id = nodeID;
    data = value;
}


// --------------------------
// Tree constructor
// --------------------------
// root starts as nullptr
template <typename T>
Tree<T>::Tree() {
    root = nullptr;
}


// --------------------------
// findNode
// --------------------------
// Simple BFS search for a node with a matching ID.
template <typename T>
Node<T>* Tree<T>::findNode(const string &id) {
    if (!root) return nullptr;

    queue<Node<T>*> q;
    q.push(root);

    while (!q.empty()) {
        Node<T>* cur = q.front();
        q.pop();

        if (cur->id == id) {
            return cur;
        }

        for (auto child : cur->children) {
            q.push(child);
        }
    }

    return nullptr;
}


// --------------------------
// createRoot
// --------------------------
// Creates the very first node of the tree.
// Only works once.
template <typename T>
void Tree<T>::createRoot(const string &id, const T &value) {
    if (root != nullptr) {
        // root already set
        return;
    }
    root = new Node<T>(id, value);
}


// --------------------------
// addNode
// --------------------------
// Adds a child under a given parent.
// If the child already exists somewhere in the tree,
// we reuse it so that it can have multiple parents.
template <typename T>
void Tree<T>::addNode(const string &parentID,
                      const string &childID,
                      const T &value) {

    Node<T>* parent = findNode(parentID);
    if (!parent) {
        // parent must exist to attach child
        return;
    }

    Node<T>* child = findNode(childID);

    // If the child does not exist yet, create it
    if (!child) {
        child = new Node<T>(childID, value);
    }

    // Link parent -> child (this allows multiple parents)
    parent->children.push_back(child);
}


// --------------------------
// printAll
// --------------------------
// Prints each node and its children in a readable way.
// Not perfect formatting, but clear enough.
template <typename T>
void Tree<T>::printAll() {
    if (!root) {
        cout << "Tree is empty." << endl;
        return;
    }

    queue<Node<T>*> q;
    q.push(root);

    cout << "\n===== Story Tree =====\n";

    while (!q.empty()) {
        Node<T>* cur = q.front();
        q.pop();

        cout << "Node " << cur->id << ": " << cur->data << "\n";

        if (cur->children.empty()) {
            cout << "  Child -> (none)\n\n";
        } else {
            for (auto c : cur->children) {
                cout << "  Child -> " << c->id << "\n";
                q.push(c);
            }
            cout << "\n";
        }
    }

    cout << "======================\n";
}


// --------------------------
// Tree destructor
// --------------------------
// We collect all unique nodes (because children
// can be shared by many parents) and delete them once.
template <typename T>
Tree<T>::~Tree() {
    if (!root) return;

    vector<Node<T>*> visited;
    queue<Node<T>*> q;
    q.push(root);

    while (!q.empty()) {
        Node<T>* cur = q.front();
        q.pop();

        // check if we already saw this node
        bool already = false;
        for (auto n : visited) {
            if (n == cur) {
                already = true;
                break;
            }
        }
        if (already) continue;

        visited.push_back(cur);

        for (auto c : cur->children) {
            q.push(c);
        }
    }

    // delete all unique nodes we collected
    for (auto n : visited) {
        delete n;
    }
}