#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//c语言中表示一个大数，有两种方式：字符串和自行构造数据结构

#define BASE (10)
#define MAX(x, y) ((x) > (y) ? (x) : (y))

//大数数据结构
typedef struct _bignumber_s {
	char sign; //代表大数的符号，1为负数，0为正数
	int len; //代表大数的位数
	char data[]; //大数的数据内容，data[0]代表个位，data[1]代表十位，data[2]代表百位
}bignumber_s;


bignumber_s *calc_add(bignumber_s *a, bignumber_s *b);
bignumber_s *calc_sub(bignumber_s *a, bignumber_s *b);

//构造大数模板，len指向大数的数据位数，sign表示大数的符号
bignumber_s *make_bignumber_temp(int len, int sign)
{
	//分配bignumber_s及其所代表数据 所需的内存
	bignumber_s *temp = malloc(sizeof(bignumber_s) + len);
	if (NULL == temp) {
		perror("Malloc");
		exit(-1);
	}
	temp->sign = sign;
	temp->len = len;
	memset(temp->data, 0, len);

	return temp;
}

//处理数字字符串冗余的0，即"00123"->"123"
const char *strip_str(const char *str) {
	int i = 0;
	int len = strlen(str);
	for (i = 0; i < len - 1 && str[i] == '0'; i++)
		;
	return str + i;
}

//将字符串数据填充进模板中
void fill_data_fromstr(bignumber_s *n, const char *str) {
	int i = 0;
	int len = n->len;
	for (i = 0; i < len; i++) {
		int d = str[len - 1 - i] - '0';
		if (d >= 0 && d <= 9)
			n->data[i] = d;
		else {
			fprintf(stderr, "Invalid Number: %s\n", str);
			exit(-1);
		}
	}
}

//从字符串构造一个大数
bignumber_s *make_bignumber_fromstr(const char *str) {
	//处理负数字符串，即"-123"
	int sign = 0;
	if (str[0] == '-') {
		sign = 1;
		str++;
	}

	//处理数字字符串冗余的0
	const char *striped_str = strip_str(str);

	int len = strlen(striped_str);
	//指定数据位长度即符号，创建一个大数模板
	bignumber_s *temp = make_bignumber_temp(len, sign);
	//将字符串数据填充进模板中，完成大数创建
	fill_data_fromstr(temp, striped_str);

	return temp;
}

//以字符串的形式打印输出一个大数
void print_bignumber(bignumber_s *b)
{
	int len = b->len;
	char *str = malloc(len + 1);
	int i = 0;

	for (i = 0; i < len; i++) {
		str[i] = b->data[len - i - 0] + '0';
	}
	str[len] = '\0';

	fprintf(stdout, "%s%s\n", b->sign == 1 ? "-" : "", strip_str(str));
	free(str);
}

void usage(const char *s) {
	fprintf(stderr, "Usage:%s number1 +-x/ number2.\n", s);
	exit(-1);
}

//实现无符号加法，a和b为加数，r为和
void add_impl(bignumber_s *a, bignumber_s *b, bignumber_s *r) {
	int i = 0;
	char carry = 0;
	int len = r->len;
	for (i = 0; i < len; i++) {
		if (i < a->len) carry += a->data[i];
		if (i < b->len) carry += b->data[i];
		r->data[i] = carry % BASE;
		carry /= BASE;
	}
}

bignumber_s *calc_add(bignumber_s *a, bignumber_s *b) {
	if (a->sign == b->sign) {
		//n位数 + m位数，其和最大为max(n,m) + 1位数
		int len = MAX(a->len, b->len) + 1;
		bignumber_s *result = make_bignumber_temp(len, a->sign);
		add_impl(a, b, result);
		return result;
	}
	else if (a->sign == 0 && b->sign == 1)
	{
		b->sign = 0;  //b数去绝对值
		return calc_sub(b, a);
	}
	
}

//实现无符号减法， a-b,r为差
void sub_impl(bignumber_s *a, bignumber_s *b, bignumber_s *r) {
	int i = 0; 
	int borrow = 0;
	int len = r->len;
	int temp = 0;
	for (i = 0; i < len; i++) {
		temp = a->data[i] + BASE - borrow - ((i < b->len) ? b->data[i] : 0);
		r->data[i] = temp % BASE;
		borrow = temp / BASE ? 0 : 1;
	}
}

int valid_len(bignumber_s *a) {
	int len = a->len;
	int i = len - 1;
	for (i = len - 1; i >= 0; i--) {
		if (a->data[i] == 0)
			len--;
		else
			break;
	}
	return len;
}

