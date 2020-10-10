//Basak Amasya

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <chrono>
#include <vector>
using namespace std;

//BinarySearchTree and AVL classes are taken from lecture slides and book, and modified according to the homework

template <class Comparable>
class BinarySearchTree;

template <class Comparable>
class BinaryNode //templated node class to be used in binary search tree
{
public:
	Comparable element;
	BinaryNode *left;
	BinaryNode *right;
	BinaryNode(const Comparable & theElement, BinaryNode *lt, BinaryNode *rt): element(theElement), left(lt), right(rt) { }
};

template <class Comparable>
class BinarySearchTree //templated Binary Search Tree class
{
public:
	explicit BinarySearchTree(const Comparable & notFound);
	const Comparable & findMin() const;
	void find(const Comparable & x);
	bool isEmpty() const;
	void printTreeInOrder(ofstream &) const;
	void printTreePreOrder(ofstream &) const;
	void insert(const Comparable & x);
	void remove(const Comparable & x);
	BinaryNode<Comparable> * GetRoot() const;
	void Draw(ofstream &); //added for drawing
	Comparable ITEM_NOT_FOUND; //moved from private to public
	bool found; //added to keep track of not found searches, deletions or additions
private:
	BinaryNode<Comparable> *root;
	const Comparable & elementAt(BinaryNode<Comparable> *t) const;
	void insert(const Comparable & x, BinaryNode<Comparable> * & t);
	void remove(const Comparable & x, BinaryNode<Comparable> * & t);
	BinaryNode<Comparable> * findMin(BinaryNode<Comparable> *t) const;
	vector<BinaryNode<Comparable>*> find(const Comparable & x,	BinaryNode<Comparable> *t, vector<BinaryNode<Comparable>*> &) const;
	void printTreeInOrder(BinaryNode<Comparable> *t, ofstream &) const;
	void printTreePreOrder(BinaryNode<Comparable> *t, ofstream &) const;
	void DrawHelper(BinaryNode<Comparable> * t, ofstream &, string, string, string);
};

template <class Comparable>
BinarySearchTree<Comparable>::BinarySearchTree(const Comparable & notFound) : ITEM_NOT_FOUND(notFound), root(NULL) { } //constructor

template <class Comparable>
const Comparable & BinarySearchTree<Comparable>::elementAt(BinaryNode<Comparable> *t) const
{
	return t == NULL ? ITEM_NOT_FOUND : t->element;
}

template <class Comparable>
void BinarySearchTree<Comparable>::find(const Comparable & x) //modified for partial searches
{
	vector<BinaryNode<Comparable>*> founditems; //keeping a vector for found contacts
	find(x, root, founditems);
	for (int i = 0; i < founditems.size(); i++)
	{
		cout << elementAt(founditems[i]) << endl;
	}
	if (founditems.size() == 0)
	{
		found = false;
	}
	else found = true;
}

template <class Comparable>
vector<BinaryNode<Comparable>*> BinarySearchTree<Comparable>::find(const Comparable & x, BinaryNode<Comparable> * t, vector<BinaryNode<Comparable>*> & found) const
{
	string name, surname, elem;
	if (t)
	{
		elem = t->element;
		Parse(elem);
		elem = elem.substr(0, x.length()); //getting the substring for partial searches
	}
	if (t == NULL)
		return found;
	else if (x < elem)
	{
		find(x, t->left, found);
		return found;
	}
	else if (elem < x)
	{
		find(x, t->right, found);
		return found;
	}
	else
	{
		found.push_back(t);
		if (t)
		{
			find(x, t->left, found);
			find(x, t->right, found);
		}
		return found;
	}
}

template <class Comparable>
void BinarySearchTree<Comparable>::insert(const Comparable & x)
{
	insert(x, root);
}

//function to parse the contact into key (name and surname)
void Parse(string & elem)
{
	istringstream iss(elem);
	string name, surname;
	iss >> name >> surname;
	elem = name + " " + surname;
}

