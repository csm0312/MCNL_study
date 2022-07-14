/*
c++ STL map 만들기
Red Black tree 기반으로 이루어진 map 만들기
◼ Class template: key, value type에 대한 generic class를 정의
◼ insert(): key, value 삽입
◼ erase(): key 삭제
◼ find(): key 검색
◼ begin(): 시작 iterator
◼ end(): 끝 iterator
◼ [ ] operator: key로 map에 접근
→ Operator overloading ◼ iterator: first(=key), second(=value)
*/

#include <iostream>
#include <string>
#include <cstring>

using namespace std;

enum Color{
    BLACK, RED
};

template <typename T_key, typename T_value>
class node{
    public:
        T_key key; //key = string
        T_value value; // value = int
        node* left; //child
        node* right; //child
        node* parent;
        Color color; //RED, BLACK
    
    public:
        node(): left(nullptr), right(nullptr), parent(nullptr), color(RED){}
        ~node(){}

        node* find(T_key key, node* root){
            node* curr = root;
            if (curr == nullptr) return nullptr;
            if(strcmp(key.c_str(), curr->key.c_str())==0) return curr;
            else if(strcmp(key.c_str(),curr->key.c_str()) < 0) return find(key, curr->left);
            else return find(key, curr->right);
        }
        
        void left_Rotate(node* x, node* root){
            node* y = x->right;
            x->right = y->left;
            if(y->left != nullptr) y->left->parent = x;
            y->parent = x->parent;

            if(x->parent == nullptr) root = y;
            else if(x == x->parent->left) x->parent->left = y;
            else x->parent->right = y;

            y->left = x;
            x->parent = y;
        }

        void right_Rotate(node* y, node* root){
            node* x = y->left;
            y->left = x->right;
            if(y->right != nullptr) y->right->parent = y;
            x->parent = y->parent;

            if(y->parent == nullptr) root = x;
            else if(y == y->parent->left) y->parent->left = x;
            else y->parent->right = x;

            x->right = y;
            y->parent = x; 
        }

        node* RB_successor(node* x){
            node* y;
            if (x->right != nullptr){
                x = x->right;
                while(x->left != nullptr) y = x->left;
                return y;
            }
            
            y = x->parent;
            while (y != nullptr && x == y->right){
                x = y;
                y = x->parent;
            }
            return y;
        }

        void RB_insertion(node* z, node* root){
            while(z->parent != nullptr && z->parent->color == RED){
                if(z->parent == z->parent->parent->left){
                    node* uncle = z->parent->parent->right;
                    if(uncle && (uncle->color == RED)){
                        z->parent->color = BLACK;
                        uncle->color = BLACK;
                        z->parent->parent->color = RED;
                        z = z->parent->parent;
                    }else{
                        if(z == z->parent->right){
                            z = z->parent;
                            left_Rotate(z, root);
                        }
                        z->parent->color = BLACK;
                        z->parent->parent->color = RED;
                        right_Rotate(z->parent->parent, root);
                    }
                }else{
                    node* uncle = z->parent->parent->left;
                    if(uncle && (uncle->color == RED)){
                        z->parent->color = BLACK;
                        uncle->color = BLACK;
                        z = z->parent->parent;
                    }else{
                        if(z == z->parent->left){
                            z = z->parent;
                            right_Rotate(z, root);
                        }
                        z->parent->color = BLACK;
                        z->parent->parent->color = RED;
                        left_Rotate(z->parent->parent, root);
                    }
                }
            }
            root->color = BLACK;
        }

        node* RB_insert(T_key key, T_value value, node* root){
            node* parent = nullptr;
            node* curr = root;
            while(curr != nullptr){
                parent = curr;
                if(strcmp(key.c_str(), curr->key.c_str()) == 0) return root;
                else if(strcmp(key.c_str(), curr->key.c_str()) < 0) curr = curr->left;   
                else curr = curr->right;
            }

            node* z = new node();
            z->key = key;
            z->value = value;

            if(parent != nullptr){
                if (parent->key < key){
                    parent->right = z;
                    z->parent = parent;
                }
                else {
                    parent->left = z;
                    z->parent = parent;
                }
            }else{
                root = z;
            }

            if(parent == nullptr){
                return root;
            }

            if(parent == root){
                return root;
            }

            RB_insertion(z, root);

            return root;
        }

