#pragma once
template <typename T> class LinkedList
{
	
public:
	struct Node
	{
		T item;
		Node* parent;
		Node* left;
		Node* right;
		Node(T val) : item(val), left(nullptr), right(nullptr), parent(nullptr)
		{}
	};
	LinkedList() : root(nullptr)
	{

	}
	~LinkedList()
	{

	}


	void insert(T item);

private:
	void insert(T item, Node* node);

	Node* root;
};

template <typename T>
void LinkedList<T>::insert(T item)
{
	if (root != nullptr)
	{
		insert(item, root);
	}
	else
	{
		root = new Node;
		root->item = item;
		root->left = nullptr;
		root->right = nullptr;

	}
}

template <typename T>
void LinkedList<T>::insert(T item, Node* node)
{

	if (item < node->item)
	{
		if (node->left != nullptr)
		{
			insert(item, node->left);
		}
		else
		{
			node->left = new Node(node);

		}
	}
	else if (item > node->item)
	{
		if (node->right != nullptr)
		{
			insert(item, node->right);
		}
		else
		{
			node->right = new Node(node);

		}
	}
	
	/*Node* temp = new Node(item);
	Node* parent;
	Node* current;
	current = root;

	while (current)
	{
		parent = current;
		if
		if (temp->item > current->item)
		{
			current = current->right;
		}
		else
		{
			current = current->left;
		}
	}
	if (temp->item < parent->item)
	{
		parent->left = temp;
	}
	else
	{
		parent->right = temp;
	}
*/
}