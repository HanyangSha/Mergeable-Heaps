#include <bits/stdc++.h>
using namespace std;

const int inf = 1000000007;

// https://stackoverflow.com/questions/19508526/what-is-the-intuition-behind-the-fibonacci-heap-data-structure 
// https://www.cs.toronto.edu/~anikolov/CSC265F19/binomial-heaps.pdf 

class Node {
public: 
Node* parent; 
Node* right; Node* left; 
Node* child; 
int value, degree; 

Node(int x) {
    value = x; 
    degree = 0;
    parent = nullptr; 
    child = nullptr; 
    right = this; 
    left = this;
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

class Lazy_Binomial_Heap { 
public: 

Lazy_Binomial_Heap() {
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

void insert(Node* tmp) {
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

Lazy_Binomial_Heap* merge(Lazy_Binomial_Heap* lbh1, Lazy_Binomial_Heap* lbh2) { // O(1)

    Lazy_Binomial_Heap* res = new Lazy_Binomial_Heap(); 

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
            { // other becomes child of head
                
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

void decrease_key(Node* x, int key) { // trickle up bc min heap, lg n 
    // it only makes sense to decrease the key in a min heap
    if (key >= x->value) return;

    x->value = key; 

    bool flag = 0; // whether a new min has been found; if found, it will be at the root of its tree 
    if (key < mn->value) flag = 1; 

    Node* par = x->parent; 
    while (par != nullptr && par->value > x->value) {
        swap(par->value, x->value);
        x = par; 
        par = par->parent;
    }

    if (flag) mn = x; 
}

void delete_element(Node* x) { // lg n 
    decrease_key(x, -inf); 
    extract_minimum(); 
}

~Lazy_Binomial_Heap() {
    delete head; 
    head = nullptr; 
}

private: 
Node* head; // first element of the root level  
Node* mn; 
};


int main() {

    Lazy_Binomial_Heap* x = new Lazy_Binomial_Heap(); 
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
