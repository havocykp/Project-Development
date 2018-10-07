include windows.inc
include msvcrt.inc  ;包含c语言的库
includelib msvcrt.lib  ;包含c语言库对应的lib文件

include kernel32.inc
includelib kernel32.lib

.data
;定义结构体、声明全局变量
CONTACTSSTRUCT struct
	szName BYTE 25 dup(0) ;名字
	szPhNumber BYTE 12 dup(0) ;电话号码
CONTACTSSTRUCT ends
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
PCONTACTSSTRUCT TYPEDEF PTR ;取别名（指针类型）
;[新的类型名] TYPEDEF PTR [原来的类型名] ;表示取一个指针类型的类型
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>


;声明全局变量
g_stContacts CONTACTSSTRUCT 100 dup(<'0'>) ;定义结构体数组
g_nCount DWORD 0 ;元素个数
g_nCountMax DWORD 100 ;最大存放元素
g_strTemContacts CONTACTSSTRUCT <'0','0'> ;接收输入信息
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

;定义格式控制符 用于输入输出时用到的格式控制符
g_szScanfFormat BYTE '%s %s',0
g_szScanName BYTE '%s',0
g_nChoose dword 100 ;菜单选择
g_numScanfFormat byte '%d',0 ;菜单选择格式化输入符

;文件操作
g_szFileWB BYTE 'wb',0
g_szFileRB byte 'rb',0
g_pFile dword 0
g_szFile byte 'data.text',0

;system功能
g_szCls byte 'cls',0 ;清屏
g_szPause byte 'pause',0 ;暂停
g_szZero byte ' ',0dh,0ah,0 ;用于换行

;提示
g_szAddStr byte '请输入：用户名 电话号码',0dh,0ah,0
g_szFindInfo byte "请输入姓名：",0dh,0ah,0
g_szInputError byte '输入错误！',0dh,0ah,0
g_szTipNewName byte '请输入新用户名 电话号码',0dh,0ah,0
g_szTipNoExist byte '用户不存在...',0dh,0ah,0
g_szTipNoDate byte '没有数据...',0dh,0ah,0
g_szTipListOver byte '查询完成...',0dh,0ah,0
g_szOK byte '操作成功!',0
g_szMenu byte '请根据选项输入序号',0dh,0ah,
	      '1 - 查看用户',0dh,0ah,
	      '2 - 添加用户',0dh,0ah,
	      '3 - 搜索用户',0dh,0ah,
	      '4 - 修改用户',0dh,0ah,
	      '5 - 删除用户',0dh,0ah,
	      '0 - 退出', 0dh,0ah,0
	      
g_szTipSizeError byte '输入数据超出限制',0dh,0ah,0
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

.code 
;添加用户信息
ADD_USER proc ;无参数
	
	push eax
	push ebx ;下面是用到该寄存器，先保存一下原来寄存器中的值
	lea eax,g_szAddStr ;lea表示取第二个操作数的地址，放入第一个操作数中
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
;1.通过push给函数传参（4字节）
;2.调用函数 这时调用的是C语言的库函数，所以要包含头文件：
;    include msvcrt.inc
;    includelib msvcrt.lib
;    库函数的格式为：crt_xxx
;3.C库函数是由调用者平衡堆栈
;  每个参数的大小为4个字节，所以传入几个参数则在之后esp加回来
;  add esp,[参数个数*4]
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

        push eax ;push r/m32(imm32) 可以是寄存器或内存或立即数
        call crt_printf
        add esp,4
        
        ;根据ecx的值找到下一个结构体名字数组的地址
        
        lea esi,[g_stContacts] ;保存数据的结构体数组
        mov ecx,g_nCount ;获取当前已插入的用户个数
        mov eax,sizeof(CONTACTSSTRUCT) ;计算结构体的大小(sizeof宏)
        imul eax,ecx
        add esi,eax ;移动结构体数组的指针(用户个数*结构体的大小)
        
        ;调用crt_scanf函数接收输入的数据
        lea eax,[esi+CONTACTSSTRUCT.szPhNumber] ;第二个参数（电话号码）
        lea edx,[esi+CONTACTSSTRUCT.szName] ;第一个参数（姓名）
        push eax
        push edx
        push offset g_szScanfFormat ;格式控制符
        call crt_scanf
        add esp,0ch ;平衡堆栈
        inc g_nCount ;用户个数加1
        
        ;保存数据
        call SaveDate
        
        ;输入成功
        push offset g_szOK ;操作成功
        call crt_printf
        add esp,4
        
        ;暂停
        push offset g_szPause
        call crt_system
        add esp,4
        
        pop eax ;与函数开始push指令对应
        pop edx
        
	ret

