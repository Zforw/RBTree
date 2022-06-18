#ifndef BINARYTREE_BINTREENODE_H
#define BINARYTREE_BINTREENODE_H
template <class T>
class BinTreeNode {
public:
	T data;
	char property;
	int _size, dir;//dir = 0 表示是左孩子
	BinTreeNode<T>  *parent, *lChild, *rChild;
	BinTreeNode() {
		parent = lChild = rChild = nullptr;
		_size = 1;
		dir = 0;
	}
	BinTreeNode(T x, BinTreeNode<T> *p = nullptr, BinTreeNode<T> *l = nullptr, BinTreeNode<T> *r = nullptr, bool d = 0) {
		data = x;
		dir = d;
		if(p) {
			parent = p;
			if(d == 0) p->lChild = this;
			else p->rChild = this;
		}
		lChild = l;
		rChild = r;
		_size = 1;
		if(l != nullptr) _size += l->size();
		if(r != nullptr) _size += r->size();
	}
	int size() {
		int ans = 1;
		if(this->lChild != nullptr) ans += lChild->size();
		if(this->rChild != nullptr) ans += rChild->size();
		_size = ans;
		return ans;
	}
};
#endif //BINARYTREE_BINTREENODE_H
