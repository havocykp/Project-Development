.386 ;汇编语言的伪指令，在80386及以后的处理器中使用该指令集
.model flat, stdcall ;模式定义"model 内存模式[,调用模式]"
option casemap:none ;选项设定（设定伪对大小写敏感）

.code


;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
start:
;主函数
main proc
	
    push ebp ;备份ebp
    mov ebp,esp
    
    call LoadDate
    
CYCLE_MAIN:
    
    ;清屏
    push offset g_szCls
    call crt_system
    add esp,4
    ;显示菜单
    call Menu
    ;重置g_nChoose
    lea ebx,[g_numScanfFormat]
    mov dword ptr[g_nChoose],0h
    
    push offset g_nChoose ;输入整型格式控制符
    push ebx
    call crt_scanf
    add esp,8
    
    cmp eax,1
    jnz @T1
    
    ;菜单跳转
    call MenuJump
    
    ;循环
    jmp CYCLE_MAIN
    
@T1:
    call crt_getchar
    cmp eax,0ah
    
    jnz @T1
    
    jmp CYCLE_MAIN
    
MAIN_END:
    mov esp,ebp
    pop ebp	
    
    ret
main endp
end start
end


