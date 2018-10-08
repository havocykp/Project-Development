#include "stdafx.h"
#include "AVL.h"
#include <vector>

using namespace std;

// 输出当前树的节点
void avltree::show(PNODE Node, int w, char c)
{
	if (Node != nullptr)
	{
		show(Node->RightChild, w + 4, '/');
		printf("%*s%c%02d\n", w, " ", c, Node->Element);
		show(Node->LeftChild, w + 4, '\\');
	}
}


// 在树种查找最小值
int avltree::min(PNODE Node)
{
	while (Node->LeftChild)
		Node = Node->LeftChild;

	return Node->Element;
}

// 在树中查找最大值
int avltree::max(PNODE Node)
{
	while (Node->RightChild)
		Node = Node->RightChild;

	return Node->Element;
}

// 在树中插入一个节点，返回被插入的节点
PNODE avltree::insert(PNODE &Node, int Elem)
{
	// 插入节点有4种可能

	// 1. 当前的位置为nullptr，说明可以插入
	if (Node == nullptr)
	{
		// 申请堆空间并初始化对象
		Node = new NODE{ Elem, nullptr, nullptr };
		// 判断申请是否成功
		if (!Node) printf("空间申请失败!\n");
	}
	// 2. 当前要插入的值较大，需要插入到右子树
	else if (Elem > Node->Element)
	{
		// 将右子树指向新插入的节点
		Node->RightChild = insert(Node->RightChild, Elem);
		// 如果插入到右子树，那么右子树可能会增高，就用右子树高度来减
		if (high(Node->RightChild) - high(Node->LeftChild) == 2)
		{
			// 如果树没有歪
			if (Elem > Node->RightChild->Element) // 修正了bug
				Node = left(Node);
			else
				Node = rightleft(Node);
		}
	}
	// 3. 当前要插入的值较小，需要插入到左子树
	else if (Elem < Node->Element)
	{
		// 将左子树指向新插入的节点
		Node->LeftChild = insert(Node->LeftChild, Elem);
		// 如果插入到左子树，那么左子树可能会增高，就用左子树高度来减
		if (high(Node->LeftChild) - high(Node->RightChild) == 2)
		{
			// 如果树没有歪
			if (Elem > Node->LeftChild->Element) // 修正了bug
				Node = right(Node);
			else
				Node = leftright(Node);
		}
	}
	// 4. 如果已存在要插入的 值，就不做任何操作
	return Node;
}

// 在树中删除一个节点，返回被插入的节点
PNODE avltree::erase(PNODE &Node, int Elem)
{
	// 删除节点的4种比较可能

	// 1. 当前的位置为nullptr，说明没有找到节点
	if (Node == nullptr)
	{
		return Node;
	}
	// 2. 当前要删除的值较大，需要从右子树种删除
	else if (Elem > Node->Element)
	{
		// 将被删除位置当前的值重新赋值
		Node->RightChild = erase(Node->RightChild, Elem);
		// 如果从右子树删除，那么右子树可能会变矮，就用左子树高度来减
		if (high(Node->LeftChild) - high(Node->RightChild) == 2)
		{
			// 如果树没有歪
			if (Node->LeftChild->LeftChild)
				Node = right(Node);
			else
				Node = leftright(Node);
		}
	}
	// 3. 当前要删除的值较大，需要从左子树种删除
	else if (Elem < Node->Element)
	{
		// 将被删除位置当前的值重新赋值
		Node->LeftChild = erase(Node->LeftChild, Elem);
		// 如果从左子树删除，那么左子树可能会变矮，就用右子树高度来减
		if (high(Node->RightChild) - high(Node->LeftChild) == 2)
		{
			// 如果树没有歪
			if (Node->RightChild->RightChild)
				Node = left(Node);
			else
				Node = rightleft(Node);
		}
	}
	// 4. 如果相等则判断情况进行删除
	else
	{
		// 1. 有两个子节点
		if (Node->LeftChild && Node->RightChild)
		{
			// 1. 在左子树中查找最大值
			int MaxNode = max(Node->LeftChild);
			Node->Element = MaxNode;
			Node->LeftChild = erase(Node->LeftChild, MaxNode);

			// 2. 在右子树种查找最小值
			// int MinNode = min(Node->RightChild);
			// Node->Element = MinNode;
			// Node->RightChild = erase(Node->RightChild, MinNode);
		}
		// 2. 有单个子节点 - 3. 是叶子节点
		else
		{
			PNODE temp = Node;

			// 如果他有左子树
			if (Node->LeftChild) Node->LeftChild;
			else if (Node->RightChild) Node = Node->RightChild;
			else Node = nullptr;

			delete temp;
		}
	}

	return Node;
}

// 前序遍历树
void avltree::Pre(PNODE Node)
{
	// 根 左 右
	if (Node != nullptr)
	{
		printf("%d ", Node->Element);
		Pre(Node->LeftChild);
		Pre(Node->RightChild);
	}
}

// 中序遍历树
void avltree::Mid(PNODE Node)
{
	if (Node != nullptr)
	{
		// 左 根 右
		Mid(Node->LeftChild);
		printf("%d ", Node->Element);
		Mid(Node->RightChild);
	}
}

// 后序遍历树
void avltree::Rear(PNODE Node)
{
	if (Node != nullptr)
	{
		// 左 右 根
		Rear(Node->LeftChild);
		Rear(Node->RightChild);
		printf("%d ", Node->Element);
	}
}

// 层序遍历
void avltree::Level(PNODE Node)
{
	// 用于存放当前层找到的所有子节点
	vector<PNODE> ChildNode;
	// 在表示当前层的容器中，放入根节点
	vector<PNODE> NowLevel = { RootNode };

	// 循环输出所有的节点
	while (NowLevel.size())
	{
		// 遍历当前层的所有节点
		for (size_t i = 0; i < NowLevel.size(); ++i)
		{
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

// 获取树的高度
int avltree::high(PNODE Node)
{
	// 如果当前节点为空就返回0
	if (Node == nullptr)
		return 0;

	// 获取左右子树的高度
	int left = high(Node->LeftChild);
	int right = high(Node->RightChild);

	// 将较大的子树高度+1就是当前树的高度
	return left > right ? left + 1 : right + 1;
}

// 左
PNODE avltree::left(PNODE k2)
{
	// 先获取新的根节点
	// 1. 右子为根
	PNODE k1 = k2->RightChild;
	// 2. 右孙变左孙
	k2->RightChild = k1->LeftChild;
	// 3. 根为左子
	k1->LeftChild = k2;


	return k1;
}

// 右
PNODE avltree::right(PNODE k2)
{
	// 先获取新的根节点
	// 1. 左子为根
	PNODE k1 = k2->LeftChild;
	// 2. 左孙变右孙
	k2->LeftChild = k1->RightChild;
	// 3. 根为左子
	k1->RightChild = k2;

	return k1;
}

// 左右
PNODE avltree::leftright(PNODE Node)
{
	Node->LeftChild = left(Node->LeftChild);

	return right(Node);
}

// 右左

PNODE avltree::rightleft(PNODE Node)
{
	Node->RightChild = right(Node->RightChild);

	return left(Node);
}