        void RB_deletion(node* x, node* root){
            node* w;

            while(x != root && x->color == BLACK){
                if(x == x->parent->left){
                    w = x->parent->right;
                    if(w->color == RED){
                        w->color = BLACK;
                        x->parent->color = RED;
                        left_Rotate(x->parent, root);
                        w = x->parent->right;
                    }

                    if(w->left->color == BLACK && w->right->color == BLACK){
                        w->color = RED;
                        x = x->parent;
                    }else if(w->right->color == BLACK){
                        w->left->color = BLACK;
                        w->color = RED;
                        right_Rotate(w, root);
                        w = x->parent->right;
                    }

                    w->color = x->parent->color;
                    x->parent->color = BLACK;
                    x->right->color = BLACK;
                    left_Rotate(x->parent, root);
                    x = root;
                }else{
                    if(x == x->parent->right){
                        w = x->parent->left;
                        if(w->color == RED){
                            w->color = BLACK;
                            x->parent->color = RED;
                            right_Rotate(x->parent, root);
                            w = x->parent->left;
                        }

                        if(w->right->color == BLACK && w->left->color == BLACK){
                            w->color = RED;
                            x = x->parent;
                        }else if(w->right->color == BLACK){
                            w->left->color = BLACK;
                            w->color = RED;
                            left_Rotate(w, root);
                            w = x->parent->left;
                        }
                    
                        w->color = x->parent->color;
                        x->parent->color = BLACK;
                        x->right->color = BLACK;
                        right_Rotate(x->parent, root);
                        x = root;
                    }
                }
            }
            x->color = BLACK;
        }

        node* RB_delete(T_key key, node* root){
            node* z = find(key, root);
            if(z==nullptr) return root;

            //case 1. no child
            if(z->left == nullptr && z->right == nullptr){
                if(z == z->parent->left) z->parent->left = nullptr;
                else z->parent->right = nullptr;
            }


            //case 2. one child
            //case 2-1. have a left child
            //else if(z->left != nullptr && z->right == nullptr){

            //}

            // if(z->left == nullptr || z->right == nullptr) y = z;
            // else{
            //     y = RB_successor(z);
            // }

            // if(y->left != nullptr) x = y->left;
            // else x = y->right;

            // x->parent = y->parent;

            // if(y->parent == nullptr) root = x;
            // else if(y == y->parent->left) y->parent->left = x;
            // else y->parent->right = x;

            // if(y != z){
            //     z->key = y->key;
            //     z->value = y->value;
            //     z->parent = y->parent;
            //     z->left = y->left;
            //     z->right = y->right;
            // }

            // if(y->color == BLACK) RB_deletion(x, root);

            // return y;

            return root;
        }
};

template <typename T_key, typename T_value>
class Myiterator{
    public:
        node<T_key, T_value>* curr;
    public:
        Myiterator(node<T_key, T_value>* p=nullptr): curr(p){}

        Myiterator& operator++(){
            if(curr->right != nullptr){
                curr = curr->right;
                while(curr->left != nullptr){
                    curr = curr->left;
                }
            }else{
                node<T_key, T_value>* parent = curr->parent;
                while(parent!= nullptr && (curr == parent->right)){
                    curr = parent;
                    parent = parent->parent;
                }
                curr = parent;
            }

            return *this;
        }
        
        T_key& operator*(){
            return curr->key;
        }

        bool operator==(const Myiterator &ref){
            return curr == ref.curr;
        }

        bool operator!=(const Myiterator &ref){
            return curr != ref.curr;
        }

        node<T_key, T_value>* operator->(){
            return curr;
        }
};

template <typename T_key, typename T_value>
class my_map{
    private:
        node<T_key, T_value>* root;
    public:
        my_map() : root(nullptr){}

        void insert(pair<T_key, T_value> p){
            root = root->RB_insert(p.first, p.second, root);
        }

        typedef Myiterator<T_key, T_value> iterator;

        void erase(T_key key){
            root = root->RB_delete(key, root);
        }

        iterator find(T_key key){
            return iterator(root->find(key, root));
        }

        iterator begin(){
            node<T_key, T_value>* curr = root;
            while(curr->left != nullptr){
                curr = curr->left;
            }
            return iterator(curr);
        }

        iterator end(){
            return iterator(nullptr);
        }

        T_value& operator[](T_key key){
            my_map<T_key, T_value>::iterator iter = find(key);

            T_value value;

            if(iter == end()){
                insert(make_pair(key, value));
                iter = find(key);
            }

            return iter->value;
        }
};

template <typename T_key, typename T_value>
void print_map(my_map<T_key, T_value> m) {
    my_map<string, int>::iterator iter;
    for (iter = m.begin(); iter != m.end(); ++iter) {
        cout << iter->key << ": " << iter->value << ": " << iter->color <<'\n';
    }
}

int main(){

    my_map<string, int> m;

    cout << "** First Step **\n";
    m.insert(make_pair("Global", 10));
    m.insert(make_pair("Handong", 30));
    m.insert(make_pair("CSEE", 20));
    m.insert(make_pair("MCNL", 15));
    print_map(m);

    cout << "\n** Second Step **\n";
    m["Pohang"] = 50;
    m["Korea"] = 60;
    print_map(m);

    cout << "\n** Third Step **\n";
    m["CSEE"] = 100;
    //m.erase("Global");
    print_map(m);

    cout << "\n** Fourth Step **\n"; 
    string key = "MCNL";
    if (m.find(key) != m.end()) {
        cout << key << " Exists! \n";
    } 
    else {
        cout << key << " does not exist! \n";
    }

    cout << "\n** Fifth Step **\n"; key = "Yunmin";
    if (m.find(key) != m.end()){
        cout << key << "exists! \n";
    } else {
        cout << key << "does not exist!\n";
    }

    return 0;
}
