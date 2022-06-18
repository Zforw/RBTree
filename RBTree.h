/*
*  God, Grant me
*  the SERENITY, to accept the things I cannot change,
*  COURAGE to change the things I can, and
*  the WISDOM to know the difference.
*  Talk is cheap.Show me the code.
*  Created by 赵丰 on 2022/6/17.
   Copyright (c) 2022 IBM Inc.All rights reserved.
*/
#ifndef RBTREE_RBTREE_H
#define RBTREE_RBTREE_H

/*
红黑树
是一种二叉搜索树，是AVL树的改进版。
红黑树的生长是自底向上的，跟2-3树类似
为啥引入红黑树：
1、二插搜索树会出现退化成链表的情况，其时间复杂度为O(n)
2、为防止二叉搜索树会退化成链表，引入了对左右子树的高度差有严格显示的AVL树。
	AVL树的缺点：
	因为对左右子树的高度差有严格的规定，所以其在插入和删除时会出现旋转，导致性能下降。
这才有了对平衡概念不是很严格的红黑树
红黑树与AVL树的比较
1. AVL树的时间复杂度优于红黑树，但是对于现在的计算机，这种差别可以忽略
2. 红黑树的插入删除比AVL树更便于控制操作。
3. 红黑树整体性能略优于AVL树。（红黑树旋转情况少于AVL树）。这点是非常重要的
4. 如果是在查询很多增删少的情况下 AVL 树还是优于红黑树的，如果增删比较频繁，那红黑树绝对是完美的一种选择
红黑树的特性：
1. 根节点是【黑色】
2. 每个节点是【黑色】或者是【红色】
3. 【红色】节点的两个子节点一定都是【黑色】
4. 每个叶子节点（NIL）都是【黑色】
5. 任意一个节点到叶子节点的路径上所包含的【黑色】节点的数量是相同的---这个也称之为【黑色完美平衡】
*/

#include <iomanip>
#include <iostream>
#include "BinaryTree.h"
using std::string;
enum RBColor {
	RED, BLACK
};

struct Node {
	RBColor color;
	Node *pLeft;
	Node *pRight;
	Node *pParent;//会出现找父节点变色或者旋转的情况
	int data;

	Node (RBColor c, Node *left, Node *right, Node *parent, int data) :
			color (c), pLeft (left), pRight (right), pParent (parent), data (data) {}
};


class RBTree {
public:
	RBTree () {
		pRoot = nullptr;
	}

	~RBTree () {
		Destroy ();
	}

	//增
	void Insert (int data);

	//销毁
	void Destroy ();

	string PreTravel ();
	string MidTravel ();
	string PreColor ();
	//打印
	void Print ();

	void PrintTree();

	Node* Search(int key) {
		return iterativeSearch(pRoot, key);
	}
	void remove(int key);

private:
	Node* iterativeSearch(Node *x, int key) {
		while (x != nullptr) {
			if (x->data > key) {
				x = x->pLeft;
			} else if (x-> data < key) {
				x = x->pRight;
			} else {
				return x;
			}
		}
		return x;
	}
	void insertNode (Node *&pNode, int data);//插入
	void leftRotate (Node *&pNode);//左旋
	void rightRotate (Node *&pNode);//右旋
	void insertAdjust (Node *&pNode, Node *pNewNode);//修正为一颗红黑树
	void Print (Node *pNode, int data, int dirction);//data表示节点值， direction表示左节点（-1）还是右节点（1）

	void removeAdjust(Node* node, Node* parent);
	void remove(Node* node);

	//遍历

	string preOrder, midOrder, preColor;
	void midTravel (Node *pNode);//中序遍历
	void preTravel (Node *pNode);//前序遍历
	void preTravelColor (Node *pNode);//中序遍历

	void Destroy (Node *pNode);

	Node *pRoot;

	inline void SetBlack (Node *pNode) {
		pNode -> color = BLACK;
	}

	inline void SetRed (Node *pNode) {
		pNode -> color = RED;
	}
};


void RBTree::Insert (int data) {
	insertNode (pRoot, data);
}

string RBTree::PreColor () {
	preColor = "";
	preTravelColor (pRoot);
	return preColor;
}

string RBTree::PreTravel () {
	preOrder = "";
	preTravel (pRoot);
	return preOrder;
}