ADD_USER endp


;查询用户信息
FindData proc
	
	;1.输入数据
	;输入查找信息
	push offset g_szFindInfo
	call crt_printf
	add esp,4
	
	;清空存放临时数据的结构体
	lea edi,[g_strTemContacts] ;保存结构体的地址
	mov ebx,sizeof(CONTACTSSTRUCT)
	push ebx
	push 0h
	push edi
	call crt_memset
	add esp,12
	
	lea edi,[g_strTemContacts.szName] ;保存结构体中名字的地址
	push edi
	push offset g_szScanName ;格式控制符
	call crt_scanf
	add esp,8 ;平衡堆栈
	
	;2.开始查询
	mov ecx,0 ;初始化循环次数（默认从0开始循环）
CYCLE_MARK:
        cmp ecx,g_nCount ;判断是否结束循环
        jz END_F
        
        ;2.1根据ecx的值找到下一个结构体名字数组的地址
        
        mov eax,sizeof(CONTACTSSTRUCT) ;计算结构体的大小(sizeof宏)
        imul eax,ecx
        lea esi,[g_stContacts] ;保存数据的结构体数组
        add esi,eax
        
        ;2.2比较字符串
        mov eax,ecx ;保存外层循环的次数
        mov ecx,6 ;初始化串操作的循环操作(4字节比较)
        
        lea edi,[g_strTemContacts.szName] ;保存当前插入的用户名字的地址
        repe cmpsd dword ptr[esi],dword ptr[edi] ;查看repe系列指令的使用原理
        je CARRIEDOUT_MARK ;如果找到则跳转(输出信息)
        mov ecx,eax ;如果没有找到则继续外层循环
        inc ecx ;层循环次数加1
        jmp CYCLE_MARK ;无条件跳转到外层循环开始位置
        
CARRIEDOUT_MARK:
	;输出信息
	mov ecx,eax
	lea esi,[g_stContacts]
	mov ebx,sizeof(CONTACTSSTRUCT)
	imul ebx,ecx
	add esi,ecx
	lea eax,[esi+CONTACTSSTRUCT.szPhNumber]
	push eax
	push offset g_szScanName
	call crt_printf
	add esp,8
	
	;换行
	push offset g_szZero
	call crt_printf
	add esp,4
	
	push offset g_szOK ;操作成功
	call crt_printf
	add esp,4
	
	;暂停
	push offset g_szPause
	call crt_system
	add esp,4
	
	ret
END_F:
	push offset g_szTipNoExist
	call crt_printf
	add esp,4
	;暂停
	push offset g_szPause
	call crt_system
	add esp,4
	ret
FindData endp

;修改用户信息
ModifyData proc
	
	;因为修改信息的第一步也是先要将当前输入的信息在已保存的数组中查询
	;1.输入数据
	;输入查找信息
	push offset g_szFindInfo
	;push offset g_szScanName
	call crt_printf
	add esp,4
	
	;清空存放临时数据的结构体
	lea edi,[g_strTemContacts] ;保存结构体的地址
	mov ebx,sizeof(CONTACTSSTRUCT)
	push ebx
	push 0h
	push edi
	call crt_memset
	add esp,12
	
	lea edi,[g_strTemContacts.szName] ;保存结构体中名字的地址
	push edi
	push offset g_szScanName ;格式控制符
	call crt_scanf 
	add esp,8 ;平衡堆栈
	
	;2.开始查询
	mov ecx,0 ;初始化循环次数（默认从0开始）
