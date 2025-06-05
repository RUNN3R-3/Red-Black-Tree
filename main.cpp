#include <iostream>
#include <cstring>
#include <fstream>

using namespace std;

/*
  rules
  - a node is either red or black
  - The node leaves are black
  - If a node is red, then its children are black.
  - All paths from a node to its leaves contain the same number of black nodes
 */

class Node {
public:

  int value;
  bool red;
  Node* left;
  Node* right;
  Node* parent;
  
};

void leftTRotate(Node*& root, Node* head) {

  if (head->parent->parent != NULL) {
    Node* g = head->parent->parent;
    if (head->parent == g->left)
      Node* u = g->right;
    if (head->parent == g->right)
      Node* u = g->left;

    g->left = head;
    head->left = head->parent;
    head->left->parent = head;
    head->parent = g;
    head->left->red = false;
  }
  
}
void rightTRotate(Node*& root, Node* head) {

  if (head->parent->parent != NULL) {
    Node* g = head->parent->parent;
    if (head->parent == g->left)
      Node* u = g->right;
    if (head->parent == g->right)
      Node* u = g->left;

    g->right = head;
    head->right = head->parent;
    head->right->parent = head;
    head->parent = g;
    head->right->red = false;
  }

}

void leftLRotate(Node*& root, Node* head) {

  if (head->parent->parent != NULL) {
    Node* g = head->parent->parent;
    if (head->parent == g->left)
      Node* u = g->right;
    if (head->parent == g->right)
      Node* u = g->left;

    if (head->parent->left != NULL)
      g->right = head->parent->left;
    g->parent = head->parent;
    if (g == root)
      root = head->parent;
    head->parent->left = g;
    g->red = true;
    head->parent->red = false;
  }
  
}
void rightLRotate(Node*& root, Node* head) {

  if (head->parent->parent != NULL) {
    Node* g = head->parent->parent;
    if (head->parent == g->left)
      Node* u = g->right;
    if (head->parent == g->right)
      Node* u = g->left;

    if (head->parent->right != NULL)
      g->left = head->parent->right;
    g->parent = head->parent;
    if (g == root)
      root = head->parent;
    head->parent->right = g;
    g->red = true;
    head->parent->red = false;
  }
  
}

/*
  - insert n and color it red
  - recolor and rotate nodes to fix violations
  *cases...
  +n = root
  +n->uncle = red
  +n->uncle = black (triangle)
  +n->uncle = black (line)
*/
void filter(Node*& root, Node* head) {

  if (head->parent->parent != NULL) {
    Node* g = head->parent->parent;
    Node* u = NULL;
    if (head->parent == g->left)
      u = g->right;
    if (head->parent == g->right)
      u = g->left;

    if (u != NULL && u->red == true) {
      u->red = false;
      head->parent->red = false;
      g->red = true;

      if (g != root) {
	filter(root, g);
      }
    }
    if (head->parent == g->left) {
      if ((u == NULL || u->red == false) && head == head->parent->right && (g->left != NULL && g->left == head->parent) && (g->right == NULL || u == g->right) && head->parent->left == NULL) {
	//left rotate black triangle
	leftTRotate(root, head);
      }
      else if ((u == NULL || u->red == false) && head == head->parent->left && u == g->right) {
	//right rotate black line
	rightLRotate(root, head);
      }
    }
    if (head->parent == g->right) {
      if ((u == NULL || u->red == false) && head == head->parent->left && (g->right != NULL && g->right == head->parent) && (g->left == NULL || u == g->left) && head->parent->right == NULL) {
      //right rotate black triangle
      rightTRotate(root, head);
    }
    else if ((u == NULL || u->red == false) && head == head->parent->right && u == g->left) {
      //left rotate black line
      leftLRotate(root, head);
    }
    }
  }
  
  root->red = false;
}

void add(Node*& root, Node* head, int in) {

  if (root == NULL) {
    Node* newNode = new Node();
    root = newNode;
    newNode->value = in;
    newNode->red = true;
  }

  else {
    if (in >= head->value) {

      if (head->right != NULL) {
        add(root, head->right, in);
      }
      else if (head->right == NULL) {
        Node* newNode = new Node();
        newNode->value = in;
	newNode->red = true;
	head->right = newNode;
	newNode->parent = head;
	filter(root, newNode);
      }

    }
    else if (in < head->value) {

      if (head->left != NULL) {
        add(root, head->left, in);
      }
      else if (head->left == NULL) {
        Node* newNode = new Node();
        newNode->value = in;
        newNode->red = true;
	head->left = newNode;
        newNode->parent = head;
        filter(root, newNode);
      }

    }
  }

  root->red = false;
}