string RBTree::MidTravel () {
	midOrder = "";
	midTravel (pRoot);
	return midOrder;
}

void RBTree::Destroy () {
	Destroy (pRoot);
}

void RBTree::Print () {
	Print (pRoot, pRoot -> data, 0);
}

void RBTree::PrintTree () {
	BinaryTree<char> bt;
	bt.BuildTree (0, PreTravel(), MidTravel(), PreColor());
}

void RBTree::insertNode (Node *&pNode, int data) {
	//根节点 设置为黑
	if (pNode == nullptr) {
		pNode = new Node (BLACK, nullptr, nullptr, nullptr, data);
		return;
	}
	Node *pNodeParent = nullptr;
	Node *pTmp = pNode;

	//遍历查找，找到待插入位置的父节点处
	while (pTmp != nullptr) {
		pNodeParent = pTmp;
		if (pTmp -> data < data) {
			pTmp = pTmp -> pRight;
		} else {
			pTmp = pTmp -> pLeft;
		}
	}
	Node *pNew = new Node (RED, nullptr, nullptr, nullptr, data);
	pNew -> pParent = pNodeParent;
	if (pNodeParent != nullptr) {
		if (pNodeParent -> data > pNew -> data)
			pNodeParent -> pLeft = pNew;
		else
			pNodeParent -> pRight = pNew;
	}

	insertAdjust (pNode, pNew);
}

/*
旋转步骤：
1、pNode右孩子的左子树插入pNode的右子树上，如果pNodeRightChild的左子树不为null，则其父节点为pNode
2、pNode的父节点设为pNodeRightChild的父节点
3、判断pNode的父节点是否为null，为null则说明pNode为根节点，应将其右节点作为pRoot
不为null，则应判断pNode是左子树还是右子树，将pNodeRightChild作为相应的子树
4、pNode作为其右孩子的左子树，pNode的父节点指向pNodeRightChild
5、将pNodeRightChild设为当前节点pNode
*/

void RBTree::leftRotate (Node *&pNode) {
	Node *pNodeRightChild = pNode -> pRight;
	pNode -> pRight = pNodeRightChild -> pLeft;

	if (pNodeRightChild -> pLeft != nullptr) {
		pNodeRightChild -> pLeft -> pParent = pNode;
	}
	//pNode的父节点设为pNodeRightChild的父节点
	pNodeRightChild -> pParent = pNode -> pParent;

	//如果pNode的父节点为null，则将其右节点设为根节点
	if (pNode -> pParent == nullptr) {
		pRoot = pNodeRightChild;
	} else {
		if (pNode -> pParent -> pLeft == pNode) {
			pNode -> pParent -> pLeft = pNodeRightChild;
		} else {
			pNode -> pParent -> pRight = pNodeRightChild;
		}
		//pNodeRightChild -> pParent = pNode -> pParent;
	}
	//pNode作为pNodeRightChild的左孩子
	pNodeRightChild -> pLeft = pNode;
	//pNode的父节点为pNodeRightChild
	pNode -> pParent = pNodeRightChild;

	//pNodeRightChild作为pNode
	pNode = pNodeRightChild;
}

/*
旋转步骤：
1、pNode左孩子的右子树插入pNode的左子树上，如果pNode左子树的右子树不为null，则该右子树的父亲为pNode
2、pNode的父节点设为pNodeLeftChild的父节点
3、判断pNode的父节点是否为null，为null则说明pNode为根节点，应将其右节点作为pRoot
不为null，则应判断pNode是左子树还是右子树，将pNodeLeftChild作为相应的子树
4、pNode作为其左孩子的右子树，pNode的父节点指向pNodeLeftChild
5、将pNodeLeftChild设为当前节点pNode
*/

