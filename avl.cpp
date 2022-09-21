#include <iostream>
#include <cmath>

using namespace std;

//The AVL tree node
class Node {
    public:
    Node *left;
    Node *right;
    int key;
    int height;
    int lsize;
    int rsize;
    int count;
};

//Get the height of the tree rooted at node
int height(Node *node) {
    if (node == nullptr) return 0;
    return node->height;
}

//Get the greater number between two
int max(int a, int b) {
    if (a > b) return a;
    return b;
}

//Construct a new node with a given key
Node* createNode(int key) {
    Node* node = new Node();
    node->key = key;
    node->left = nullptr;
    node->right = nullptr;
    node->count = 1;
    node->height = 1; //By default height = 1
    return node;
}

//Right rotate the subtree rooted at y
Node *rightRotate(Node *y) {
    Node *x = y->left;
    Node *xr = x->right;
    //Rotate
    x->right = y;
    y->left = xr;
    //Update height
    y->height = max(height(y->left), height(y->right)) + 1;
    x->height = max(height(x->left), height(x->right)) + 1;
    //Update size
    int xrsize = x->rsize;
    int yrsize = y->rsize;
    x->rsize = xrsize + yrsize + y->count;
    y->lsize = xrsize;
    //Return the rotated tree rooted at x
    return x;
}

//Left rotate the subtree rooted at x
Node *leftRotate(Node *x) {
    Node *y = x->right;
    Node *yl = y->left;
    //Rotate
    y->left = x;
    x->right = yl;
    //Update height
    x->height = max(height(x->left), height(x->right)) + 1;
    y->height = max(height(y->left), height(y->right)) + 1;
    //Update size
    int xlsize = x->lsize;
    int ylsize = y->lsize;
    x->rsize = ylsize;
    y->lsize = xlsize + x->count + ylsize;
    //Return the rotated tree rooted at y
    return y;
}

//Get Balance factor of node
int getBalanceFactor(Node* node){
    if (node == nullptr) return 0;
    int BF = height(node->left) - height(node->right);
    return BF;
}

//Insert a new node with a given key to the tree rooted at node
Node* insertNode(Node* node, int key) {
    //Step 1: Locate and insert the node
    if (node == nullptr) return(createNode(key));
    if (key < node->key){
        node->left = insertNode(node->left, key);
        node->lsize++;
    }
    else if (key > node->key){
        node->right = insertNode(node->right, key);
        node->rsize++;
    }
    else{
        node->count++;
    }

    //Step 2: Update height
    node->height = 1 + max(height(node->left), height(node->right));

    //Step 3: Calculate the balance factor
    int bf = getBalanceFactor(node);

    //If unbalanced
    //Left Left - Single Rotation
    if (bf > 1 && key < node->left->key)
        return rightRotate(node);
    //Right Right - Single Rotation
    if (bf < -1 && key > node->right->key)
        return leftRotate(node);
    //Left Right Case - Double Rotation
    if (bf > 1 && key > node->left->key) {
        node->left = leftRotate(node->left);
        return rightRotate(node);
    }
    //Right Left Case - Double Rotation
    if (bf < -1 && key < node->right->key) {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }

    //If balanced
    return node;
}

//Get the parent of the predecessor of root
//Update rsize along the way
Node * maxValueNodeParent(Node* root) {
    Node* current = root->left;
    if (current->right == nullptr){
        return root;
    }
    current->rsize--;
    //Find the rightmost leaf's parent
    while (current->right->right != nullptr){
        current = current->right;
        current->rsize--;
    }
    return current;
}

bool search(Node* root, int key){
    if (root == nullptr) return false;
    if (key < root->key){
        return search(root->left, key);
    }
    else if(key > root->key){
        return search(root->right, key);
    }
    else return true;
}