//static istringstream iss;
template <class Comparable>
void BinarySearchTree<Comparable>::insert(const Comparable & x, BinaryNode<Comparable> * & t)
{
	/*string name, surname, keyname, keysurname, elem, key;
	BinaryNode<Comparable> * newnode = new BinaryNode<Comparable>(x, NULL, NULL);
	BinaryNode<Comparable> * temp = root;
	BinaryNode<Comparable> * y = nullptr;
	iss.str(x);
	iss >> keyname >> keysurname;
	key = keyname + " " + keysurname;
	iss.str("");

	while (temp) 
	{
		y = temp;
		iss.str(temp->element);
		iss >> name >> surname;
		elem = name + " " + surname;
		iss.str("");
		if (key < elem)
			temp = temp->left;
		else
			temp = temp->right;
	}

	if (!root)
		root = newnode;

	else if (key < elem)
		y->left = newnode;

	else
		y->right = newnode;*/
	static int count = 0;
	string elem, key;
	if (t)
	{
		elem = t->element;
		Parse(elem); 
		key = x;
		Parse(key);		
	}
	if (t == NULL) // create a new node at the right place
	{
		t = new BinaryNode<Comparable>(x, NULL, NULL);
	}
	else if (key < elem)
		insert(x, t->left); // insert at the left or
	else if (elem < key)
		insert(x, t->right); // right subtree
	else if (key == elem)
	{
		BST.found = true;
	}
}

template <class Comparable>
void BinarySearchTree<Comparable>::remove(const Comparable & x)
{
	remove(x, root);
}

template <class Comparable>
void BinarySearchTree<Comparable>::remove(const Comparable & x, BinaryNode<Comparable> * & t)
{
	string name, surname, elem;
	if (t)
	{
		elem = t->element;
		Parse(elem);
	}
	if (t == NULL)
	{
		return;
	}
	if (x < elem)
		remove(x, t->left);
	else if (elem < x)
		remove(x, t->right);
	else if (t->left != NULL && t->right != NULL) // Two children
	{
		t->element = findMin(t->right)->element;
		elem = t->element;
		Parse(elem);
		remove(elem, t->right);
		found = true;
	}
	else // one or no children
	{
		BinaryNode<Comparable> *oldNode = t;
		t = (t->left != NULL) ? t->left : t->right;
		delete oldNode;
		found = true;
	}
}

template <class Comparable>
const Comparable & BinarySearchTree<Comparable>::findMin() const
{
	return elementAt(findMin(root));
}

template <class Comparable>
BinaryNode<Comparable> *BinarySearchTree<Comparable>::findMin(BinaryNode<Comparable> *t) const
{
	if (t == NULL)		return NULL;

	if (t->left == NULL)
		return t;
	return findMin(t->left);
}

template <class Comparable>
void BinarySearchTree<Comparable>::printTreeInOrder(ofstream & file) const
{
	if (isEmpty()) cout << "Empty tree" << endl;
	else
	{
		printTreeInOrder(root, file);
	}
}

template <class Comparable>
void BinarySearchTree<Comparable>::printTreePreOrder(ofstream & file) const
{
	if (isEmpty()) cout << "Empty tree" << endl;
	else
	{
		printTreePreOrder(root, file);
	}
}

template <class Comparable>
void BinarySearchTree<Comparable>::printTreeInOrder(BinaryNode<Comparable> * t, ofstream & file) const
{
	if (t != NULL)
	{
		printTreeInOrder(t->left, file);
		file << t->element << endl;
		printTreeInOrder(t->right, file);
	}
}

template <class Comparable>
void BinarySearchTree<Comparable>::printTreePreOrder(BinaryNode<Comparable> * t, ofstream & file) const
{
	if (t != NULL)
	{
		file << t->element << endl;
		printTreePreOrder(t->left, file);
		printTreePreOrder(t->right, file);
	}
}

template <class Comparable>
void BinarySearchTree<Comparable>:: Draw(ofstream & file)
{
	if (isEmpty()) cout << "Empty tree" << endl;
	else
	{
		DrawHelper(root, file, "__", "left", "");
	}
}

