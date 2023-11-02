.CODE

; �ڲ����õ�movaps xmmwordָ��, ��ָ�����Ҫ��rsp��16λջ�����, �����Ҫ����rsp�����һλΪ0

InitLogAsm PROC
	call r9							; ��4�������Ǻ�����ַ
	ret
InitLogAsm ENDP

InitParentIpcAsm PROC
	sub rsp, 48h
	call rdx						; ��2�������Ǻ�����ַ
	add rsp, 48h
	ret
InitParentIpcAsm ENDP

LaunchChildProcessAsm PROC
	sub rsp, 48h
	mov rax, [rsp + 70h]			; [rsp + 48h + 28h]ԭ��5������
	mov [rsp + 20h], rax
	mov rax, [rsp + 78h]			; [rsp + 48h + 30h]ԭ��6������
	mov [rsp + 28h], rax
	mov rax, [rsp + 80h]			; [rsp + 48h + 38h]ԭ��7������ Ҳ����func_addr
	call rax
	add rsp, 48h
	ret
LaunchChildProcessAsm ENDP

ConnectedToChildProcessAsm PROC
	sub rsp, 48h
	call r8							; ��3�������Ǻ�����ַ
	add rsp, 48h
	ret
ConnectedToChildProcessAsm ENDP

SendIpcMessageAsm PROC
	sub rsp, 48h
	mov rax, [rsp + 70h]			; [rsp + 48h + 28h]ԭ��5������
	mov [rsp + 20h], rax
	mov rax, [rsp + 78h]			; [rsp + 48h + 30h]ԭ��6������
	mov [rsp + 28h], rax
	mov rax, [rsp + 80h]			; [rsp + 48h + 38h]ԭ��7������ Ҳ����func_addr
	call rax
	add rsp, 48h
	ret
SendIpcMessageAsm ENDP

TerminateChildProcessAsm PROC
	sub rsp, 48h
	mov rax, [rsp + 70h]			; [rsp + 48h + 28h]ԭ��5������ Ҳ����func_addr
	call rax
	add rsp, 48h
	ret
TerminateChildProcessAsm ENDP

ReLaunchChildProcessAsm PROC
	sub rsp, 48h
	call r8							; ��3�������Ǻ�����ַ
	add rsp, 48h
	ret
ReLaunchChildProcessAsm ENDP

InitChildIpcAsm PROC
	sub rsp, 48h
	call rdx						; ��2�������Ǻ�����ַ
	add rsp, 48h
	ret
InitChildIpcAsm ENDP

SetChildReceiveCallbackAndCOMPtrAsm PROC
	sub rsp, 48h
	call r9							; ��4�������Ǻ�����ַ
	add rsp, 48h
	ret
SetChildReceiveCallbackAndCOMPtrAsm ENDP

ChildSendIpcMessageAsm PROC
	sub rsp, 48h
	mov rax, [rsp + 70h]			; [rsp + 48h + 28h]ԭ��5������
	mov [rsp + 20h], rax
	mov rax, [rsp + 78h]			; [rsp + 48h + 30h]ԭ��6������ Ҳ����func_addr
	call rax
	add rsp, 48h
	ret
ChildSendIpcMessageAsm ENDP

END