CYCLE_MARK: ;标号
	cmp ecx,g_nCount ;判断是否结束循环
	jz END_M
	
	;2.2比较字符串
	mov eax,ecx ;保存外层循环的次数
	mov ecx,6 ;初始化串操作的循环次数（4字节比较）
	repe cmpsd dword ptr[esi],dword ptr[edi]
	je CARRIEDOUT_MARK ;如果找到则跳转（修正信息）
	mov ecx,eax ;如果没有找到则继续外层循环
	inc ecx ;层循环次数加1
	jmp CYCLE_MARK ;无条件跳转到外层循环开始位置
	
CARRIEDOUT_MARK:
	;修改信息
	mov ecx,eax
	lea esi,[g_stContacts]
	mov ebx,sizeof(CONTACTSSTRUCT)
	imul ebx,ecx
	add esi,ebx
	
	;输入新信息
	push offset g_szTipNewName
	call crt_printf
	add esp,4
	
	lea ebx,[esi+CONTACTSSTRUCT.szName]
	lea eax,[esi+CONTACTSSTRUCT.szPhNumber]
	push eax
	push ebx
	push offset g_szScanfFormat
	call crt_scanf
	add esp,0ch
	
	;保存数据
	call SaveDate
	
	push offset g_szOK ;操作成功
	call crt_printf
	add esp,4
	
	;暂停
	push offset g_szPause
	call crt_system
	add esp,4
	
	ret
END_M:
	push offset g_szTipNoExist
	call crt_printf
	add esp,4
	;暂停
	push offset g_szPause
	call crt_system
	add esp,4
	ret

ModifyData endp


;删除用户信息 
RemoveDate proc
	
	;1.输入数据
	;输入查找信息
	push offset g_szFindInfo
	;push offset g_szScanName
	call crt_printf
	add esp,4
	
	;清空存放临时数据的结构体
	lea edi,[g_strTemContacts] ;保存结构体的地址
	mov ebx,sizeof(CONTACTSSTRUCT)
	push ebx
	push 0h
	push edi
	call crt_memset
	add esp,12
	
	lea edi,[g_strTemContacts.szName] ;保存结构体中名字的地址
	push edi
	push offset g_szScanName ;格式控制符
	call crt_scanf
	add esp,8 ;平衡堆栈
	
	;2.开始查询
	mov ecx,0 ;初始化循环次数（默认从0开始循环）
CYCLE_MARK: ;标号
	cmp ecx,g_nCount ;判断是否结束循环
	jz END_M
	
	;2.1根据ecx的值找到下一个结构体名字数组的地址
	lea esi,[g_stContacts] ;保存数据的结构体数组
	lea edi,[g_strTemContacts.szName] ;获取当前已输入的要查询用户名字地址
	mov eax,sizeof(CONTACTSSTRUCT)
	imul eax,ecx
	add esi,eax
	
	;2.2比较字符串
	mov eax,ecx ;保存外层循环的次数
	mov ecx,6 ;初始化串操作的循环次数（4字节比较）
	repe cmpsd dword ptr[esi],dword ptr[edi]
	je CARRIEDOUT_MARK
	mov ecx,eax ;没有找到则继续外层循环
	int ecx ;层循环次数加1
	jmp CYCLE_MARK ;无条件跳转到外层循环开始位置
	