template <class Comparable>
void BinarySearchTree<Comparable>::DrawHelper(BinaryNode<Comparable> * t, ofstream & file, string s, string par, string b)
{
	string parent, elem;
	if (t != NULL)
	{
		file << b;
		file << "|";
		file << s;
		if (s == "--") parent = "left";
		else parent = "right";
		if (par == "left") b += "   "; 
		else b += "|  "; // adding | if parent is right
		if (par == "right" && s == "__")
		{
			b = b.substr(0, b.length() - 3); //deleting | if parent and itself is right
			b += "   ";
		}
		elem = t->element;
		Parse(elem);
		file << elem << endl;
		//file << name << " " << surname << endl;
		DrawHelper(t->left, file, "--", parent, b); //putting -- if left child
		DrawHelper(t->right, file, "__", parent, b); //putting __ if left child
	}
}

template <class Comparable>
bool BinarySearchTree<Comparable>::isEmpty() const
{
	return (root == nullptr);
}

template <class Comparable>
BinaryNode<Comparable> * BinarySearchTree<Comparable>::GetRoot() const
{
	return root;
}

template <class Comparable>
class AvlTree;

template <class Comparable>
class AvlNode //templated node to be used in AVL Tree class
{
public:
	Comparable 	element;
	AvlNode *left;
	AvlNode *right;
	int height;
	AvlNode(const Comparable & theElement, AvlNode *lt, AvlNode *rt, int h = 0)
		: element(theElement), left(lt), right(rt), height(h) { }
};

template <class Comparable>
class AvlTree //templated AVL Tree class
{
public:
	explicit AvlTree(const Comparable & notFound);
	const Comparable & findMin() const;
	void find(const Comparable & x) const;
	bool isEmpty() const;
	void insert(const Comparable & x);
	void remove(const Comparable & x);
	AvlNode<Comparable> * GetRoot() const;
	void printTreePreOrder(ofstream &) const;
	void printTreeInOrder(ofstream &) const;
	void Draw(ofstream &);
	Comparable ITEM_NOT_FOUND; //moved from private to public
private:
	AvlNode<Comparable> *root;
	const Comparable & elementAt(AvlNode<Comparable> *t) const;
	void insert(const Comparable & x, AvlNode<Comparable> * & t) const;
	void remove(const Comparable & x, AvlNode<Comparable> * & t);
	AvlNode<Comparable> * findMin(AvlNode<Comparable> *t) const;
	vector<AvlNode<Comparable>*> find(const Comparable & x, AvlNode<Comparable> *t, vector<AvlNode<Comparable>*> &) const;
	void printTreePreOrder(AvlNode<Comparable> *t, ofstream &) const;
	void printTreeInOrder(AvlNode<Comparable> *t, ofstream &) const;
	void DrawHelper(AvlNode<Comparable> * t, ofstream &, string, string, string);
	void balance(AvlNode<Comparable> *&t);
	int height(AvlNode<Comparable> *t) const; //AVL manipulations
	int max(int lhs, int rhs) const;
	void rotateWithLeftChild(AvlNode<Comparable> * & k2) const;
	void rotateWithRightChild(AvlNode<Comparable> * & k1) const;
	void doubleWithLeftChild(AvlNode<Comparable> * & k3) const;
	void doubleWithRightChild(AvlNode<Comparable> * & k1) const;
};

template <class Comparable>
AvlTree<Comparable>::AvlTree(const Comparable & notFound) : ITEM_NOT_FOUND(notFound), root(NULL) { } //constructor

template <class Comparable>
bool AvlTree<Comparable>::isEmpty() const
{
	return (root == nullptr);
}

template <class Comparable>
const Comparable & AvlTree<Comparable>::findMin() const
{
	return elementAt(findMin(root));
}

template <class Comparable>
AvlNode<Comparable> *AvlTree<Comparable>::findMin(AvlNode<Comparable> *t) const
{
	if (t == NULL)		return NULL;

	if (t->left == NULL)
		return t;
	return findMin(t->left);
}

template <class Comparable>
void AvlTree<Comparable>::printTreePreOrder(ofstream & file) const
{
	if (isEmpty()) cout << "Empty tree" << endl;
	else
	{
		printTreePreOrder(root, file);
	}
}

template <class Comparable>
void AvlTree<Comparable>::printTreeInOrder(ofstream & file) const
{
	if (isEmpty()) cout << "Empty tree" << endl;
	else
	{
		printTreePreOrder(root, file);
	}
}

