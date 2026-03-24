this program aims to:(this file can be considered malicious)
Create a thread in a suspended state
Allocate executable memory
Copy shellcode into the allocated memory
Retrieve the thread’s register context
Modify the instruction pointer (EIP/RIP) to point to the shellcode
Apply the updated context to the thread
Resume the thread
The thread executes the shellcode
Shellcode finishes execution and the thread exits