CARRIRDOUT_MARK:
	;删除
	;将esi设置为当前要删除的结构体数组的首地址
	mov ecx,eax ;eax是在上面获取到的，表示当前找到的数组的位置
	lea edi,[g_stContacts]
	mov ebx,sizeof(CONTACTSSTRUCT)
	imul ebx,ecx
	add edi,ebx
	mov esi,edi ;edi此时保存的是当前要删除的结构体数组的首地址
	mov ebx,sizeof(CONTACTSSTRUCT)
	add esi,ebx ;esi指向要删除的结构体数组的下一个元素的首地址

	add ecx,1 ;因为要保存数组时，是从数组0开始的，所以加1用于计算
		  ;需要移动多少个元素，中间某一个元素被删除了，后面
		  ;的元素向前移动
	mov eax,g_nCount
	sub eax,ecx ;需要移动的次数
	mov ebx,sizeof(CONTACTSSTRUCT)	  
	imul ebx,eax ;计算需要移动的字节
	mov ecx,ebx
	rep movs BYTE ptr[edi],BYTE ptr[esi] ;开始移动（以一个字节的大小移动）
	
	;移动完成后删除最后一个结构体中的信息
	mov ebx,sizeof(CONTACTSSTRUCT)
	push ebx ;大小
	push 0 ;内容
	push edi ;删除的首地址
	call crt_memset  ;调用初始化函数
	add esp,12
	dec g_nCount
	
	;保存数据
	call SaveDate
	
	push offset g_szOK ;操作成功
	call crt_printf
	add esp,4
	
	;暂停
	push offset g_szPause
	call crt_system
	add esp,4
	
	ret
END_M:
	push offset g_szTipNoExist
	call crt_printf
	add esp,4
	;暂停
	push offset g_szPause
	call crt_system
	add esp,4
	ret

RemoveDate endp


;菜单
Menu proc
	
	;打印菜单
	push offset g_szMenu
	call crt_printf
	add esp,4 ;平衡堆栈
	
	ret

Menu endp

;菜单选项分支跳转
MenuJump proc
	
	;比较两个操作数，操作数1-操作数2
	cmp g_nChoose,0
	jz OP0
	
	cmp g_nChoose,1
	jz OP1
	
	cmp g_nChoose,2
	jz OP2
	
	cmp g_nChoose,3
	jz OP3
	
	cmp g_nChoose,4
	jz OP4
	
	cmp g_nChoose,5
	jz OP5
	
	;输入错误
	push offset g_szInputError
	call crt_printf
	add esp,4
	
	
	;暂停
	push offset g_szPause
	call crt_system
	add esp,4
	
	;INVOKE GetStdHandle,STD_INPUT_HANDLE;读取输入句柄，STD_INPUT_HANDLE是win32常数
	;mov consoleInHandle,eax ;保存
	;INVOKE FlushConsoleInputBuffer,consoleInHandle;清除输入（通常是键盘）缓冲函数式
	
	
	jmp EndMenu_Mark
	
	;退出
OP0:
	;exitProcess(0)
	push 0
	call ExitProcess
	add esp,4
	jmp EndMenu_Mark
	
	;查询
OP1:	
	call ListAll
	jmp EndMenu_Mark
	
	;添加
OP2:
	call ADD_USER
	jmp EndMenu_Mark
	
	;搜索
OP3:
	call FindData
	jmp EndMenu_Mark
	
	;修改
OP4:
	call ModifyData
	jmp EndMenu_Mark
	
	;删除
OP5:
	call RemoveDate
	jmp EndMenu_Mark
	
EndMenu_Mark:

	ret

MenuJump endp


;查看所有用户
ListAll proc
	
	xor ecx,ecx
	;1.对比当前容量
	cmp g_nCount,0
	je NODATE
	jne CYCLE_lIST
	
;无数据
NODATE:
	push offset g_szTipNoDate
	call crt_printf
	add esp,4
	jmp EndList_Mark
	