//Delete a node with a given key from the tree rooted at root
Node* deleteNode(Node* root, int key, bool exist = false) {
    //Step 0: Check if the key exists
    if (!exist){
        if (!search(root, key)){
            return root;
        }
    }
    //Step 1: Locate and delete the node
    if (root == nullptr)
        return root;
    if (key < root->key){
        root->left = deleteNode(root->left, key, true);
        root->lsize--;
    }
    else if(key > root->key){
        root->right = deleteNode(root->right, key, true);
        root->rsize--;
    }
    else{
        //If count > 1, just count-- and return
        if (root->count > 1){
            root->count--;
            return root;
        }
        //0 child
        if( (root->left == nullptr) && (root->right == nullptr) ){
            delete root;
            return nullptr;
        }

        //1 child
        else if( (root->left == nullptr) || (root->right == nullptr) ){
            Node * node;
            if (root->left){
                node = root->left;
            }
            else{
                node = root->right;
            }
            *root = *node;
            delete node;
        }

        //2 children
        else{
            Node* node;
            Node* parent = maxValueNodeParent(root);
            if (parent == root) node = root->left;
            else node = parent->right;
            root->key = node->key;
            root->lsize--;
            if (node->left){
                *node = *node->left;
                delete node->left;
            }
            else{
                if (parent == root){
                    root->left = nullptr;
                    delete node;
                }
                else{
                    parent->right = nullptr;
                    delete node;
                }
            }
        }
    }

    //Step 2: Update height
    root->height = 1 + max(height(root->left), height(root->right));

    //Step 3: Calculate the balance factor
    int bf = getBalanceFactor(root);

    //If unbalanced
    //Left Left - Single Rotation
    if (bf > 1 && getBalanceFactor(root->left) >= 0)
        return rightRotate(root);

    //Right Right - Single Rotation
    if (bf < -1 && getBalanceFactor(root->right) <= 0)
        return leftRotate(root);

    //Left Right - Double Rotation
    if (bf > 1 && getBalanceFactor(root->left) < 0){
        root->left = leftRotate(root->left);
        return rightRotate(root);
    }

    //Right Left - Double Rotation
    if (bf < -1 && getBalanceFactor(root->right) > 0){
        root->right = rightRotate(root->right);
        return leftRotate(root);
    }

    //If balanced
    return root;
}

//Print the keys in pre-order
void preorderPrint(Node *root){
    if(root != nullptr){
        for (int i=0; i<root->count; i++){
            cout << root->key << " ";
        }
        preorderPrint(root->left);
        preorderPrint(root->right);
    }
}

//Delete the tree
void deleteTree(Node *root){
    if(root != nullptr){
        deleteTree(root->left);
        deleteTree(root->right);
        delete root;
    }
}

static int kthCount = 0;

//Return the kth largest number
int kth(Node* root, int k){
    kthCount++;
    if (root == nullptr){
        return 0;
    }
    if (k <= root->rsize){
        return kth(root->right, k);
    }
    else if (root->rsize+1 <= k && k <= root->rsize+root->count){
        return root->key;
    }
    // rsize+1 < k <= lsize+count+rsize
    else{
        return kth(root->left, k-root->rsize-root->count);
    }
}

//Return the median
int median(Node* root){
    if (root == nullptr) return 0;
    return kth(root, (root->lsize+root->rsize+root->count+1)/2);
}

int main(){

    Node* root = nullptr;

    int insertCount, deleteCount;

    cout << "Input the number of keys to insert: ";
    cin >> insertCount;
    cout << "Input the keys to insert:" << endl;

    for (int i=0; i<insertCount; i++){
        int key;
        cin >> key;
        root = insertNode(root, key);
    }

    cout << "Preorder tree:" << endl;
    preorderPrint(root);
    cout << endl;
    kthCount = 0;
    cout << "N = " << root->lsize+root->rsize+root->count << endl;
    cout << "median = " << median(root) << endl;
    cout << "logN+1 = " << log(root->lsize+root->rsize+root->count)+1 << endl;
    cout << "kth time = " << kthCount << endl;

    cout << "Input the number of keys to delete: ";
    cin >> deleteCount;
    cout << "Input the keys to delete:" << endl;

    for (int i=0; i<deleteCount; i++){
        int key;
        cin >> key;
        root = deleteNode(root, key);
    }

    cout << "Preorder tree:" << endl;
    preorderPrint(root);
    cout << endl;
    kthCount = 0;
    cout << "N = " << root->lsize+root->rsize+root->count << endl;
    cout << "median = " << median(root) << endl;
    cout << "logN+1 = " << log(root->lsize+root->rsize+root->count)+1 << endl;
    cout << "kth time = " << kthCount << endl;

    deleteTree(root);
}
