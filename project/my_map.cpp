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

        void RB_insert_balancing(node* z, node* root){
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
                if(strcmp(key.c_str(), curr->key.c_str()) == 0){
                    cout << strcmp(key.c_str(), curr->key.c_str()) << "\n";
                    return root;
                }
                else if(strcmp(key.c_str(), curr->key.c_str()) < 0) {
                    cout << strcmp(key.c_str(), curr->key.c_str()) << "\n";
                    curr = curr->left;   
                }
                else{
                    cout << strcmp(key.c_str(), curr->key.c_str()) << "\n";
                    curr = curr->right;
                }
            }

            node* z = new node();
            z->key = key;
            z->value = value;

            if(parent != nullptr){
                if (parent->key < key) parent->right = z;
                else parent->left = z;
            }else{
                root = z;
            }

            RB_insert_balancing(z, root);

            return(root);
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
                cout << "오른쪽으로 이동\n";
                while(curr->left != nullptr){
                    curr = curr->left;
                    cout << "왼쪽으로 이동\n";
                }
            }else{
                node<T_key, T_value>* parent = curr->parent;
                while(parent!= nullptr && (curr == parent->right)){
                    curr = parent;
                    parent = parent->parent;
                }
                curr = parent;
                cout << "위로으로 이동\n";
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

        iterator begin(){
            node<T_key, T_value>* curr = root;
            while(root->left != nullptr){
                root = root->left;
            }
            return iterator(root);
        }

        iterator end(){
            return iterator(nullptr);
        }
};

template <typename T_key, typename T_value>
void print_map(my_map<T_key, T_value> m) {
    my_map<string, int>::iterator iter;
    for (iter = m.begin(); iter != m.end(); ++iter) {
        cout << iter->key << ": " << iter->value << '\n';
    }
}

int main(){

    my_map<string, int> m;

    cout << "** First Step **\n";
    m.insert(make_pair("Global", 10));
    print_map(m);
    cout << "** First Step **\n";
    m.insert(make_pair("Handong", 30));
    print_map(m);
    cout << "** First Step **\n";
    m.insert(make_pair("CSEE", 20));
    print_map(m);
    cout << "** First Step **\n";
    m.insert(make_pair("MCNL", 15));
    print_map(m);

    return 0;
}

/*
insert 오류 찾고
delete 구현하기
아직 미완성
*/
