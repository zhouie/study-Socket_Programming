## study-Socket_Programming

Windows 环境下基于TCP/IP套接字编程(Socket Programming)

### Environment && Platform

| Enviro & Plat | Intro |
| :-: | :-: |
| System | Windows 10 |
| Language | C++ 11 |
| IDE | Code::Blocks 16.01 |

---

### Execute

* 开启服务端(server)，处于等待连接请求状态
* 开启客户端(client)，连接成功后，发送指令
* 服务端接收到指令后，给客户端回复
* 客户端显示所收到的内容

这个demo中的 “指令” 就是指 “**daytime**”，以获取服务器返回时间值。

---

### Q&A

#### 头文件stdafx.h的解决方法

将下面这段代码复制一下到新建的`stdafx.h`文件中，路径：`D:\Program Files\codeblocks_new\CodeBlocks\MinGW\include\stdafx.h`

```
// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently

#if !defined(AFX_STDAFX_H__4607A810_33E2_483D_80D8_BE41F0D473D5__INCLUDED_)
#define AFX_STDAFX_H__4607A810_33E2_483D_80D8_BE41F0D473D5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// Insert your headers here
#define WIN32_LEAN_AND_MEAN        // Exclude rarely-used stuff from Windows headers
#define DLLEXPORT __declspec(dllexport)
#define    DLLIMPORT __declspec(dllimport)

#include "stdio.h"
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib,"ws2_32.lib")

// TODO: reference additional headers your program requires here
void Msg(char *szFormat, ...);
void dbMsg(char *szFormat, ...);
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__4607A810_33E2_483D_80D8_BE41F0D473D5__INCLUDED_)
```

#### 报错 undefined reference to 'WSAStartup@8'

Codeblocks MinGW 提供了一套简单方便的Windows下的基于GCC 程序开发环境。MinGW是完全免费的自由软件，收集了一系列免费的Windows 使用的头文件和库文件；同时整合了GNU ( http://www.gnu.org/ )的工具集，特别是GNU 程序开发工具，如经典gcc, g++, make等。它在Windows平台上模拟了Linux下GCC的开发环境，为C++的跨平台开发提供了良好基础支持，为了在Windows下工作的程序员熟悉Linux下的C++工程组织提供了条件。

Windows 下C++ Socket编程，可能会调用`WSAStartup`函数报错：`undefined reference to 'WSAStartup@8'`，这是因为 **缺少socket的链接参数**，解决方案便是把 **-lwsock32** 加到链接器中，路径：`Setting -> Compiler setting -> Global compiler setting -> Linker setting -> Other linker options`。

![](http://pco46wcft.bkt.clouddn.com/zhouie/study-Socket_Programming/1.png)

#### CodeBlocks中文乱码问题

在了解怎么解决之前，先要去搞清楚其中三个涉及到编码问题的地方

**1、CodeBlocks编辑器保存源文件用的编码**

默认情况下，CodeBlocks是保存为windows本地编码格式的，也就是`WINDOWS-936(GBK)`编码。但不幸的是，GCC编译器默认编译的时候是按照`UTF-8`解析的。你存成`GBK`，但是当成`UTF-8`解析，这怎么会编译通过呢，所以如果这两个地方编码不统一好，编译的时候就会报错：`error: converting to execution character set: Illegal byte sequence`！

其实要真正解决这个问题也很简单，编写CodeBlocks的人只需要在调用编译器之前检测一下源文件是什么编码，然后就自动让编译器用什么编码进行解析，问题就解决了。只是很可惜，他们还没有这么做，所以在易用性便不如微软了，免费和商业的东西还是有一定差距的。

**2、GCC编译器编译的时候对输入的源文件解释用的编码**

这个在编译器中可以通过设置`-finput-charset=charset`来指定编译器用什么编码解释输入源文件。如果源文件的字符集是`GBk`,那么就必须指定`-finput-charset=GBK`,如果不指定，一律默认当做`UTF-8`处理。这个时候，除非你源文件真的是`UTF-8`，否则就会出现转换错误。

**3、编译好的执行文件所用编码**

如果你 **1** 和 **2** 两个地方的编码都能统一，那么编译时也就不会报错了。But，这个时候运行一下看看，如果在控制台显示的依然是乱码，那么就是一步的问题的了。那是因为啊，控制台显示的时候缺省的是使用系统默认的字符集，比如windows下用的是`GBk`，而在默认情况下，编译之后的执行文件时编译成`UTF-8`的，这又出现了不统一，乱码由此而生！

解决的方法也很简单，就是给编译器加上选项：`-fexec-charset=GBK`，和windows默认的统一就OK了。

**综合以上**，搞懂了乱码产生的原因，那么也就不难得出结论，如何修改，你想修改成什么都OK，关键是要统一。

##### 修改操作

* 修改源文件保存编码格式
    - 路径：`settings->Editor->gernal settings->encoding setting`
    ![](http://pco46wcft.bkt.clouddn.com/zhouie/study-Socket_Programming/3.png)

* 修改编译器对源文件解释编码格式和生成执行文件执行时候采用的编码格式
    - 路径：`settings->compiler and debugger settings->global compiler setting->Other compiler options`
    ```
    -finput-charset=UTF-8
    -fexec-charset=GBK
    ```
    ![](http://pco46wcft.bkt.clouddn.com/zhouie/study-Socket_Programming/4.png)

---

### Reference

[TCP IP网络编程基础教程（Chap1-3）](https://github.com/zhouie/study-Socket_Programming/Tutorial)

[C++中Socket编程（入门）](https://www.cnblogs.com/kefeiGame/p/7246942.html)

[Windows环境下用C++实现Socket编程](https://blog.csdn.net/xiaoquantouer/article/details/58001960)