void print(Node* head, int depth) {

  if (head->right != NULL){
    print(head->right, depth + 1);
  }

  for (int j = 0; j < depth; j++) {
    cout << '\t';
  }
  cout << head->value;
  if (head->red == true)
    cout << "r" << endl;
  else
    cout << "b" << endl;

  if (head->left != NULL) {
    print(head->left, depth + 1);
  }
}

void del(Node* head, int in, Node*& root) {
  //when there is no children
  if (head->right == NULL && head->left == NULL && head->value == in) {
    if (head->parent->right == head) {
      head->parent->right = NULL;
      head->parent = NULL;
    }
    else if (head->parent->left == head) {
      head->parent->left = NULL;
      head->parent = NULL;
    }
  }

  //A right child only
  else if (head->right != NULL && head->left == NULL && head != root) {
    if (head->parent->right == head) {
      head->parent->right = head->right;
      head->right->parent = head->parent;
      head = head->right;
    }
    else if (head->parent->left == head) {
      head->parent->left = head->right;
      head->right->parent = head->parent;
      head = head->right;
    }
  }

  //A right child only of the root
  else if (head->right != NULL && head->left == NULL && head == root) {
    root = head->right;
    head->right = NULL;
    root->parent = NULL;
    head = root;
  }

  //A left child only
  else if (head->left != NULL && head->right == NULL && head != root) {
    if (head->parent->right == head) {
      head->parent->right = head->left;
      head->left->parent = head->parent;
      head = head->left;
    }
    else if (head->parent->left == head) {
      head->parent->left = head->left;
      head->left->parent = head->parent;
      head = head->left;
    }

  }

  //A left child only of the root
  else if (head->left != NULL && head->right == NULL && head == root) {
    root = head->left;
    head->right = NULL;
    root->parent = NULL;
    head = root;
  }

  //Two children
  else if (head->right != NULL && head->left != NULL) {
    Node* temp = head->right;
    while (temp->left != NULL) {
      temp = temp->left;
    }
    head->value = temp->value;
    del(temp, temp->value, root);
  }
}


void find(Node* head, int in, Node*& root){
   if (in >= head->value) {

      if (head->right != NULL && head->value != in) {
        head = head->right;
        find(head, in, root);
      }

      else if (head->value == in) {
        del(head, in, root);
      }

      else if (head->right == NULL) {
        cout << "Data point was not found" << endl;
      }

    }
    else if (in < head->value) {

      if (head->left != NULL && head->value != in) {
        head = head->left;
        find(head, in, root);
      }

      else if (head->value == in) {
        del(head, in, root);
      }

      else if (head->left == NULL) {
        cout << "Data point was not found" << endl;
      }

    }
}

int main() {

  int depth = 0;
  int in = 0;
  char input[10];
  int amount = 0;
  bool working = true;
  Node* root = NULL;
  Node* head = NULL;

  cout << endl << "Welcome to Red-Black Tree, please choose one of the following options..." << endl;

  //Continuous statements so you can use it for as long as you want
  while (working == true) {
    //Starting statement
    cout << endl << "ADD ... PRINT ... DELETE ... QUIT" << endl << " -  ";
    cin >> input;

    if (input[0] == 'A') {
      cout << "From the console or file?" << endl;
      cin >> input;

      if (input[0] == 'c') {
        cout << endl << "What is the value?" << endl;
        cin >> in;
        add(root, head, in);
        head = root;
      }
      else if (input[0] == 'f') {
        cout << endl << "How many #'s should be added - ";
        cin >> amount;
        ifstream fin ("num.txt");
        for (int a = 0; a < amount; a++) {
          fin >> in;
          add(root, head, in);
          head = root;
        }
        fin.close();
      }
    }

    else if (input[0] == 'P') {
      print(head, depth);
    }

    else if (input[0] == 'D') {
      cout << "What number do you want to delete from the tree?" << endl;
      cin >> in;
      //find(head, in, root);
      cout << "DONE!" << endl;
      head = root;
    }

    else if (input[0] == 'Q') {
      cout << "Goodbye";
      working = false;
    }
    else {
      cout << "ERROR -- Try again" << endl;
    }

  }

  return 0;
}