template <class Comparable>
void AvlTree<Comparable>::printTreePreOrder(AvlNode<Comparable> * t, ofstream & file) const
{
	if (t != NULL)
	{
		file << t->element << endl;
		printTreePreOrder(t->left, file);
		printTreePreOrder(t->right, file);
	}
}

template <class Comparable>
void AvlTree<Comparable>::printTreeInOrder(AvlNode<Comparable> * t, ofstream & file) const
{
	if (t != NULL)
	{
		printTreeInOrder(t->left, file);
		file << t->element << endl;
		printTreeInOrder(t->right, file);
	}
}


template <class Comparable>
void AvlTree<Comparable>::Draw(ofstream & file)
{
	if (isEmpty()) cout << "Empty tree" << endl;
	else
	{
		DrawHelper(root, file, "__", "left", "");
	}
}

template <class Comparable>
void AvlTree<Comparable>::DrawHelper(AvlNode<Comparable> * t, ofstream & file, string s, string par, string b) //recursive draw function
{
	string parent, elem;
	if (t != NULL)
	{
		file << b;
		file << "|";
		file << s;
		if (s == "--") parent = "left";
		else parent = "right";
		if (par == "left") b += "   ";
		else b += "|  "; // putting | if parent is right
		if (par == "right" && s == "__")
		{
			b = b.substr(0, b.length() - 3); //deleting | if parent and itself is right
			b += "   ";
		}
		elem = t->element;
		Parse(elem);
		file << elem << endl;
		//file << name << " " << surname << endl;
		DrawHelper(t->left, file, "--", parent, b); //putting -- if left child
		DrawHelper(t->right, file, "__", parent, b); //putting __ if left child
	}
}

template <class Comparable>
vector<AvlNode<Comparable>*> AvlTree<Comparable>::find(const Comparable & x, AvlNode<Comparable> *t, vector<AvlNode<Comparable>*> & found) const
{
	string name, surname, elem;
	if (t)
	{
		elem = t->element;
		Parse(elem);
		elem = elem.substr(0, x.length());
	}
	if (t == NULL)
		return found;
	else if (x < elem)
	{
		find(x, t->left, found);
		return found;
	}
	else if (elem < x)
	{
		find(x, t->right, found);
		return found;
	}
	else
	{
		found.push_back(t);
		if (t)
		{
			find(x, t->left, found);
			find(x, t->right, found);
		}
		return found;
	}
}

template <class Comparable>
void AvlTree<Comparable>::find(const Comparable & x) const
{
	vector<AvlNode<Comparable>*> found;
	find(x, root, found);
	for (int i = 0; i < found.size(); i++)
	{
		cout << elementAt(found[i]) << endl;
	}
}

template <class Comparable>
const Comparable & AvlTree<Comparable>::elementAt(AvlNode<Comparable> *t) const
{
	return t == NULL ? ITEM_NOT_FOUND : t->element;
}

template <class Comparable>
int AvlTree<Comparable>::height(AvlNode<Comparable> *t) const
{
	if (t == NULL)
		return -1;
	return t->height;
}

template <class Comparable>
int AvlTree<Comparable>::max(int lhs, int rhs) const
{
	if (lhs > rhs)
		return lhs;
	return rhs;
}

template <class Comparable>
void AvlTree<Comparable>::insert(const Comparable & x, AvlNode<Comparable> * & t) const
{
	if (t == NULL)
		t = new AvlNode<Comparable>(x, NULL, NULL);
	else if (x < t->element)
	{
		// X should be inserted to the left tree!
		insert(x, t->left);
		// Check if the left tree is out of balance (left subtree grew in height!)
		if (height(t->left) - height(t->right) == 2)
			if (x < t->left->element)  // X was inserted to the left-left subtree!
				rotateWithLeftChild(t);
			else			     // X was inserted to the left-right subtree!
				doubleWithLeftChild(t);
	}
	else if (t->element < x)
	{    // Otherwise X is inserted to the right subtree
		insert(x, t->right);
		if (height(t->right) - height(t->left) == 2)
			// height of the right subtree increased
			if (t->right->element < x)
				// X was inserted to right-right subtree
				rotateWithRightChild(t);
			else // X was inserted to right-left subtree
				doubleWithRightChild(t);
	}
	else
		cout << "duplicate AVL" << endl;
		;  // Duplicate; do nothing
	t->height = max(height(t->left), height(t->right)) + 1; // update the height the node
}

