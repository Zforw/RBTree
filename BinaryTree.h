#ifndef BINARYTREE_BINARYTREE_H
#define BINARYTREE_BINARYTREE_H

#include "BinTreeNode.h"
#include <string>
#include <queue>
#include <vector>
#include <algorithm>
#include <iostream>

int Max(const int &a, const int &b) {
	return a > b ? a : b;
}
int Min(const int &a, const int &b) {
	return a < b ? a : b;
}
template <class T>
class BinaryTree {
public:
	BinaryTree():root(nullptr) {}
	bool BuildTree(bool order, std::string s1, std::string mid, std::string property) {
		tag = true;
		if(order == 0) {
			root = preOrder(s1, mid, nullptr, 0, property);
			preShow (root);
			for(int i = 0;i < output.size();i++) {
				for(int j = 0;j < output[i].size();j++) {
					std::cout << output[i][j];
				}
				std::cout << std::endl;
			}
			std::cout << std::endl;
		} else {
			root = postOrder(s1, mid, nullptr, 0);
			postShow(s1);
		}
		return tag;
	}
	int Size() const {
		return root->size;
	}
	int Height() {
		return height(root);
	}
	void addNode(BinTreeNode<T> *rt, T x, bool dir) {
		if(dir == 0) {
			if(rt->lChild == nullptr) {
				rt -> lChild = new BinTreeNode<T> (x, rt);
			} else {
				addNode(rt->lChild, x, dir);
			}
		} else {
			if(rt->rChild == nullptr) {
				rt ->rChild = new BinTreeNode<T> (x, rt);
			} else {
				addNode(rt->rChild, x, dir);
			}
		}
	}
	void Destroy() {
		destroy(root);
	}
	void Show() {
		show(root);
	}
private:
	bool tag;//输入正确/错误
	BinTreeNode<T> *root;//根节点
	std::vector<int> HEIGHT, WIDTH;//记录不同深度的树输出后的高度和宽度
	std::vector<std::vector<int>> pos;//记录每一行的元素在树的位置
	std::vector<std::vector<char>> output;//输出内容
	std::vector<std::vector<std::pair<std::pair<char,char>, std::pair<int, int>>>> ans;////记录每一行元素的值
	void destroy(BinTreeNode<T> *rt) {
		if(rt) {
			destroy(rt->lChild);
			destroy(rt->rChild);
			delete rt;
			rt = nullptr;
		}
	}
	int height(BinTreeNode<T> *rt) {
		if(rt == nullptr) return 0;
		return Max(height(rt->lChild), height(rt->rChild)) + 1;
	}
	int totH(int H) {
		if(H == 1) return 1;
		else if(H == 2) return 3;
		else return totH(H - 1) * 2;
	}
	//h层子节点与父节点之间元素相隔的宽度
	int dx(int h) {
		if(Height() - h == 1) return 2;
		else return totH(Height() - h);
	}
	std::queue<int> elemPos(int h) {
		std::queue<int> q;
		if(h == 0) {
			q.push(totH(Height()) - 1);
		} else {
			std::queue<int> p = elemPos(h - 1);
			std::vector<int> t;
			while(!p.empty()) {
				int u = p.front();p.pop();
				t.push_back(u - dx(h));
				t.push_back(u + dx(h));
			}
			for(int i = 0;i < pos[h].size();i++) {
				if(pos[h][i]) q.push(t[i]);
			}
		}
		return q;
	}
	/*
	 * h = Height()
	 */
	int totW(int h) {
		if(h == 0) return 0;
		else if(h == 1) return 1;
		else if(h == 2) return 5;
		else return totW(h - 1) * 2 + 1;
	}
	//计算出二叉树的第i行应该输出到第几行
	int line(int i) {
		if(i == 0) return 0;
		else if(Height() - i == 1) {
			return 2 + line(i-1);
		}
		else return 1 + line(i-1) + (totW(Height() - i) - 1) / 2;
	}
	void preShow(BinTreeNode<T> *rt) {
		//每次都要重新遍历

		if(root == nullptr) return;
		std::queue<std::pair<BinTreeNode<T>*, int>> q;
		pos.clear();//每次都要重新遍历
		ans.clear();
		output.clear();
		q.push(std::make_pair(rt,1));
		std::vector<std::pair<std::pair<char,char>, std::pair<int, int>>> tmp;//保存一行的元素值
		std::vector<int> tp;//保存元素是所在层的第几个
		tp.push_back(1);
		pos.push_back(tp);
		tp.clear();
		int siz = 1;
		while(!q.empty()){
			BinTreeNode<T> * u = q.front().first;
			int d = q.front().second;
			q.pop();
			tmp.push_back(std::make_pair(std::make_pair(u->property,u->data), std::make_pair(0, 0)));

			if(u->lChild != nullptr) {
				q.push(std::make_pair(u->lChild, 2 * d - 1));
				tp.push_back(2 * d - 1);//左孩子是下一层的2*d-1个节点
			} else {
				tp.push_back(0);//左孩子为空pos记为0
			}
			if(u->rChild != nullptr) {
				q.push(std::make_pair(u->rChild,2 * d));
				tp.push_back(2 * d);//右孩子是下一层的2*d-1个节点
			} else {
				tp.push_back(0);//右孩子为空pos记为0//siz用于记录每一层节点的个数，当前层已遍历的节点个数等于siz时，队列中的节点全部为当前层的孩子，用队列中节点的个数来更新下一层的siz
			}
			//k++;
			if(tmp.size() == siz) {
				ans.push_back(tmp);
				pos.push_back(tp);
				tmp.clear();
				tp.clear();
				siz = q.size();
			}
		}

		std::vector<std::pair<int,int>> last;//记录上一层节点的位置, 用于连线
		int elem_line = 0, ans_cnt = 0;
		int last_pos = 0;
		int k = 0;
		std::vector<char> out;
		for(int i = 0; i < totH(Height());i++) {
			out.clear();
			if(i == line(elem_line)) {//当前行有节点
				std::queue<int> elem_pos = elemPos(elem_line);//获取节点位置
				last.clear();
				k = 0;
				for(int j = 0;j < totW(Height());j++) {
					if(!elem_pos.empty() && j == elem_pos.front()) {//当前位置j是节点
						if(elem_line < Height() - 1) {
							if(pos[elem_line + 1][2 * k] != 0) {//有左孩子
								last.push_back(std::make_pair(j - 1, 0));
							}
							if(pos[elem_line + 1][2 * k + 1] != 0) {//有右孩子
								last.push_back(std::make_pair(j + 1, 1));
							}
							k++;
						}
						out.push_back(ans[elem_line][ans_cnt].first.first);//将节点数据添加到输出数组里
						out.push_back(ans[elem_line][ans_cnt++].first.second);
						ans[elem_line][ans_cnt - 1].second = std::make_pair(i, j);
						//out.push_back(' ');
						elem_pos.pop();
					} else {
						out.push_back(' ');
						out.push_back(' ');
					}
				}
				output.push_back(out);
				out.clear();
				elem_line++;
				ans_cnt = 0;
			} else {
				last_pos = 0;
				for(int j = 0; j < totW(Height());j++) {
					if (last_pos < last.size() && j == last[last_pos].first) {
						if(last[last_pos].second) {//根据last进行画线, 若有右孩子
							out.push_back('\\');
							out.push_back(' ');
							last[last_pos].first++;//列坐标+1
						} else {//若有左孩子
							out.push_back('/');
							out.push_back(' ');
							last[last_pos].first--;//列坐标-1
						}
						last_pos++;
					}
					else {
						out.push_back(' ');
						out.push_back(' ');
					}
				}
				output.push_back(out);
				out.clear();
			}
		}


	}

