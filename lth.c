#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
unsigned char shellCode[]={
    0x50,                   
    0x48, 0xC7, 0xC0, 0x03, 0x00, 0x00, 0x00,
    0x48, 0x83, 0xC0, 0x04,       
    0x58,                        
    0xC3                         
};
size_t shellCodeSize=sizeof(shellCode);
void nothing(){
    printf("i am doing nothing\n");
    return;
}
int main(){
//creating a thread in suspended state
DWORD dwTheadId;
HANDLE hThread=CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)nothing,NULL,CREATE_SUSPENDED,&dwTheadId);
if(hThread==NULL){
printf("could not create thread in suspended state\n");
return 1;
}
printf("suspende thread ID:%d\n",dwTheadId);
//allocate shellCode
LPVOID reservedMem=VirtualAlloc(NULL,shellCodeSize,MEM_COMMIT | MEM_RESERVE,PAGE_EXECUTE_READWRITE);
if(reservedMem==NULL){
    printf("could not allocate memory\n");
    return 1;
}
RtlMoveMemory(reservedMem,shellCode,shellCodeSize);
//get EIP
CONTEXT threadContext={0};
threadContext.ContextFlags=CONTEXT_FULL;
BOOL bThreadContextResult=GetThreadContext(hThread,&threadContext);
if(bThreadContextResult==0){
    printf("could not get thread context\n");
    return 1;
}
printf("EIP:[0x%p]\n",(PVOID *)threadContext.Eip);
//change EIP to shellCode address
threadContext.Eip=(DWORD32) reservedMem;
if(SetThreadContext(hThread,&threadContext)==0){
    printf("could not modify the thred structure\n");
};
//resume thread execution
printf("Hijacking thread\n");
DWORD dwResumeThreadResult=ResumeThread(hThread);
if(dwResumeThreadResult==(DWORD) -1){
    printf("could not resume the thraed\n");
    return 1;
}
WaitForSingleObject(hThread,INFINITE);
//free the allocated shellCode buffer
BOOL bVirtualFree=VirtualFree(reservedMem,shellCodeSize,MEM_DECOMMIT);
if(bVirtualFree==0){
    printf("could not free shellCode buffer\n");
    return 1;
}
printf("shellcode buffer was freed\n");
//exit
return 0;
}