void RBTree::rightRotate (Node *&pNode) {
	//pNode左孩子的右子树插入pNode的左子树上
	Node *pNodeLeftChild = pNode -> pLeft;
	pNode -> pLeft = pNodeLeftChild -> pRight;
	//如果pNode左子树的右子树不为null，则该右子树的父亲为pNode
	if (pNodeLeftChild -> pRight != nullptr) {
		pNodeLeftChild -> pRight -> pParent = pNode;
	}

	//pNodeLeftChild的父亲设为pNode的父亲
	pNodeLeftChild -> pParent = pNode -> pParent;

	//如果pNode为根节点，则pNodeLeftChild设为根节点
	if (pNode -> pParent == nullptr)
		pRoot = pNodeLeftChild;
	else//根据pNode是其父节点的左or右孩子，将pNodeLeftChild设为相应的孩子
	{
		if (pNode -> pParent -> pLeft == pNode)
			pNode -> pParent -> pLeft = pNodeLeftChild;
		else
			pNode -> pParent -> pRight = pNodeLeftChild;
		//pNodeLeftChild -> pParent = pNode -> pParent;
	}
	//pNode作为其左孩子的右子树
	pNodeLeftChild -> pRight = pNode;
	//pNode的父节点为pNodeLeftChild
	pNode -> pParent = pNodeLeftChild;

	//pNodeLeftChild作为pNode
	pNode = pNodeLeftChild;
}

/*
调整平衡分为以下几种情况
1、红黑树为null，则新插入节点作为根节点，变色为黑色
2、插入节点的父节点为黑色，直接插入即可
3、插入节点的父节点为红色
	3.1插入节点的父节点为插入节点的爷爷节点的左孩子
		3.1.1插入节点的叔叔节点存在，且为红色
			 将新插入节点的父节点和其叔叔节点设置为黑色即可,爷爷节点设置为红色，将爷爷节点设置为当前节点
		3.1.2 叔叔节点不存在（其实叔叔节点不可能为黑色，因为在新插入节点之前，父节点是红色，破坏了黑色平衡，故叔叔节点一定是不存在的）
		（1）插入节点为其父节点的左孩子
				将父节点设置为黑色，爷爷节点设置为红色，对其爷爷节点进行右旋
		（2）插入节点为其父节点的右孩子
			先对父节点进行左旋，将插入节点设置为黑色，爷爷节点设置为红色，然后对其爷爷节点进行右旋
	3.2插入节点的父节点为插入节点的爷爷节点的右孩子
		3.2.1 插入节点的叔叔节点存在，且为红色
			将新插入节点的父节点和其叔叔节点设置为黑色即可,爷爷节点设置为红色，将爷爷节点设置为当前节点
		3.2.2 叔叔节点不存在（其实叔叔节点不可能为黑色，因为在新插入节点之前，父节点是红色，破坏了黑色平衡，故叔叔节点一定是不存在的）
		（1）插入节点为其父节点的右孩子
			将父节点设置为黑色，爷爷节点设置为红色，对其爷爷节点进行左旋
		（2）插入节点为其父节点的左孩子
			先对其父节点进行右旋，将插入节点设置为黑色，爷爷节点设置为红色，然后对其爷爷节点进行左旋
*/

void RBTree::insertAdjust (Node *&pNode, Node *pNewNode) {
	Node *pParent, *pGParent, *pUncle;//父节点, 爷爷节点, 叔叔节点（父节点的兄弟）

	pParent = pNewNode -> pParent;

	//只有当第三种情况发生时才会发生红黑树的调整
	while (pParent != nullptr && pParent -> color == RED) {
		pGParent = pParent -> pParent;
		//插入节点的父节点为插入节点的爷爷节点的左孩子
		if (pParent == pGParent -> pLeft) {
			pUncle = pGParent -> pRight;
			//叔叔节点存在且为红色
			//将新插入节点的父节点和其叔叔节点设置为黑色即可,将爷爷节点设置为当前节点
			//递归处理
			if (pUncle != nullptr && pUncle -> color == RED) {
				SetBlack (pUncle);
				SetBlack (pParent);
				SetRed (pGParent);
				pNewNode = pGParent;
				pParent = pNewNode->pParent;
				continue;
			}
			//叔叔节点不存在
			if (pUncle == nullptr) {
				//插入节点为其父节点的左孩子
				//将父节点设置为黑色，对其爷爷节点进行右旋
				if (pParent -> pLeft == pNewNode) {
					SetRed (pGParent);
					SetBlack (pParent);
					rightRotate (pGParent);
				} else {
					//插入节点为其父节点的右孩子
					//先对父节点进行左旋，将插入节点设置为黑色，爷爷节点设置为红色，然后对其爷爷节点进行右旋
					leftRotate (pParent);
					SetBlack (pParent);//此时插入节点指向pParent
					SetRed (pGParent);
					rightRotate (pGParent);
				}
			}
		} else {
			//插入节点的父节点为插入节点的爷爷节点的右孩子
			pUncle = pGParent -> pLeft;//叔叔节点为父节点的左孩子
			//叔叔节点存在且为红色
			//将新插入节点的父节点和其叔叔节点设置为黑色即可,将爷爷节点设置为当前节点
			//递归处理
			if (pUncle != nullptr && pUncle -> color == RED) {
				SetBlack (pUncle);
				SetBlack (pParent);
				SetRed (pGParent);
				pNewNode = pGParent;
				pParent = pNewNode->pParent;
				continue;
			}
			//叔叔节点不存在
			if (pUncle == nullptr) {
				//插入节点为其父节点的右孩子
				//将父节点设置为黑色，爷爷节点设置为红色，对其爷爷节点进行左旋
				if (pNewNode == pParent -> pRight) {
					SetBlack (pParent);
					SetRed (pGParent);
					leftRotate (pGParent);
				} else {
					//插入节点为其父节点的左孩子
					//先对其父节点进行右旋，将插入节点设置为黑色，爷爷节点设置为红色，然后对其爷爷节点进行左旋
					rightRotate (pParent);
					SetBlack (pParent);//此时插入节点指向pParent
					SetRed (pGParent);
					leftRotate (pGParent);
				}
			}
		}
	}

	//无根节点，新插入节点做根节点，设置为黑色
	SetBlack (pRoot);
}