template <class Comparable>
void AvlTree<Comparable>::insert(const Comparable & x)
{
	insert(x, root);
}

template <class Comparable>
void AvlTree<Comparable>::remove(const Comparable & x)
{
	remove(x, root);
}

//taken from the book
template <class Comparable>
void AvlTree<Comparable>::remove(const Comparable & x, AvlNode<Comparable> * & t)
{
	string name, surname, elem;
	if (t)
	{
		elem = t->element;
		Parse(elem);
	}
	if (t == nullptr)
		return; // Item not found; do nothing

	if (x < elem)
		remove(x, t->left);
	else if (elem < x)
		remove(x, t->right);
	else if (t->left != nullptr && t->right != nullptr) // Two children
	{
		t->element = findMin(t->right)->element;
		elem = t->element;
		Parse(elem);
		//istringstream iss(t->element);
		//iss >> name >> surname;
		//elem = name + " " + surname;
		remove(elem, t->right);
	}
	else
	{
		AvlNode<Comparable> *oldNode = t;
		t = (t->left != nullptr) ? t->left : t->right;
		delete oldNode;
	}
	balance(t);
}

//taken from the book
static const int ALLOWED_IMBALANCE = 1;
template <class Comparable>
void AvlTree<Comparable>::balance(AvlNode<Comparable> *&t)
{
	if (t == nullptr)
		return;
	if (height(t->left) - height(t->right) > ALLOWED_IMBALANCE)
		if (height(t->left->left) >= height(t->left->right))
			rotateWithLeftChild(t);
		else
			doubleWithLeftChild(t);
	else
		if (height(t->right) - height(t->left) > ALLOWED_IMBALANCE)
			if (height(t->right->right) >= height(t->right->left))
				rotateWithRightChild(t);
			else
				doubleWithRightChild(t);
	t->height = max(height(t->left), height(t->right)) + 1;
}


template <class Comparable>
AvlNode<Comparable> * AvlTree<Comparable>::GetRoot() const
{
	return root;
}

template <class Comparable>
void AvlTree<Comparable>::rotateWithLeftChild(AvlNode<Comparable> * & k2) const
{
	AvlNode<Comparable> *k1 = k2->left;
	k2->left = k1->right;
	k1->right = k2;
	k2->height = max(height(k2->left), height(k2->right)) + 1;
	k1->height = max(height(k1->left), k2->height) + 1;
	k2 = k1;
}

template <class Comparable>
void AvlTree<Comparable>::rotateWithRightChild(AvlNode<Comparable> * & k1) const
{
	AvlNode<Comparable> *k2 = k1->right;
	k1->right = k2->left;
	k2->left = k1;
	k1->height = max(height(k1->left), height(k1->right)) + 1;
	k2->height = max(height(k2->right), k1->height) + 1;
	k1 = k2;
}

template <class Comparable>
void AvlTree<Comparable>::doubleWithLeftChild(AvlNode<Comparable> * & k3) const
{
	rotateWithRightChild(k3->left);
	rotateWithLeftChild(k3);
}

template <class Comparable>
void AvlTree<Comparable>::doubleWithRightChild(AvlNode<Comparable> * & k1) const
{
	rotateWithLeftChild(k1->right);
	rotateWithRightChild(k1);
}

BinarySearchTree<string> BST(" "); //BST to be used as phonebook, declared as global variable
AvlTree<string> AVL(" "); //AVL Tree to be used as phonebook, declared as global variable

//function that makes the string uppercase
void UpperCase(string & str)
{
	for (unsigned int i = 0; i < str.length(); i++) // checking every character of the string
	{
		if ('a' <= str.at(i) && str.at(i) <= 'z') // if the letter is lowercase
		{
			str.at(i) = str.at(i) + ('A' - 'a'); // make the character at that index uppercase
		}
	}
}

