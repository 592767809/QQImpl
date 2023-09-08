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


; ����ΪMMMojo��ʵ��
OnReadPushTransfer PROC
	mov rax, [r8]    
	mov r9, r8                        
	mov r8, rdx                       
	mov edx, ecx                      
	mov rcx, r9                       
	jmp qword ptr [rax + 8h]
OnReadPushTransfer ENDP

OnReserved2Transfer PROC
	mov rax, [r8]
	mov r9, r8                
	mov r8, rdx               
	mov edx, ecx              
	mov rcx, r9               
	jmp qword ptr [rax + 10h]
OnReserved2Transfer ENDP

OnReserved3Transfer PROC
	mov rax, [r8]
	mov r9, r8
	mov r8, rdx
	mov edx, ecx
	mov rcx, r9
	jmp qword ptr [rax + 18h]
OnReserved3Transfer ENDP

OnRemoteConnectTransfer PROC
	mov rax, [rdx]
	mov r8, rdx
	movzx edx, cl
	mov rcx, r8
	jmp qword ptr [rax + 20h]
OnRemoteConnectTransfer ENDP

OnRemoteDisconnectTransfer PROC
	mov rax, [rcx]
	jmp qword ptr [rax + 28h]
OnRemoteDisconnectTransfer ENDP

OnRemoteProcessLaunchedTransfer PROC
	mov rax, [rcx]
	jmp qword ptr [rax + 30h]
OnRemoteProcessLaunchedTransfer ENDP

OnRemoteProcessLaunchFailedTransfer PROC
	mov rax, [rdx]
	mov r8, rdx
	mov edx, ecx
	mov rcx, r8
	jmp qword ptr [rax + 38h]
OnRemoteProcessLaunchFailedTransfer ENDP

OnRemoteMojoErrorTransfer PROC
	mov rax, [r8]
	mov r9, r8
	mov r8d, edx
	mov rdx, rcx
	mov rcx, r9
	jmp qword ptr [rax + 40h]
OnRemoteMojoErrorTransfer ENDP

END