;列出所有
CYCLE_lIST:
	push ecx
	;2.1地址
	lea ebx,sizeof(CONTACTSSTRUCT)
	imul ebx,ecx
	add esi,ebx ;第ecx个结构体地址
	
	;printf("%s%s",a,b)
	lea eax,[esi+CONTACTSSTRUCT.szPhNumber]
	push eax
	lea eax,[esi+CONTACTSSTRUCT.szName]
	push eax
	push offset g_szScanFormat
	call crt_printf
	add esp,0ch
	
	push offset g_szZero
	call crt_printf
	add esp,4
	
	pop ecx
	inc ecx ;第一个联系人ecx是0，所以比较前需要加1
	cmp dword prt [g_nCount], ecx ;比较当前输出位置ecx  通讯录人数g_nCount
	jle EndList_Mark ;相等代表全部打印完，跳到查询结束
	
	;还有联系人没有打印
	;inc ecx ;前面已经加1了，这里不需要再加
	jmp CYCLE_lIST
	
;查询结束
EndList_Mark:
	;查询完成
	push offset g_szTipListOver
	call crt_printf
	add esp,4
	
	;暂停
	push offset g_szPause
	call crt_system
	add esp,4

	ret

ListAll endp


;保存
SaveDate proc
	
	push ebp
	mov ebp,esp
	
	;重置文件指针
	mov g_pFile,0
	
	;打开文件
	push offset g_szFileWB
	push offset g_szFile
	call crt_fopen
	add esp, 04h
	mov dword ptr [g_pFile],eax
	
;size_t fwrite(const void* buffer,size_t size, size_t count, FILE* stream);
 ;--buffer:每个数据块的指针
 ;--size:每个数据的大小，单位为Byte
 ;--count:数据个数
 ;--stream:文件指针
 
 	;存储联系人数量
 	push g_pFile ;文件指针
 	push 1 ;数据个数
 	push 4 ;每个数据的大小，这里是dword 4字节
 	lea ebx,[g_nCount] ;数据指针
 	push ebx
 	call crt_fwrite
 	add esp,10h
 	
 	;存储文件结构体数组
 	;文件指针
 	push g_pFile ;文件指针
 	
 	;数据个数
 	lea eax,g_nCount
 	push eax
 	
 	;数据大小
 	mov ebx,sizeof(CONTACTSSTRUCT)
 	push ebx
 	
 	;结构体数组首地址
 	lea esi,[g_stContacts]
 	push esi
 	
 	call crt_fwrite
 	add esp,10h
 	
 	;关闭文件
 	push g_pFile
 	call crt_fclose
 	add esp,4
 	mov dword ptr [g_pFile],0
 	
 	mov esp,ebp
 	pop ebp
 	
	ret

SaveDate endp


;数据加载
LoadDate proc
	
	push ebp
	mov ebp,esp
	
	;重置文件指针
	mov g_pFile,0
	
	;打开文件
	push offset g_szFileRB
	push offset g_szFile
	call crt_fopen
	add esp,08h
	
	mov g_pFile,eax ;打开文件返回值eax，eax为0打开失败，成功为1
	cmp g_pFile,0
	je End_Load ;打开失败，跳转
	
	;size_t fread(void* buffer,size_t size,size_t count,FILE *stream);
	;--buffer:指向数据块的指针
	;--size:每个数据的大小，单位为Byte
	;--count:数据个数
	;--stream:文件指针
	
	;读取联系人数量
	push g_pFile ;文件指针
	push 1
	push 4
	lea esi,[g_nCount] ;数据地址
	push esi
	call crt_fread
	add esp,10h
	
	;读取联系人结构体数组数据
	push g_pFile
	mov eax,g_nCount
	push eax
	mov ebx,sizeof(CONTACTSSTRUCT)
	push ebx
	lea esi,[g_stContacts]
	push esi
	call crt_fread
	add esp,10h
	
End_Load:
	;关闭文件
	push g_pFile
	call crt_fClose
	add esp,4
	
	mov esp,ebp
	pop ebp
	
	ret

LoadDate endp