/*
     * 红黑树删除修正函数
     *
     * 在从红黑树中删除插入节点之后(红黑树失去平衡)，再调用该函数；
     * 目的是将它重新塑造成一颗红黑树。
     *
     * 参数说明：
     *     node 待修正的节点
     */
void RBTree::removeAdjust(Node* node, Node* parent) {
	Node* other;

	while ((node == nullptr || node->color == BLACK) && (node != pRoot)) {
		if (parent->pLeft == node) {
			other = parent->pRight;
			if (other->color == RED) {
				// Case 1: x的兄弟w是红色的
				SetBlack(other);
				SetRed(parent);
				leftRotate(parent);
				other = parent->pRight;
			}

			if ((other->pLeft == nullptr || other->pLeft->color == BLACK) &&
				(other->pRight == nullptr || other->pRight->color == BLACK)) {
				// Case 2: x的兄弟w是黑色，且w的俩个孩子也都是黑色的
				SetRed(other);
				node = parent;
				parent = node->pParent;
			} else {
				if (other->pRight == nullptr || other->pRight->color == BLACK) {
					// Case 3: x的兄弟w是黑色的，并且w的左孩子是红色，右孩子为黑色。
					SetBlack(other->pLeft);
					SetRed(other);
					rightRotate(other);
					other = parent->pRight;
				}
				// Case 4: x的兄弟w是黑色的；并且w的右孩子是红色的，左孩子任意颜色。
				other->color = parent->color;
				SetBlack(parent);
				SetBlack(other->pRight);
				leftRotate(parent);
				node = pRoot;
				break;
			}
		} else {
			other = parent->pLeft;
			if (other->color == RED) {
				// Case 1: x的兄弟w是红色的
				SetBlack(other);
				SetRed(parent);
				rightRotate(parent);
				other = parent->pLeft;
			}

			if ((other->pLeft == nullptr || other->pLeft->color == BLACK) &&
				(other->pRight == nullptr || other->pRight->color == BLACK)) {
				// Case 2: x的兄弟w是黑色，且w的俩个孩子也都是黑色的
				SetRed(other);
				node = parent;
				parent = node->pParent;
			} else {
				if (other->pLeft == nullptr || other->pLeft->color == BLACK) {
					// Case 3: x的兄弟w是黑色的，并且w的左孩子是红色，右孩子为黑色。
					SetBlack(other->pRight);
					SetRed(other);
					leftRotate(other);
					other = parent->pLeft;
				}

				// Case 4: x的兄弟w是黑色的；并且w的右孩子是红色的，左孩子任意颜色。
				other->color = parent->color;
				SetBlack(parent);
				SetBlack(other->pLeft);
				rightRotate(parent);
				node = pRoot;
				break;
			}
		}
	}

	if (node != nullptr) {
		SetBlack(node);
	}
}

/*
 * 删除结点(node)，并返回被删除的结点
 *
 * 参数说明：
 *     node 删除的结点
 */