	//可以做一个预处理
	std::pair<int, int> findPos(char c, int & isR, int & isL) {
		for(int i = ans.size() - 1;i >= 0;i--) {
			for(int j = 0;j < ans[i].size();j++) {
				if(ans[i][j].first.second == c) {

					int cnt = -1;
					for(int k = 0;k < pos[i].size();k++) {
						if(pos[i][k] != 0) cnt++;
						if(cnt == j) {
							isR = (pos[i][k] % 2 == 0);
							if(!isR && k + 1 < pos[i].size() && pos[i][k + 1] == 0) {
								isL = 1;
							}
							break;
						}
					}
					return ans[i][j].second;
				}
			}
		}
	}
	void postShow(std::string post) {
		preShow(root);//保存树的形态，进行选择性输出
		for(int h = 1;h <= Height();h++) {
			HEIGHT.push_back(totH(h));
			WIDTH.push_back(totW(h));
		}
		int maxJ = -1, minI = 100000;
		std::vector<int> end;//end 记录每一行输出的最后位置
		for(int i = 0;i < HEIGHT[HEIGHT.size() - 1];i++) {
			end.push_back(0);
		}
		for(int ps = 0; ps < post.length();ps++) {
			int isR = 0, isL = 0;
			std::pair<int, int> rc = findPos(post[ps], isR, isL);
			minI = Min(rc.first, minI);
			maxJ = Max(rc.second, maxJ);
			for(int i = minI;i < HEIGHT[HEIGHT.size() - 1];i++) {
				if(i == rc.first) {
					for(int j = 0;j <= rc.second * 2;j++) {
						std::cout << output[i][j];
					}
					end[i] = rc.second * 2;
					if(isR) {//若是右孩子
						int p;
						//找到上一层节点在输出数组中的高度
						for(p = 0;p < HEIGHT.size();) {
							if(HEIGHT[p] == HEIGHT[HEIGHT.size() - 1] - i) {
								break;
							} else {
								p++;
							}
						}
						//更新上一层到上一层节点所在行输出的最后位置，每一层都-1， 使得输出内容只到右孩子之上的连线 \ 截止
						for(int r = i - 1;r > HEIGHT[HEIGHT.size() - 1] - HEIGHT[p + 1];r--) {
							end[r] = end[r + 1] - 1;
						}
					} else if(isL) {//若是左孩子
						int p;
						for (p = 0; p < HEIGHT.size ();) {
							if (HEIGHT[p] == HEIGHT[HEIGHT.size () - 1] - i) {
								break;
							} else {
								p ++;
							}
						}
						if(p < HEIGHT.size() - 1) {
							for (int r = i - 1; r > HEIGHT[HEIGHT . size () - 1] - HEIGHT[p + 1]; r --) {
								end[r] = Max (end[r + 1] + 2, end[r]);
							}
						}
					}
				} else {
					for (int j = 0; j <= Min(maxJ * 2, end[i]); j ++) {
						std::cout << output[i][j];
					}
				}
				std::cout << std::endl;
			}
			std::cout << std::endl;
		}
	}
	BinTreeNode<T> * preOrder(std::string pre, std::string mid, BinTreeNode<T> *prt, bool dir, std::string property) {
		if(pre.length() == 0 && mid.length() == 0) return nullptr;
		if(pre.length() != mid.length()) {
			tag = false;
			return nullptr;
		}
		int m = mid.find(pre[0]), n = mid.length();
		if(m >= n || m < 0) {
			tag = false;
			return nullptr;
		}
		BinTreeNode<T> *rt = new BinTreeNode<T> (pre[0], prt, nullptr, nullptr, dir);
		rt->property = property[0];
		if(root == nullptr) root = rt;
		BinTreeNode<T> *l = preOrder(pre.substr(1, m), mid.substr(0, m), rt, 0, property.substr(1, m));
		BinTreeNode<T> *r = preOrder(pre.substr(m + 1, n - m - 1), mid.substr(m + 1, n - m - 1), rt, 1, property.substr(m + 1, n - m - 1));
		rt->lChild = l;
		rt->rChild = r;
		if(l) rt->_size += l->size();
		if(r) rt->_size += r->size();
		return rt;
	}

	BinTreeNode<T> * postOrder(std::string post, std::string mid, BinTreeNode<T> *prt, bool dir) {
		if(post.length() == 0 && mid.length() == 0) return nullptr;
		if(post.length() != mid.length()) {
			tag = false;
			return nullptr;
		}
		int n = mid.length(), m = mid.find(post[n - 1]);
		if(m >= n) {
			tag = false;
			return nullptr;
		}
		BinTreeNode<T> *rt = new BinTreeNode<T> (post[n - 1], prt, nullptr, nullptr, dir);
		if(root == nullptr) root = rt;
		BinTreeNode<T> *l = postOrder(post.substr(0, m), mid.substr(0, m), rt, 0);
		BinTreeNode<T> *r = postOrder(post.substr(m, n - m - 1), mid.substr(m + 1, n - m - 1), rt, 1);
		rt->lChild = l;
		rt->rChild = r;
		if(l) rt->_size += l->size();
		if(r) rt->_size += r->size();

		return rt;
	}
};

#endif //BINARYTREE_BINARYTREE_H