//function for searching contacts
void SearchContact()
{
	cout << "Searching an item in the phonebook (BST) ..." << endl;
	cout << "====================" << endl;	
	string contact;
	cout << "Search for a contact: ";
	//cin >> contact;
	cin.ignore(); //does not work in my computer unless I put ignore before getline
	getline(cin, contact);
	UpperCase(contact);
	cout << "Phonebook : Searching for: (" << contact << ")" << endl;
	cout << "====================" << endl;
	BST.found = false;
	auto BSTstarted = std::chrono::high_resolution_clock::now(); //start keeping the time
	BST.find(contact);
	auto BSTfinished = std::chrono::high_resolution_clock::now(); //insertion finished

	if (!BST.found)
	{
		cout << "Not found" << endl;
		cout << endl;
	}

	cout << "Searching an item in the phonebook (AVL) ..." << endl;
	cout << "Phone book: Section (" << contact << ")" << endl;

	auto AVLstarted = std::chrono::high_resolution_clock::now(); //start keeping the time
	AVL.find(contact);
	auto AVLfinished = std::chrono::high_resolution_clock::now(); //insertion finished

	if (!BST.found)
	{
		cout << "Not found" << endl;
		cout << endl;
	}

	BST.found = false;

	cout << "The search in BST took ";
	cout << chrono::duration_cast<std::chrono::nanoseconds>(BSTfinished - BSTstarted).count(); //display the durations
	cout << " nanoseconds..." << endl;
	cout << endl;
	cout << "The search in AVL took ";
	cout << chrono::duration_cast<std::chrono::nanoseconds>(AVLfinished - AVLstarted).count();
	cout << " nanoseconds..." << endl;
	cout << endl;
}

//function for adding a contact
void AddContact()
{
	string name, tel, city, key;
	cout << "Adding an item to to the phonebook (BST) ..." << endl;
	cout << "====================" << endl;
	cout << "Enter the information of the contact to be added:" << endl;
	cout << "Name: ";
	cin.ignore(); //does not work in my computer unless I put ignore before getline
	getline(cin, name);
	//cin >> name;
	UpperCase(name);
	cout << endl;
	cout << "Tel: ";
	cin >> tel;
	cout << endl;
	cout << "City: ";
	cin >> city;
	cout << endl;
	key = name + " " + tel + " " + city; //turning the inputs into the key
	BST.found = false;

	auto BSTstarted = std::chrono::high_resolution_clock::now();
	BST.insert(key);
	auto BSTfinished = std::chrono::high_resolution_clock::now();

	if (!BST.found)
	{
		cout << "Contact has been added successfully to the BST" << endl;
		cout << "Adding an item to to the phonebook (AVL) ..." << endl;
		cout << "====================" << endl;

		auto AVLstarted = std::chrono::high_resolution_clock::now();
		AVL.insert(key);
		auto AVLfinished = std::chrono::high_resolution_clock::now();

		cout << "Contact has been added successfully to the AVL tree" << endl;
		cout << endl;
		cout << "Adding a contact to the Binary Tree took ";
		cout << chrono::duration_cast<std::chrono::nanoseconds>(BSTfinished - BSTstarted).count();
		cout << " nanoseconds" << endl;
		cout << "Adding a contact to the AVL Tree took ";
		cout << chrono::duration_cast<std::chrono::nanoseconds>(AVLfinished - AVLstarted).count();
		cout << " nanoseconds" << endl;
		cout << endl;
	}
	else //if that contact already exists
	{
		cout << "The given contact full name already exists in the database." << endl;
	}
	BST.found = false;
}

//function for deleting a contact
void DeleteContact()
{
	string contact;
	cout << "Deleting an item from the phonebook ..." << endl;
	cout << "====================" << endl;
	cout << "Enter the fullname of the contact to be deleted:" << endl;
	cin.ignore(); //does not work in my computer unless I put ignore before getline
	getline(cin, contact);
	//cin >> contact;
	UpperCase(contact);
	BST.found = false;

	auto BSTstarted = std::chrono::high_resolution_clock::now();
	BST.remove(contact);
	auto BSTfinished = std::chrono::high_resolution_clock::now();

	auto AVLstarted = std::chrono::high_resolution_clock::now();
	AVL.remove(contact);
	auto AVLfinished = std::chrono::high_resolution_clock::now();

	if (BST.found)	
		cout << "Deleted successfully..." << endl;
	else //if that contact does not exist
		cout << "Not found" << endl;
	BST.found = false;

	cout << "Deleting from Binary Tree took ";
	cout << chrono::duration_cast<std::chrono::nanoseconds>(BSTfinished - BSTstarted).count(); //displaying the durations
	cout << " nanoseconds..." << endl;
	cout << "Deleting from AVL Tree took ";
	cout << chrono::duration_cast<std::chrono::nanoseconds>(AVLfinished - AVLstarted).count();
	cout << " nanoseconds..." << endl;
}

