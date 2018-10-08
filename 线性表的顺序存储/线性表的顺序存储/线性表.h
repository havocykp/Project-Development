#pragma once
#include <sal.h>

typedef int Operator; // 定义操作码类型
#define OR_SUCCESS 1  // 操作成功
#define OR_BUFFER_IS_FULL  -1  // 缓冲区满了
#define OR_BUFFER_IS_EMPTY -2  // 缓冲区为空
#define OR_ERROR           -3  // 未知错误
#define OR_INDEX_IS_WRONG  -4  // 索引值有误
#define OR_MEM_ERROR       -5  // 分配内存失败
#define MAX_SIZE          100  // 线性表最大长度


template <class T>
class CMyTemplateArray
{
public:
	CMyTemplateArray(); // 构造函数
	~CMyTemplateArray(); // 析构函数

	// 初始化，建立一个线性表 
	bool initArray(_Out_ int& nError);
	// 判断线性表是否为空
	bool isEmpty(_Out_ int& nError);
	// 清空线性表
	bool clearArray(_Out_ int& nError);
	// 按下标获取元素
	bool getEleByIndex(_In_ int nPos, _Out_ T& nEle, _Out_ int& nError);
	// 按元素值获取下标
	bool getEleByValue(_Out_ int& nPos, _In_ T& nEle, _Out_ int& nError);
	// 插入元素
	bool insertEleByIndex(_In_ int nPos, _In_ T nEle, _Out_ int& nError);
	// 按位置删除元素
	bool delEleByIndex(_In_ int nPos, _Out_ T& nEle, _Out_ int& nError);
	// 按元素值删除元素
	bool delEleByValue(_In_ T nEle, _Out_ int& nError);
	// 返回当前长度
	bool getLength(_In_ int& nLenth, _In_ int& nError);

private:
	T m_nBuf[MAX_SIZE];
	int m_nLenth;
};


/* 构造函数 */
template <class T>
CMyTemplateArray<T>::CMyTemplateArray()
{
	memset(m_nBuf, 0, sizeof(int) * MAX_SIZE);
	m_nLenth = 0;
}

/* 析构函数 */
template <class T>
CMyTemplateArray<T>::~CMyTemplateArray()
{

}

/* 初始化，建立一个线性表 */ 
template <class T>
bool CMyTemplateArray<T>::initArray(_Out_ int& nError)
{
	return clearArray(nError);
}

// 判断线性表是否为空
template <class T>
bool CMyTemplateArray<T>::isEmpty(_Out_ int& nError)
{
	nError = OR_SUCCESS;

	if (m_nLenth == 0) {
		return true;
	}
	else{
		return false;
	}
}

/* 清空线性表 */
template <class T>
bool CMyTemplateArray<T>::clearArray(_Out_ int& nError)
{
	for (int i = 0; i < m_nLenth; ++i) {
		m_nBuf[i] = 0;
	}
	m_nLenth = 0;
	nError = OR_SUCCESS;

	return true;
}

/* 按下标获取元素 */
template <class T>
bool CMyTemplateArray<T>::getEleByIndex(_In_ int nPos, _Out_ T& nEle, _Out_ int& nError)
{
	// 1.检测是否越界
	if (nPos >= m_nLenth) {
		nError = OR_INDEX_IS_WRONG;
		return false;
	}
	// 2.直接赋值
	nEle = m_nBuf[nPos];

	// 3.返回true
	nError = OR_SUCCESS;

	return true;
}

/* 按元素值获取下标 */
template <class T> 
bool CMyTemplateArray<T>::getEleByValue(_Out_ int& nPos, _In_ T& nEle, _Out_ int& nError)
{
	// 1.先将Loc返回值置为一个不会出现的值，如果没有找到则传出此值，代表没有找到
	nPos = -1;

	// 2.在线性表中查找目标元素，传出匹配到的第一个位置
	for (int i = 0; i < m_nLenth; ++i) {
		if (nEle == m_nBuf[i]) {
			nPos = i;
			break;
		}
	}

	// 3.设置错误码
	if (nPos > -1) {
		nError = OR_SUCCESS;
		return true;
	}
	else {
		nError = OR_INDEX_IS_WRONG;
		return false;
	}
}

/* 插入元素 */
template <class T>
bool CMyTemplateArray<T>::insertEleByIndex(_In_ int nPos, _In_ T nEle, _Out_ int& nError)
{
	// 1.检测是否已满
	if (m_nLenth == MAX_SIZE) {
		nError = OR_BUFFER_IS_FULL;
		return false;
	}

	// 2.检测是否越界
	if (nPos > m_nLenth || nPos < 0) {
		nError = OR_INDEX_IS_WRONG;
		return false;
	}

	// 3.假如插入的不是最后一个元素，则之前的每一个元素都往后移
	if (nPos != m_nLenth) {
		for (int i = m_nLenth - 1; i >= nPos; --i) {
			m_nBuf[i + 1] = m_nBuf[i];
		}
	}

	// 4.直接在数组相应的位置赋值
	m_nBuf[nPos] = nEle;

	// 5.元素个数自增
	m_nLenth++;

	// 6.设置错误码
	nError = OR_SUCCESS;
	return true;
}

/* 按位置删除元素,并传出删除元素 */
template <class T>
bool CMyTemplateArray<T>::delEleByIndex(_In_ int nPos, _Out_ T& nEle, _Out_ int& nError)
{
	// 1.检测线性表是否为空
	if (m_nLenth == 0) {
		nError = OR_BUFFER_IS_EMPTY;
		return false;
	}

	// 2.检测是否越界
	if (nPos >= m_nLenth || nPos < 0) {
		nError = OR_INDEX_IS_WRONG;
		return false;
	}

	// 3.用待删元素设置返回值
	nEle = m_nBuf[nPos];

	// 4.假如删除的不是最后一个元素，那么需要往前移动
	// 注：最后一个元素根本不用管
	if (nPos != (m_nLenth - 1)) {
		for (int i = nPos; i < m_nLenth - 1; ++i) {
			m_nBuf[i] = m_nBuf[i + 1];
		}
	}

	// 5.长度自减
	m_nBuf[m_nLenth - 1] = 0;
	m_nLenth--;

	// 6.设置错误码，以及返回值
	nError = OR_SUCCESS;
	return true;
}

/* 按元素值删除元素，并传出删除的元素 */
template <class T>
bool CMyTemplateArray<T>::delEleByValue(_In_ T nEle, _Out_ int& nError)
{
	// 1.获取待删除元素的下标
	int nPos = -1;
	bool isSuccess;
	isSuccess = getEleByValue(nPos, nEle, nError);

	// 2.处理找不到的情况
	if (!isSuccess) {
		return false;
	}

	// 3.能正确找到下标
	isSuccess = delEleByIndex(nPos, nEle, nError);

	// 4.处理删除失败的情况
	if (!isSuccess) {
		return false;
	}

	// 5.正确返回
	nError = OR_SUCCESS;
	return true;
}

/* 返回当前长度 */
template <class T>
bool CMyTemplateArray<T>::getLength(_In_ int& nLenth, _In_ int& nError)
{
	nLenth = m_nLenth;
	nError = OR_SUCCESS;
	return true;
}

