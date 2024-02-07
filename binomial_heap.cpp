#include <bits/stdc++.h>
using namespace std;

const int inf = 1000000007;

// https://stackoverflow.com/questions/19508526/what-is-the-intuition-behind-the-fibonacci-heap-data-structure?rq=3
// https://www.cs.toronto.edu/~anikolov/CSC265F19/binomial-heaps.pdf 

class Node {
public: 
Node* parent; 
Node* right;
Node* child; 
int value, degree; 

Node(int x) {
    value = x; 
    degree = 0;
    parent = nullptr; 
    child = nullptr; 
    right = nullptr; 
}

~Node() {
    delete child; 
    delete right; 
    // cout << "deleting node of value " << value << endl;
}

};

class Binomial_Heap { 
public: 

Binomial_Heap() {
    head = nullptr; 
}

pair<Node*, Node*> minimum_and_prev() { // lg n 
    Node* ans = nullptr; 
    Node* ans_previous = nullptr;
    int mn = inf; 

    Node* cur = head; 
    Node* previous = nullptr; 
    while (cur != nullptr) {
        if (cur->value < mn) {
            mn = cur->value;
            ans_previous = previous; 
            ans = cur; 
        }
        previous = cur; 
        cur = cur->right; 
    }
    return make_pair(ans, ans_previous); 
}

Node* minimum() {
    return minimum_and_prev().first; 
}

Node* merge(Node* h1, Node* h2) { // lg n 

    if (h1 == nullptr && h2 == nullptr) return nullptr; 
    else if (h1 == nullptr) return h2; 
    else if (h2 == nullptr) return h1; 

    Node* list_start = nullptr; 
    if (h1->degree < h2->degree) {
        list_start = h1; 
        h1 = h1->right; 
    }
    else {
        list_start = h2; 
        h2 = h2->right; 
    }

    Node* it = list_start; 

    while (h1 != nullptr && h2 != nullptr) {
        if (h1->degree < h2->degree) {
            it->right = h1; 
            h1 = h1->right; 
        }
        else {
            it->right = h2; 
            h2 = h2->right; 
        }
        it = it->right; 
    }

    while (h1 != nullptr) {
        it->right = h1; 
        h1 = h1->right; 
        it = it->right; 
    }
    while (h2 != nullptr) {
        it->right = h2;
        h2 = h2->right; 
        it = it->right; 
    }

    // at most 2 binomial trees of any order k
    Node* previous = nullptr; 
    Node* cur = list_start; 
    Node* next = cur->right; 

    while (next != nullptr) {
        if (cur->degree != next->degree) {
            previous = cur; 
            cur = next; 
            next = next->right; 
        }
        else { // note that the children are actually stored in descending order by value 
            if (cur->value < next->value) { // next becomes child of cur
                Node* nextnext = next->right; 

                next->right = cur->child;
                cur->child = next; next->parent = cur; 
                cur->degree ++;
                
                cur->right = nextnext; 
                next = nextnext; 

                // do not advance cur
            }
            else { // cur becomes child of next
                if (previous == nullptr) list_start = next; 
                else previous->right = next; 

                cur->right = next->child; 
                next->child = cur; cur->parent = next; 
                next->degree ++;

                cur = next; 
                next = next->right; 

                // do not advance previous 
            }
        }
    }

    return list_start; 

}

void insert(int x) { // lg n
    Node* tmp = new Node(x); 
    head = merge(tmp, head);
}

void insert(Node* x) {
    head = merge(x, head);
} 

void extract_minimum() { // lg n
    pair<Node*, Node*> x = minimum_and_prev(); 
    Node* mn = x.first; 
    Node* previous = x.second; 

    if (previous == nullptr) {
        head = mn->right; 
    }
    else {
        previous->right = mn->right;
    }

    if (mn->child == nullptr) {
        mn->right = nullptr; 
        delete mn; 
        return ;
    }

    // reverse the order of x->child and set it to new_heap_head
    Node* cur = mn->child; 
    Node* next = cur->right; 
    cur->right = nullptr; 
    cur->parent = nullptr; 

    while (next != nullptr) {
        next->parent = nullptr; 

        Node* nextnext = next->right; 
        next->right = cur;
        cur = next; 
        next = nextnext; 
    }

    mn->child = nullptr; 
    mn->right = nullptr; 
    delete mn; 

    Node* new_heap_head = cur;
    head = merge(head, new_heap_head);  

}

void decrease_key(Node* x, int key) { // trickle up bc min heap, lg n 
    // it only makes sense to decrease the key in a min heap
    if (key >= x->value) return;

    x->value = key; 
    Node* par = x->parent; 
    while (par != nullptr && par->value > x->value) {        
        swap(par->value, x->value);
        x = par; 
        par = par->parent;
    }
}

void delete_element(Node* x) { // lg n 
    decrease_key(x, -inf); 
    extract_minimum(); 
}

~Binomial_Heap() {
    delete head; 
    head = nullptr; 
}

private: 
Node* head; // first element of the root level  
};


int main() {

    Binomial_Heap x; 
    // vector<Node*> v(10); 
    // for (int i = 1; i < 4; i++) {
    //     v[i] = new Node(i);
    //     x.insert(v[i]);
    // }
    
    // cout << x.minimum()->value << endl; 
    // x.extract_minimum();
    // x.decrease_key(v[3], 1); 
    // cout << x.minimum()->value << endl; 

    vector<Node*> v(10); 
    for (int i = 1; i < 10; i++) {
        v[i] = new Node(i);
        x.insert(v[i]);
    }
    
    cout << x.minimum()->value << endl; 
    x.extract_minimum();
    x.decrease_key(v[8], 1); 
    cout << x.minimum()->value << endl; 

    return 0;
}