//function for printing the phonebook into files, with in-order and pre-order traversal
void PrintPhonebook()
{
	//implemented according to the update in hw file page 4 on March 23, indicated 4 separate files for each tree for each traversal
	ofstream fileinbst, fileinavl, filepreavl, fileprebst; 
	string filenameinbst = "phonebookInOrderBST.txt"; //corresponding filenames
	string filenameinavl = "phonebookInOrderAVL.txt";
	string filenameprebst = "phonebookPreOrderBST.txt";
	string filenamepreavl = "phonebookPreOrderAVL.txt";

	cout << "Saving the phonebook to a file (In-Order) ..." << endl; //inorder traversal for both trees
	fileinbst.open(filenameinbst);
	BST.printTreeInOrder(fileinbst);
	fileinbst.close();

	fileinavl.open(filenameinavl);
	BST.printTreeInOrder(fileinavl);
	fileinavl.close();

	cout << "Saving the phonebook to a file (Pre-Order) ..." << endl; //preoder traversal for both trees
	filepreavl.open(filenamepreavl);
	AVL.printTreePreOrder(filepreavl);
	filepreavl.close();

	fileprebst.open(filenameprebst);
	BST.printTreePreOrder(fileprebst); 
	fileprebst.close();
}

//function that draws the phonebooks as trees into corresponding files
void DrawPhonebook()
{
	ofstream fileBST, fileAVL;
	string filenameBST = "phonebookTreeBST.txt"; //corresponding file names
	string filenameAVL = "phonebookTreeAVL.txt";

	fileBST.open(filenameBST);
	cout << "Printing out the phonebook (BST)" << endl;
	cout << "====================" << endl;
	BST.Draw(fileBST); //drawing the BST
	fileBST.close();

	fileAVL.open(filenameAVL);
	cout << "Printing out the phonebook (AVL)" << endl;
	cout << "====================" << endl;
	AVL.Draw(fileAVL); //drawing the AVL
	fileAVL.close();
}

//function that displays the main menu functions
void MainMenu()
{
	while (true)
	{
		int option;
		cout << "Choose which action to perform from 1 to 7:" << endl
			<< "1 - Search a phonebook contact" << endl
			<< "2 - Adding a phonebook contact" << endl
			<< "3 - Deleting a phonebook contact" << endl
			<< "4 - Print the phonebook to a file(inorder)\n    Print the phonebook to a file(postorder)" << endl
			<< "5 - Draw the Phonebook as a Tree to a file" << endl
			<< "6 - Press 6 to exit" << endl;
		cout << endl;
		cin >> option;
		switch (option) //calling the corresponding functions for each option
		{
		case 1:
			SearchContact();
			break;
		case 2:
			AddContact();
			break;
		case 3:
			DeleteContact();
			break;
		case 4:
			PrintPhonebook();
			break;
		case 5:
			DrawPhonebook();
			break;
		case 6:
			cout << "Exiting..." << endl;
			exit(0);
		default:
			cout << "Invalid Option! Try again!" << endl;
		}
	}
}

//recursive function to calculate the height of the BST
int CalculateHeightHelperBST(BinaryNode<string> * node)
{
	if (!node) //if the tree is empty
		return 0;
	else
	{
		int lHeight = CalculateHeightHelperBST(node->left); //left subtree
		int rHeight = CalculateHeightHelperBST(node->right); //right subtree
		if (lHeight > rHeight) //get the one which is higher
			return (lHeight + 1);
		else
			return (rHeight + 1);
	}
}

