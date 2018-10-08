#include "stdafx.h"
#include "动态线性表.h"

/* 构造函数*/
CMyVector::CMyVector()
{
	m_nBuf = nullptr;
	m_nLenth = 0;
	m_nSize = 0;
}

/* 析构函数 */
CMyVector::~CMyVector()
{
	int nError;
	clearVector(nError);
}

/* 初始化，建立线性表 */
bool CMyVector::initVector(_Out_ int& nError)
{
	clearVector(nError);
	m_nBuf = new int[INIT_SIZE];
	memset(m_nBuf, 0, sizeof(int) * INIT_SIZE);
	m_nLenth = 0;
	m_nSize = INIT_SIZE;
	return true;
}

/* 判断线性表表是否为空 */
bool CMyVector::isEmpty(_Out_ int& nError)
{
	nError = OR_SUCCESS;

	if (m_nLenth == 0) {
		return true;
	}
	else {
		return false;
	}
}

/* 判断线性表是否满 */
bool CMyVector::isFull(_Out_ int& nError)
{
	nError = OR_SUCCESS;

	if (m_nLenth == m_nSize) {
		return true;
	}
	else {
		return false;
	}
}

/* 清空线性表 */
bool CMyVector::clearVector(_Out_ int& nError)
{
	if (m_nBuf) {
		delete[] m_nBuf;
	}
	m_nLenth = 0;
	m_nSize = 0;
	nError = OR_SUCCESS;
	return true;
}

/* 按下标获取元素 */
bool CMyVector::getEleByIndex(_In_ int nPos, _Out_ int& nEle, _Out_ int& nError)
{
	// 1.检测是否越界
	if (nPos >= m_nLenth || nPos < 0) {
		nError = OR_INDEX_IS_WRONG;
		return false;
	}

	// 2.直接赋值
	nEle = m_nBuf[nPos];

	// 3.返回true
	return true;
}

/* 按元素值获取下标 */
bool CMyVector::getEleByValue(_Out_ int& nPos, _In_ int& nEle, _Out_ int& nError)
{
	// 1.先将Loc返回值置为一个不会出现的值，如果没有找到，则传出此值，代表没有找到
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
bool CMyVector::insertEleByIndex(_In_ int nPos, _In_ int nEle, _Out_ int& nError)
{
	// 1.检测是否已满
	if (isFull(nError)) {
		// 满了，直接返回
		if (!reSize(nError)) {
			return true;
		}
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

/* 按位置删除元素 */
bool CMyVector::delEleByIndex(_In_ int nPos, _Out_ int& nEle, _Out_ int& nError)
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

	// 3.用待删元素返回值
	nEle = m_nBuf[nPos];

	// 4.假如删除的不是最后一个元素，那么需要前移
	// 注，最后一个元素不用管
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

/* 按元素值删除元素 */
bool CMyVector::delEleByValue(_In_ int nEle, _Out_ int& nError)
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

/* 重新设置vector的大小 */
bool CMyVector::reSize(_In_ int& nError)
{
	// 1.查看是否能够开辟内存
	int newSize = 0;
	if (m_nSize == MAX_SIZE) {
		nError = OR_BUFFER_IS_FULL;
		return false;
	}
	if (2 * m_nSize >= MAX_SIZE) {
		newSize = MAX_SIZE;
	}
	else {
		newSize = 2 * m_nSize;
	}

	// 2.重新开辟内存
	int *tempBuf = new int[newSize];

	// 3.把数据转移过来
	for (int i = 0; i < m_nLenth; ++i) {
		tempBuf[i] = m_nBuf[i];
	}

	// 4.清除原内存
	int lenth = m_nLenth;
	bool isSuccess = clearVector(nError);

	// 5.新内存数据赋给成员变量
	m_nBuf = tempBuf;
	m_nSize = newSize;
	m_nLenth = lenth;

	// 5.设置返回值
	if (isSuccess) {
		return true;
	}
	else {
		m_nBuf = nullptr;
		m_nSize = 0;
		m_nLenth = 0;
		return false;
	}
}

/* 返回当前长度 */
bool CMyVector::getLength(_Out_ int& nLenth, _Out_ int& nError)
{
	nLenth = m_nLenth;
	nError = OR_SUCCESS;
	return true;
}

/* 返回最大长度 */
bool CMyVector::max_size(_Out_ int& nLenth, _Out_ int& nError)
{
	nError = OR_SUCCESS;
	nLenth = MAX_SIZE;
	return true;
}

/* 在尾部插入 */
bool CMyVector::push_back(_In_ int nEle, _Out_ int& nError)
{
	int nlen;
	getLength(nlen, nError); // 成员变量保存当前长度
	insertEleByIndex(nlen, nEle, nError);
	return true;
}

/* 弹出尾部元素 */
bool CMyVector::pop_back(_Out_ int& nEle, _Out_ int& nError)
{
	int nlen;
	getLength(nlen, nError);
	delEleByIndex(nlen - 1, nEle, nError);
	return true;
}

/* 在下标处插入元素 */
bool CMyVector::insert(_In_ int pos, _In_ int elem, _Out_ int& nError)
{
	insertEleByIndex(pos, elem, nError);
	return true;
}

/* 在下标处插入多个元素 */
bool CMyVector::insert(_In_ int pos, _In_ int n, _In_ int elem, _Out_ int& nError)
{
	for (int i = 0; i < n; ++i) {
		insertEleByIndex(pos + i, elem, nError);
	}
	return true;
}

/* 在开始坐标与终点坐标之间插入多个元素 */
bool CMyVector::insertEx(_In_ int beg, _In_ int end, _In_ int elem, _Out_ int& nError)
{
	for (int i = 0; i < end - beg; ++i) {
		insertEleByIndex(beg + i, elem, nError);
	}
	return true;
}

/* 返回指定位置的元素 */
bool CMyVector::at(_In_ int idx, _Out_ int& elem, _Out_ int& nError)
{
	return getEleByIndex(idx, elem, nError);
}

/* 重载[]符号 */
int CMyVector::operator[](_In_ int pos)
{
	int elem;
	int nError;
	if (getEleByIndex(pos, elem, nError)) {
		return m_nBuf[pos]; // return elem
	}
}