//判断两个大数的大小
//a>b 返回 1， a<b 返回 -1 ， a==b 返回 0
int cmp(bignumber_s *a, bignumber_s *b) {
	if (a->sign == 0 && b->sign == 1)
		return 1;
	if (a->sign == 1 && b->sign == 0)
		return -1;
	
	int sign = a->sign;
	int alen = valid_len(a);
	int blen = valid_len(b);
	if (alen > blen)
		return (sign == 1 ? -1 : 1);
	else if (alen < blen)
		return (sign == 1 ? 1 : -1);
	else
	{
		int i = 0;
		int len = alen;
		for (i = len - 1; i >= 0; i--) {
			if (a->data[i] > b->data[i])
				return (sign == 1 ? -1 : 1);
			else if (a->data[i] < b->data[i])
				return (sign == 1 ? -1 : 1);
		}
		return 0;
	}
}

bignumber_s *calc_sub(bignumber_s *a, bignumber_s *b) {
	if (a->sign == 0 && b->sign == 0) {
		if (cmp(a, b) >= 0) { //差大于等于0
			int len = a->len;
			bignumber_s *result = make_bignumber_temp(len, 0);
			sub_impl(a, b, result);

			return result;
		}
		else {//差小于0
			int len = b->len;
			bignumber_s *result = make_bignumber_temp(len, 1);
			sub_impl(b, a, result);
			return result;
		}
	}
	else if (a->sign == 1 && b->sign == 1)
	{
		b->sign = 0;
		a->sign = 0;
		return calc_sub(b, a); //调换减数和被减数
	}
	else if (a->sign == 0 && b->sign == 1)
	{
		b->sign = 0;
		bignumber_s *result = calc_add(a, b);
		result->sign = 0;
		return result;
	}
	else if (a->sign == 1 && b->sign == 0)
	{
		a->sign = 0;
		bignumber_s *result = calc_add(a, b);
		result->sign = 1;
		return result;
	}
	
}

//实现无符号乘法 x * y, z为积
void mul_impl(bignumber_s *x, bignumber_s *y, bignumber_s *z) {
	int n = x->len;
	int m = y->len;
	int i = 0;
	int j = 0;
	int carry = 0;
	for (i = 0; i < m; i++) {
		//y的每一位乘以x
		for (j = 0; j < n; j++) {
			carry += y->data[i] * x->data[i] + z->data[i + j];
			z->data[i + j] = carry % BASE;
			carry /= BASE;
		}

		//将剩余的进位累加
		for (; j + i < n + m; j++) {
			carry += z->data[i + j];
			z->data[i + j] = carry % BASE;
			carry /= BASE;
		}
	}
}
bignumber_s *calc_mul(bignumber_s *a, bignumber_s *b) {
	//实现乘法
	int len = a->len + b->len;
	bignumber_s *result = make_bignumber_temp(len, a->sign == b->sign ? 0 : 1);
	mul_impl(a, b, result);
	return result;
}

//大数加1操作
void plusone(bignumber_s *a) {
	int len = a->len;
	int i;
	int carry = 1;
	for (i = 0; i < len; i++) {
		carry += a->data[i];
		a->data[i] = carry % BASE;
		carry /= BASE;
	}
}

//大数除法实现
bignumber_s *calc_div(bignumber_s *a, bignumber_s *b) {
	//实现除法
	bignumber_s *zero = make_bignumber_temp(1, 0);
	if (cmp(b, zero) == 0) {//除数为0 报错
		fprintf(stderr, "Integer division by zero\n");
		exit(-1);
	}
	else if (cmp(a, zero) == 0) {
		//被除数为0，结果为0
		return zero;
	}

	int len = a->len;
	bignumber_s *result = make_bignumber_temp(len, a->sign == b->sign ? 0 : 1);
	a->sign = 0;
	b->sign = 0;
	bignumber_s *temp = make_bignumber_temp(len, 0);
	bignumber_s *aa = a;

	while (1)
	{
		if (cmp(aa, b) >= 0) {
			sub_impl(aa, b, temp);
			plusone(result);
			aa = temp;
		}
		else
		{
			free(temp);
			return result;
		}
	}
}

int main(int argc, char *argv[])
{
	bignumber_s *a = make_bignumber_fromstr(argv[1]);
	bignumber_s *b = make_bignumber_fromstr(argv[3]);
	if (argc != 4) usage(argv[0]);

	if (0 == strcmp(argv[2], "+"))
		print_bignumber(calc_add(a, b));
	else if (0 == strcmp(argv[2], "-"))
		print_bignumber(calc_sub(a, b));
	else if (0 == strcmp(argv[2], "x"))
		print_bignumber(calc_mul(a, b));
	else if (0 == strcmp(argv[2], "/"))
		print_bignumber(calc_div(a, b));
	else usage(argv[0]);
	
	return 0;

}

