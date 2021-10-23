# UniqueLab 2021 Autumn Assignment 1
c++练手
## Description

你需要实现两种C++ STL中的基本容器，set与priority queue。

方法定义如下：
- Set
    - bool empty() const;
    - size_type size() const;
    - void clear();
    - void insert(const value_type& value);
    - size_type erase(const value_type& value);
    - size_type count(const value_type& value);
- Priority Queue
    - const_reference top() const;
    - bool empty() const;
    - size_type size() const;
    - void push(const value_type& value);
    - void pop();

更多的信息可以在 [C++ reference](http://www.cplusplus.com/reference) 找到

你大可以放轻松，因为这只是用来提高你对C++语言的掌握程度，但是这里也有一些要求。

## Requirements

- 你被要求使用LLRB（左偏红黑树）作为 Set 的物理结构，斐波那契堆作为Priority Queue的物理结构。
- 一个正常的工程师应当明白如何检验自己的程序是否正常通过，所以你被要求自己生成足够强度的数据集来对你的数据结构进行检测。
- 使用 valgrind， memory sanitizer，address sanitizer 来确保你的程序不存在内存泄漏，越界访问，double free 等问题。

## Specifications

- 编程语言：你可以使用C++，C，汇编语言，机器码中的任意一种语言来实现题目的要求。
- 编程风格：**NO OI STYLE**，使用OI风格进行编程是非常不推荐的，请尽量使用贴近工程的风格编写，此外，良好的编程习惯会很有帮助，这里有更多信息 [Google Coding Style Guide](https://google.github.io/styleguide/cppguide.html#C++_Version)
    - 这意味着，你不应该开一个大数组当作内存池，或者不处理动态内存的释放：想象你的代码会作为与标准库类似的基础设施被很多项目使用。
- 其他：首先你需要明白自己在写什么，还要明白为什么这样写，这意味这你最好明白上述数据结构复杂度的证明~~，即使这不是必要的。~~

## Evaluation

- Basic:
    - 实现所有上述的数据结构与功能
    - 自己生成数据进行测试
- Advance:
    - 为Set实现iterator，并且为Set增添以下操作：
        - iterator find(const value_type& value);
        - iterator lower_bound(const value_type& value);
        - iterator upper_bound(const value_type& value);
    - 使用Template实现上述要求
    - 使用一种构建框架（如 CMake）
    - 使用一种单元测试框架（如 gtest, catch2）
    - 使你的数据结构线程安全(Thread-Safe)
    - **将正常的STL容器直接替换成你的实现后，可以正常运行**

## DDL
下次组会

## Reference
这里有你需要的更多信息，大概
- 问问 [Google](www.google.com)
- 也可以可以向学长~~学姐~~提问
- [dbg-macro](https://github.com/sharkdp/dbg-macro) *A macro for `printf`-style debugging fans.*
- [Learn C++ in Y miniutes](https://learnxinyminutes.com/docs/c++/)
- [Left-Leaning Red-Black Trees](https://www.cs.princeton.edu/~rs/talks/LLRB/RedBlack.pdf) - Robert Sedgewick Princeton University
- [Balanced Search Trees](https://algs4.cs.princeton.edu/lectures/33BalancedSearchTrees-2x2.pdf) -_Algorithms_Robert Sedgewick | Kevin Wayne
- *Introduction to Algorithms* [Thomas H. Cormen](https://en.wikipedia.org/wiki/Thomas_H._Cormen) [Charles E. Leiserson](https://en.wikipedia.org/wiki/Charles_E._Leiserson) [Ronald L. Rivest](https://en.wikipedia.org/wiki/Ron_Rivest) [Clifford Stein](https://en.wikipedia.org/wiki/Clifford_Stein)
- [Fibonacci Heaps](https://www.cl.cam.ac.uk/teaching/1415/Algorithms/fibonacci.pdf) University of Cambridge, Thomas Sauerwald
- [Fibonacci Heaps](https://www.cs.princeton.edu/~wayne/teaching/fibonacci-heap.pdf) Princeton University, Kevin Wayne
- [Priority Queue](https://en.wikipedia.org/wiki/Priority_queue)
- [GNU libstdc++](https://gcc.gnu.org/onlinedocs/libstdc++/)

## 提示
尽信书则不如无书，以上提到的参考资料中，有部分内容和代码是存在错误的，希望你能不被误导。

你**不应该**直接复制粘贴网上的代码。

