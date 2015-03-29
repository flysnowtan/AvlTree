#include <algorithm>
#include <set>
#include <iostream>

using namespace std;

template<typename T>
class AvlTree;

template<typename T>
class AvlNode{
	friend class AvlTree < T >;
	T data;
	int height;
	AvlNode *left;
	AvlNode *right;
	AvlNode(T _data) :data(_data), height(0), left(NULL), right(NULL){}
};


template<typename T>
class AvlTree{
	AvlNode<T> *root;
public:
	AvlTree() :root(NULL){}
	int Height(AvlNode<T> *node){ return node == NULL ? -1 : node->height; }
	void Insert(T _data){ _insert(root, _data); }
	void Delete(T _data){ _delete(root, _data); }
	bool Find(T _data);
	void print(){ travel(root); cout << endl; }
private:
	AvlTree(const AvlTree &);
	AvlTree& operator=(const AvlTree&);
	void LL(AvlNode<T>* &node);
	void RR(AvlNode<T>* &node);
	void RL(AvlNode<T>* &node);
	void LR(AvlNode<T>* &node);
	void _insert(AvlNode<T>* &node, T _data);
	void _delete(AvlNode<T>* &node, T _data);
	void travel(AvlNode<T> *node);
	AvlNode<T> *FindMin(AvlNode<T> *node);
	AvlNode<T> *FindParent(AvlNode<T> *node, T _data);
	void ParentRotate(AvlNode<T>* &node, AvlNode<T>* &parent);
	void LeftHigherThanRight(AvlNode<T>* &node);
	void RightHigherThanLeft(AvlNode<T>* &node);
};

template<typename T>
void AvlTree<T>::LeftHigherThanRight(AvlNode<T>* &node)
{
	if (Height(node->left) - Height(node->right) == 2){
		if (node->left){
			if (Height(node->left->right) > Height(node->left->left))
				LR(node);
			else
				LL(node);
		}
	}
}

template<typename T>
void AvlTree<T>::RightHigherThanLeft(AvlNode<T>* &node)
{//调用次数多，封装成函数
	if (Height(node->right) - Height(node->left) == 2){
		if (node->right){
			if (Height(node->right->left)>Height(node->right->right))
				RL(node);
			else
				RR(node);
		}
	}
}

template<typename T>
void AvlTree<T>::ParentRotate(AvlNode<T>* &node, AvlNode<T>* &parent)
{//旋转从node到parent路径上的每一个结点，使之平衡
	if (parent == NULL)
		return;

	if (node->data > parent->data){
		ParentRotate(node->left, parent);
	}
	else if (node->data<parent->data){
		ParentRotate(node->right, parent);
	}

	LeftHigherThanRight(node);
	RightHigherThanLeft(node);

}

template<typename T>
AvlNode<T> *AvlTree<T>::FindParent(AvlNode<T> *node, T _data)
{//找父亲结点
	if (node == NULL || node->data == _data)
		return NULL;
	if ((node->left && node->left->data == _data) || (node->right&&node->right->data == _data))
		return node;
	else if (node->data>_data)
		return FindParent(node->left, _data);
	else if (node->data<_data)
		return FindParent(node->right, _data);
}

template<typename T>
AvlNode<T> *AvlTree<T>::FindMin(AvlNode<T> *node)
{//找最小结点
	if (node){
		while (node->left)
			node = node->left;
	}
	return node;
}

template<typename T>
void AvlTree<T>::_delete(AvlNode<T>* &node, T _data)
{
	if (node == NULL){
		//	cout << "the element is not exit" << endl;
		return;
	}
	else {
		if (_data < node->data){
			_delete(node->left, _data);
			RightHigherThanLeft(node);//右子树高于左子树，旋转
		}
		else if (_data > node->data){
			_delete(node->right, _data);
			LeftHigherThanRight(node);//左子树高于右子树，旋转
		}
		else {//定位到要删除的结点
			AvlNode<T> *temp = FindMin(node->right);
			if (temp == NULL){//node没有右结点
				temp = node;
				node = node->left;
				delete temp;
			}
			else{
				if (temp == node->right){//node右结点没有左子树
					node->right = temp->right;
					node->data = temp->data;
					delete temp;
					LeftHigherThanRight(node);//删除后，可能左子树高于右子树
				}
				else{//node右结点有左子树
					AvlNode<T> *parent = FindParent(node, temp->data);//寻找父节点
					node->data = temp->data;
					parent->left = temp->right;
					delete temp;
					//parent may not balence
					ParentRotate(node, parent);//使得node到parent路径上的每一个结点能够平衡
				}
			}
		}
	}
	if (node != NULL)
		node->height = std::max(Height(node->left), Height(node->right)) + 1;//更新高度
}

template<typename T>
void AvlTree<T>::_insert(AvlNode<T>* &node, T _data)
{
	if (node == NULL){
		node = new AvlNode<T>(_data);//找到插入点
	}
	else {
		if (_data < node->data){
			_insert(node->left, _data);//插入到左子树后，判断其高度是否平衡
			if (Height(node->left) - Height(node->right) == 2){
				if (_data > node->left->data)
					LR(node);
				else
					LL(node);
			}
		}
		else if (_data > node->data){//插入到右子树后，判断其高度是否平衡
			_insert(node->right, _data);
			if (Height(node->right) - Height(node->left) == 2){
				if (_data<node->right->data)
					RL(node);
				else
					RR(node);
			}
		}
		else {
			//	cout << "the element has exit" << endl;
			return;
		}

	}

	node->height = std::max(Height(node->left), Height(node->right)) + 1;//更新高度
}

template<typename T>
void AvlTree<T>::LL(AvlNode<T>* &node)
{
	AvlNode<T> *temp = node->left;
	node->left = temp->right;
	temp->right = node;

	node->height = std::max(Height(node->left), Height(node->right)) + 1;
	temp->height = std::max(Height(temp->left), Height(temp->right)) + 1;

	node = temp;

}

template<typename T>
void AvlTree<T>::RR(AvlNode<T>* &node)
{
	AvlNode<T> *temp = node->right;
	node->right = temp->left;
	temp->left = node;

	node->height = std::max(Height(node->left), Height(node->right)) + 1;
	temp->height = std::max(Height(temp->left), Height(temp->right)) + 1;

	node = temp;
}

template<typename T>
void AvlTree<T>::LR(AvlNode<T>* &node)
{
	RR(node->left);
	LL(node);
}

template<typename T>
void AvlTree<T>::RL(AvlNode<T>* &node)
{
	LL(node->right);
	RR(node);
}

template<typename T>
void AvlTree<T>::travel(AvlNode<T> *node)
{
	if (node){
		travel(node->left);
		cout << node->data << " ";
		travel(node->right);
	}
}
