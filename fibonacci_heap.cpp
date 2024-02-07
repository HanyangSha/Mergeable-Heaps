#include <bits/stdc++.h>
using namespace std;

const int inf = 1000000007; 

class Node {
public: 
Node* parent; 
Node* right; Node* left; 
Node* child; 
int value, degree; 
bool marked; 

Node(int x) {
    value = x; 
    degree = 0;
    parent = nullptr; 
    child = nullptr; 
    right = this; 
    left = this;
    marked = 0; 
}

~Node() {
    left->right = nullptr; 
    delete right; 
    delete child; 
    // cout << "deleting node of value " << value << endl;
    
}

};

void link(Node* l, Node* r) {
    l->right = r; 
    r->left = l; 
}

class Fibonacci_Heap { 
public: 

Fibonacci_Heap() {
    head = nullptr;
    mn = nullptr;  
}

Node* minimum() {
    return mn; 
}

void insert(int x) { // O(1)
    Node* tmp = new Node(x); 
    insert(tmp);
}

void insert(Node* tmp) { // O(1)
    if (head == nullptr) {
        head = tmp; 
    }
    else {
        Node* tail1 = head->left; 
        link(tail1, tmp); 
        link(tmp, head);
    }

    if (mn == nullptr || tmp->value < mn->value) {
        mn = tmp; 
    }
}

Fibonacci_Heap* merge(Fibonacci_Heap* lbh1, Fibonacci_Heap* lbh2) { // O(1)

    Fibonacci_Heap* res = new Fibonacci_Heap(); 

    Node* h1 = lbh1->head; 
    Node* h2 = lbh2->head; 

    Node* mn1 = lbh1->mn; 
    Node* mn2 = lbh2->mn; 

    if (h1 == nullptr && h2 == nullptr) return res; 
    else if (h1 == nullptr) {
        lbh1->head = nullptr; 
        delete lbh1; 
        return lbh2; 
    }
    else if (h2 == nullptr) {
        lbh2->head = nullptr; 
        delete lbh2; 
        return lbh1; 
    }

    delete lbh1; 
    delete lbh2; 

    Node* tail1 = h1->left; 
    Node* tail2 = h2->left; 
    link(tail1, h2); 
    link(tail2, h1);

    res->head = h1; 
    res->mn = (mn1->value < mn2->value) ? mn1 : mn2; 
    return res; 
}

void extract_minimum() { // lg n
    if (mn == head) {
        head = head->right; 
    }

    Node* previous = mn->left; 
    if (previous == mn) {
        head = nullptr;
    }
    else {
        link(previous,mn->right);
    }

    if (mn->child != nullptr) {
        
        if (head == nullptr) {
            head = mn->child; 
        }
        else {
            link(head->left, mn->child); 
            // link(mn->child->left, head);

            // we still need to set the parent pointer of all children of mn to nullptr. 
            // we will do this in consolidate() 
        }
    }

    mn->child = nullptr; 
    mn->left = mn;
    mn->right = mn; 
    delete mn; 

    consolidate(); 

}

void consolidate() { // also need to update mn  
    vector<Node*> v;

    Node* first = head; 
    
    // we can modify head in this function bc we will construct a completely new root list 
    do {
        Node* next = head->right; 

        head->parent = nullptr; 

        int deg = head->degree; 
        if (deg > (int)(v.size())-1) {
            v.resize(deg+1, nullptr); 
        }

        while (v[deg] != nullptr) {
            Node* other = v[deg]; 

            if (head->value > other->value) {
                swap(other, head);
            }
            { // head->value <= other->value, so other becomes child of head
                
                if (head->child != nullptr) {
                    other->right = head->child; 
                    other->left = head->child->left; 
                    head->child->left->right = other; 
                    head->child->left = other; 

                    // doing this wipes out head->child->left before its right pointer is assigned to other 
                    // link(other, head->child);
                    // link(head->child->left,other);
                }
                else {
                    other->right = other; 
                    other->left = other; 
                }

                head->child = other; 
                other->parent = head; 
                head->degree ++;
                v[deg] = nullptr; 
            }

            deg++; 
            if (deg > (int)(v.size())-1) 
                v.resize(deg+1, nullptr); 
        }

        // root nodes' left and right should be set to 0, and will be set later 
        head->left = nullptr; 
        head->right = nullptr; 

        v[deg] = head; 
        head = next; 
    } while (head != first) ;

    int mn_val = inf; 
    Node* previous = nullptr; 
    for (int i = 0; i < v.size(); i++) if (v[i] != nullptr) {
        if (mn_val == inf || mn->value > v[i]->value) { 
            mn = v[i]; 
            mn_val = v[i]->value;
        }

        if (previous == nullptr) {
            head = v[i];
            head->right = head; // in case there is only 1 root. head->left = head is already done by link(previous, head)
        }
        else {
            link(previous->right, v[i]);
        }
        previous = v[i]; 
    }

    link(previous, head); // link last root node with first root node 
}

void decrease_key(Node* x, int key) { // amortized O(1) 
    // it only makes sense to decrease the key in a min heap
    if (key >= x->value) return;

    x->value = key; 
    if (key < mn->value) mn = x; // actually easier to do than binomial heap because the new min just goes to root list instead of being trickled up

    if (x->parent != nullptr && x->value < x->parent->value) // heap order violated 
        cut(x);
}

void cut(Node* x) {

    if (x->parent == nullptr) { // already at root list 
        return; 
    }

    // remove from child list 
    
    if (x->right == x) { // removing the only child node
        x->parent->child = nullptr; 
    }
    else {
        if (x->parent->child == x) { // pointing to x (not the only child node), point to another child
            x->parent->child = x->left; 
        }
        link(x->left, x->right);
    }
    
    // move it to the root list 
    link(head->left, x);
    link(x, head);
    // head->left->right = x; 
    // x->left = head->left; 
    // x->right = head; 
    // head->left = x; 

    if (!x->parent->marked) {
        x->parent->marked = 1; 
    }
    else { // already marked, perform cascading cut 
        cut(x->parent);
        x->marked = 0; 
    }
    
    x->parent = nullptr; // x is in root list so there is no parent 
}

void delete_element(Node* x) { // lg n 
    decrease_key(x, -inf); 
    extract_minimum(); 
}

~Fibonacci_Heap() {
    delete head; 
    head = nullptr; 
}

private: 
Node* head; // first element of the root level  
Node* mn; 
};


int main() {

    Fibonacci_Heap* x = new Fibonacci_Heap(); 
    vector<Node*> v(10); 
    for (int i = 1; i < 10; i++) {
        v[i] = new Node(i);
        x->insert(v[i]);
    }
    
    cout << x->minimum()->value << endl; 
    x->extract_minimum();
    x->decrease_key(v[8], 1); 
    cout << x->minimum()->value << endl; 

    delete x; 

    return 0;
}
