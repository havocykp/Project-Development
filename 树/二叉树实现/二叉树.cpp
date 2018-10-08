#include "stdafx.h"
#include "二叉树.h"
#include <vector>

using std::vector;

/** 输出当前树的节点
 *  参数2：宽度
 *  参数3：连接符号 
 */
void bstree::show(PNODE Node, int w, char c)
{
	if (Node != nullptr) {
		show(Node->RightChild, w + 4, '/');
		printf("%*s%c%02d\n", w, " ", c, Node->Element);
		show(Node->LeftChild, w + 4, '\\');
	}
}

/**
 * 在树中查找最小值
 */
int bstree::min(PNODE Node)
{
	while (Node->LeftChild)
		Node = Node->LeftChild;

	return Node->Element;
}

/**
* 在树中查找最大值
*/
int bstree::max(PNODE Node)
{
	while (Node->RightChild)
		Node = Node->RightChild;

	return Node->Element;
}

/**
 * 在树中插入一个节点，返回被插入的节点
 */
PNODE bstree::insert(PNODE &Node, int Elem)
{
	// 插入节点有4中可能

	// 1.当前的位置为nullptr,说明可以插入
	if (Node == nullptr)
	{
		// 申请空间并初始化对象
		Node = new NODE{ Elem, nullptr, nullptr };
		// 判断申请是否成功
		if (!Node)  printf("空间申请失败!\n");
	}

	// 2.当前要插入的值较大，需要插入到右子树
	else if (Elem > Node->Element)
	{
		// 将右子树指向要插入的节点
		Node->RightChild = insert(Node->RightChild, Elem);
	}

	// 3.当前要插入的值较小，需要插入到左子树
	else if (Elem < Node->Element)
	{
		// 将左子树指向要插入的节点
		Node->LeftChild = insert(Node->LeftChild, Elem);
	}

	// 4.如果已存在要插入的值，就不做任何操作
	return Node;
}

/** 
 * 在树中删除一个节点，返回被插入的节点
 */
PNODE bstree::erase(PNODE &Node, int Elem)
{
	// 删除节点的4种比较可能

	// 1.当前的位置为nullptr，说明没有找到节点
	if (Node == nullptr) {
		return Node;
	}

	// 2.当前要删除的值较大，需要从右子树中删除
	else if (Elem > Node->Element) {
		// 将被删除位置当前的值重新赋值
		Node->RightChild = erase(Node->RightChild, Elem);
	}

	// 3.当前要删除的值较小，需要从左子树中删除
	else if (Elem < Node->Element) {
		// 将被删除位置当前的值重新赋值
		Node->LeftChild = erase(Node->LeftChild, Elem);
	}

	// 4.如果相等，则判断情况进行删除
	else
	{
		// 1.有两个子节点
		if (Node->LeftChild && Node->RightChild) {
			// 1.在左子树中查找最大值
			int MaxNode = max(Node->LeftChild);
			Node->Element = MaxNode;
			Node->LeftChild = erase(Node->LeftChild, MaxNode);

			// 2.在右子树中查找最小值
// 			int MinNode = min(Node->RightChild);
// 			Node->Element = MinNode;
// 			Node->RightChild = erase(Node->RightChild, MinNode);
		}

		// 2.有单个子节点
		// 3.是叶子节点
		else {
			PNODE temp = Node;

			// 如果它有左子树
			if (Node->LeftChild) Node->LeftChild;
			else if (Node->RightChild) Node = Node->RightChild;
			else Node = nullptr;

			delete temp;
		}
	}

	return Node;
}

/**
 * 前序遍历
 */

void bstree::Pre(PNODE Node)
{
	// 根 左 右
	if (Node != nullptr) {
		printf("%d ", Node->Element);
		Pre(Node->LeftChild);
		Pre(Node->RightChild);
	}
}

/**
* 中序遍历
*/
void bstree::Mid(PNODE Node)
{
	// 左 根 右
	if (Node != nullptr) {
		Mid(Node->LeftChild);
		printf("%d ", Node->Element);
		Mid(Node->RightChild);
	}
}

/**
* 后序遍历
*/
void bstree::Rear(PNODE Node)
{
	// 左 右 根
	if (Node != nullptr) {
		Rear(Node->LeftChild);
		Rear(Node->RightChild);
		printf("%d ", Node->Element);
	}
}


/**
* 层序遍历
*/
void bstree::Level(PNODE Node)
{
	// 用于存放当前层找到的所有子节点
	vector<PNODE> ChildNode;
	// 在表示当前层的容器中，放入根结点
	vector<PNODE> NowLevel = { RootNode };

	// 循环输出所有的节点
	while (NowLevel.size()) {
		// 遍历当前层的所有节点
		for (size_t i = 0; i < NowLevel.size(); ++i) {
			printf("%d ", NowLevel[i]->Element);
			if (NowLevel[i]->LeftChild)
				ChildNode.push_back(NowLevel[i]->LeftChild);
			if (NowLevel[i]->RightChild)
				ChildNode.push_back(NowLevel[i]->RightChild);
		}

		NowLevel = ChildNode;
		ChildNode.clear();
		printf("\n");
	}
}