//recursive function to calculate the height of the AVL
int CalculateHeightHelperAVL(AvlNode<string> * node) //same idea with BST
{
	if (!node)
		return 0;
	else
	{
		int lHeight = CalculateHeightHelperAVL(node->left);
		int rHeight = CalculateHeightHelperAVL(node->right);
		if (lHeight > rHeight)
			return(lHeight + 1);
		else
			return(rHeight + 1);
	}
}

int CalculateHeightBST(BinaryNode<string> * root)
{
	return CalculateHeightHelperBST(root); //calling the recursive function
}

int CalculateHeightAVL(AvlNode<string> * root)
{
	return CalculateHeightHelperAVL(root); //calling the recursive function
}

//function that creates the BST phonebook
void CreateBST(ifstream & file)
{
	string line, name, surname, tel, city, key;
	cout << "Loading the phonebook into a BST" << endl;
	cout << endl;
	auto started = std::chrono::high_resolution_clock::now(); //start keeping the time
	while (getline(file, line)) //read file line by line
	{
		istringstream iss(line);
		iss >> name >> surname >> tel >> city; //get each word
		UpperCase(name); // turn the name and surname into uppercase
		UpperCase(surname);
		key = name + " " + surname + " " + tel + " " + city; //make the key
		BST.insert(key); //insert the key
	}
	auto finished = std::chrono::high_resolution_clock::now(); //creating has finished
	cout << "Phonebook creation in BST took ";
	cout << chrono::duration_cast<std::chrono::nanoseconds>(finished - started).count(); //display the duration
	cout << " nanoseconds" << endl;
	cout << endl;

	int leftheight = CalculateHeightBST((BST.GetRoot())->left); //calculate the heights of the left and right subtrees
	int rightheight = CalculateHeightBST((BST.GetRoot())->right);

	if (abs(leftheight - rightheight) <= 1) //check the height difference between left and right subtrees
	{
		cout << "The tree is balanced" << endl;
	}
	else
	{
		cout << "The tree is not balanced" << endl;
	}
	cout << endl;
	cout << "The heights of BST are for left: " << leftheight << " and right: "<< rightheight << endl;
}

//function that creates the AVL tree phonebook
void CreateAVL(ifstream & file)
{
	string line, name, surname, tel, city, key; //similiar to creating the BST
	cout << "Loading the phonebook into an AVL" << endl;
	cout << endl;
	auto started = std::chrono::high_resolution_clock::now();
	while (getline(file, line))
	{
		istringstream iss(line);
		iss >> name >> surname >> tel >> city;
		UpperCase(name);
		UpperCase(surname);
		key = name + " " + surname + " " + tel + " " + city; //make the key
		AVL.insert(key); //insert the key
	}
	auto finished = std::chrono::high_resolution_clock::now();
	cout << "Phonebook creation in AVL took ";
	cout << chrono::duration_cast<std::chrono::nanoseconds>(finished - started).count();
	cout << " nanoseconds" << endl;
	cout << endl;

	int leftheight = CalculateHeightAVL((AVL.GetRoot())->left); //calculate the heights of the left and right subtrees
	int rightheight = CalculateHeightAVL((AVL.GetRoot())->right);

	if (abs(leftheight - rightheight) <= 1) //check the height difference between left and right subtrees
	{
		cout << "The tree is balanced" << endl;
	}
	else
	{
		cout << "The tree is not balanced" << endl;
	}
	cout << endl;
	cout << "The heights of AVL are for left: " << leftheight << " and right: " << rightheight << endl;
	cout << endl;
}

//function that reads the input file and calls the functions to create the trees
void ReadFile()
{
	ifstream file;
	string filename;
	cout << "Please enter the contact file name: " << endl;
	cin >> filename;
	file.open(filename.c_str());
	while (file.fail()) //keep asking for the file until a valid one is entered
	{
		cout << "Couldn't open file. Please enter again: ";
		cin >> filename;
		file.open(filename.c_str());
	}
	file.clear();
	file.seekg(0);
	CreateBST(file); //create the Binary Search Tree
	file.clear();
	file.seekg(0);
	CreateAVL(file); //create the AVL tree
	file.close();
}

int main() //main part of the program
{
	ReadFile(); //reads the input file
	MainMenu(); //diplays the main menu for the functions
	return 0;
}