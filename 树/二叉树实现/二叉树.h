#pragma once
#include <cstdio>

// 节点的定义
typedef struct NODE
{
	int Element;
	NODE *LeftChild;
	NODE *RightChild;
}NODE, *PNODE;

// 二叉搜索树
class bstree
{
public:
	// 使用默认的构造函数
	bstree() = default;
	~bstree() = default;

	// 提供给用户层的插入接口
	PNODE insert(int Elem)
	{
		return insert(RootNode, Elem);
	}

	// 在树中删除一个节点，返回被插入的节点
	PNODE erase(int Elem)
	{
		return erase(RootNode, Elem);
	}

	// 提供给用户层的输出接口
	void show()
	{
		show(RootNode, 0, '*');
		printf("------------------\n");
	}

	// 提供给用户层的前序遍历
	void Pre()
	{
		Pre(RootNode);
		printf("\n");
	}

	// 提供给用户层的中序遍历
	void Mid()
	{
		Mid(RootNode);
		printf("\n");
	}

	// 提供给用户层的后序遍历
	void Rear()
	{
		Rear(RootNode);
		printf("\n");
	}

	// 提供给用户层的层序遍历
	void Level()
	{
		Level(RootNode);
		printf("\n");
	}

private:
	// 输出当前树的节点
	// 参数2：宽度
	// 参数3：连接符号
	void show(PNODE Node, int w, char c);
	
	//在树中插入一个节点，返回被插入的节点
	PNODE insert(PNODE &Node, int Elem);

	// 在树中删除一个节点，返回被插入的节点
	PNODE erase(PNODE &Node, int Elem);

	// 在树中查找最小值
	int min(PNODE Node);

	// 在树中查找最大值
	int max(PNODE Node);

	// 前序遍历
	void Pre(PNODE Node);

	// 中序遍历
	void Mid(PNODE Node);

	// 后序遍历
	void Rear(PNODE Node);

	// 层序遍历
	void Level(PNODE Node);


private:
	PNODE RootNode = nullptr; // 指向根节点的指针
};