void RBTree::remove(Node* node) {
	Node *child, *parent;
	RBColor color;

	// 被删除节点的"左右孩子都不为空"的情况。
	if ((node->pLeft != nullptr) && (node->pRight != nullptr)) {
		// 被删节点的后继节点。(称为"取代节点")
		// 用它来取代"被删节点"的位置，然后再将"被删节点"去掉。
		Node* replace = node;

		// 获取后继节点
		replace = replace->pRight;
		while (replace->pLeft != nullptr) {
			replace = replace->pLeft;
		}

		// "node节点"不是根节点(只有根节点不存在父节点)
		if (node->pParent != nullptr) {
			if (node->pParent->pLeft == node) {
				node->pParent->pLeft = replace;
			} else {
				node->pParent->pRight = replace;
			}
		} else {
			// "node节点"是根节点，更新根节点。
			pRoot = replace;
		}

		// child是"取代节点"的右孩子，也是需要"调整的节点"。
		// "取代节点"肯定不存在左孩子！因为它是一个后继节点。
		child = replace->pRight;
		parent = replace->pParent;
		// 保存"取代节点"的颜色
		color = replace->pParent->color;

		// "被删除节点"是"它的后继节点的父节点"
		if (parent == node) {
			parent = replace;
		} else {
			// child不为空
			if (child != nullptr) {
				child->pParent = parent;
			}
			parent->pLeft = child;
			replace->pRight = node->pRight;
			node->pRight->pParent = replace;
		}
		replace->pParent = node->pParent;
		replace->color = node->color;
		replace->pLeft = node->pLeft;
		node->pLeft->pParent = replace;

		if (color == BLACK) {
			removeAdjust(child, parent);
		}

		node = nullptr;
		return;
	}

	if (node->pLeft != nullptr) {
		child = node->pLeft;
	} else {
		child = node->pRight;
	}

	parent = node->pParent;
	// 保存"取代节点"的颜色
	color = node->color;

	if (child != nullptr) {
		child->pParent = parent;
	}

	// "node节点"不是根节点
	if (parent != nullptr) {
		if (parent->pLeft == node) {
			parent->pLeft = child;
		} else {
			parent->pRight = child;
		}
	} else {
		pRoot = child;
	}

	if (color == BLACK) {
		removeAdjust(child, parent);
	}
	node = nullptr;
}

/*
 * 删除结点(z)，并返回被删除的结点
 *
 * 参数说明：
 *     tree 红黑树的根结点
 *     z 删除的结点
 */
void RBTree::remove(int key) {
	Node* node;

	if ((node = iterativeSearch(pRoot, key)) != nullptr) {
		remove(node);
	}
}


void RBTree::Print (Node *pNode, int data, int direction) {
	if (pNode != NULL) {
		if (direction == 0)    // tree是根节点
			std::cout << std::setw (2) << pNode -> data << "(B) is root" << std::endl;
		else                // tree是分支节点
			std::cout << std::setw (2) << pNode -> data << ((pNode -> color == RED) ? "(R)" : "(B)") << " is "
					  << std::setw (2) << data << "'s " << std::setw (12)
					  << (direction == 1 ? "right child" : "left child") << std::endl;

		Print (pNode -> pLeft, pNode -> data, - 1);
		Print (pNode -> pRight, pNode -> data, 1);
	}
}


void RBTree::preTravel (Node *pNode) {
	if (pNode != nullptr) {
		preOrder += char(pNode->data + 48);
		preTravel (pNode -> pLeft);
		preTravel (pNode -> pRight);
	}
}

void RBTree::midTravel (Node *pNode) {
	if (pNode != nullptr) {
		midTravel (pNode -> pLeft);
		midOrder += char(pNode->data + 48);
		midTravel (pNode -> pRight);
	}
}

void RBTree::Destroy (Node *pNode) {
	while (pNode) {
		Destroy (pNode -> pLeft);
		Destroy (pNode -> pRight);
		//节点
		delete pNode;
		pNode = nullptr;
	}
}

void RBTree::preTravelColor (Node *pNode) {
	if (pNode != nullptr) {
		preColor += char(pNode->color == BLACK?'B':'R');
		preTravelColor (pNode -> pLeft);
		preTravelColor (pNode -> pRight);
	}
}

#endif //RBTREE_RBTREE_H
