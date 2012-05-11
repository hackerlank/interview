.386
.model flat,stdcall
option casemap:none

include \masm32\include\windows.inc
include \masm32\include\user32.inc
include \masm32\include\kernel32.inc
includelib \masm32\lib\user32.lib
includelib \masm32\lib\kernel32.lib

DlgProc PROTO :DWORD,:DWORD,:DWORD,:DWORD
DoExit PROTO
load PROTO :DWORD,:DWORD,:LPVOID,:LPCVOID,:DWORD

.data
DlgName db "MyDialog",0
AppName db "Warcraft II BNE.exe",0
ProcessInfo PROCESS_INFORMATION <>
StartupInfo STARTUPINFO <0>
flOldProtect dd 0
widthbyte dd 500h
heightbyte dd 400h

unk_403074 db 0
.data?
hInstance HINSTANCE ?
CommandLine LPSTR ?
buffer db 512 dup(?)


.const
IDC_BUTTON1      equ 3001
IDC_BUTTON2      equ 3002
IDC_EXIT        equ 3003



.code
start:
	invoke GetModuleHandle, NULL
	mov    hInstance,eax
	invoke DialogBoxParam, hInstance, ADDR DlgName,NULL,addr DlgProc,NULL
	invoke ExitProcess,eax

DlgProc proc hWnd:HWND, uMsg:UINT, wParam:WPARAM, lParam:LPARAM
	.IF uMsg==WM_INITDIALOG
		invoke GetDlgItem, hWnd,IDC_BUTTON1
		invoke SetFocus,eax
	.ELSEIF uMsg==WM_CLOSE
		invoke PostQuitMessage,0
	.ELSEIF uMsg==WM_COMMAND
		mov eax,wParam
		mov edx,wParam
		shr edx,16
		.if dx==BN_CLICKED
			.IF ax==IDC_BUTTON1
				; creating war2bne proc
                .if ProcessInfo.hProcess!=0
                    invoke CloseHandle,ProcessInfo.hProcess
                    mov ProcessInfo.hProcess,0
                .endif
                invoke GetStartupInfo,ADDR StartupInfo
                invoke CloseHandle,ProcessInfo.hThread				
				invoke CreateProcess, ADDR AppName, NULL, NULL, NULL, FALSE, NORMAL_PRIORITY_CLASS, NULL, NULL, ADDR StartupInfo, ADDR ProcessInfo
				push 1
				push offset unk_403074
				push 428E17h
				push 0
				push 0
				call load
			.ELSEIF ax==IDC_BUTTON2
                .if ProcessInfo.hProcess!=0
                    invoke CloseHandle,ProcessInfo.hProcess
                    mov ProcessInfo.hProcess,0
                .endif
                invoke GetStartupInfo,ADDR StartupInfo
                invoke CloseHandle,ProcessInfo.hThread				
				invoke CreateProcess, ADDR AppName, NULL, NULL, NULL, FALSE, NORMAL_PRIORITY_CLASS, NULL, NULL, ADDR StartupInfo, ADDR ProcessInfo
				push 1
				push offset unk_403074
				push 428E17h
				push 0
				push 0
				call load
				push 1
				push offset heightbyte
				push 48C42Ch
				push 0
				push 0
				call load
				push 1
				push offset widthbyte
				push 48C431h
				push 0
				push 0
				call load					
            .ELSEIF ax==IDC_EXIT
				invoke PostQuitMessage,0
			.ENDIF
		.ENDIF
	.ELSE
		mov eax,FALSE
		ret
	.ENDIF
	mov eax,TRUE
	ret
DlgProc endp

DoExit proc
	invoke ExitProcess,0
	;invoke PostQuitMessage,0
DoExit endp


load proc a1:DWORD, a2:DWORD, lpBaseAddress: LPVOID, lpBuffer: LPCVOID, nSize: DWORD
	
	push offset flOldProtect
	push 40h
	push nSize
	push lpBaseAddress
	push ProcessInfo.hProcess
	call VirtualProtectEx
	
	push 0
	push nSize
	push lpBuffer
	push lpBaseAddress
	push ProcessInfo.hProcess
	call WriteProcessMemory
	
	leave
	retn 14h

load endp




end start

;PatchNoCd proc i1:DWORD, i2:DWORD, lpBaseAddress: LPVOID, lpBuffer: LPCVOID, nSize:SIZE_T

 
	


