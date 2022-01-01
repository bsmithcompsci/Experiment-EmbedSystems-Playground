# Embed Systems
This git repository essentially exposes a bunch of standard library use cases of the various languages; and proceeds to execute various algorithms, testing their performances between them. Giving the RAM usage and the execution time.

Some languages will have more than one output for their benchmark, that is due to testing more than one version. They will appear in the same order as in the `Supported Languages` section.

The bias that this project will take is that we completely rely on the language's interpreter to handle the necessary optimizations for the algorithms. Meaning that if the language has a standard library that can be pulled into the language; then it will be used, otherwise the code will be manually written in the scope of the language.
Additionally each language test case will not use clever memory tricks to bring more performance out of them.
# Supported Languages
| Language      | Environment                   | Native Runtime        | Just-In-Time (JIT)    | Debugger Support  | Strongly Typed    | Allocator | GC Managed            | Platforms                                                 |
| ---           | ---                           | ---                   | ---                   | ---               | ---               | ---       | ---                   | ---                                                       |
| Cpp           | Statically MSVC (x64/O3)      | Yes                   | No                    | N/A               | Yes               | Exposed   | Self-Managed(sgen)    | Any Modern Device                                         |
| CSharp        | Mono(2.0)                     | Yes                   | No                    | N/A               | Yes               | Exposed   | Self-Managed(sgen)    | [Windows, Linux, MacOS, FreeBSD, XboxOne, Xbox Series X, Xbox Series S, PlayStation 4, Android, iOS](https://www.mono-project.com/docs/about-mono/supported-platforms/)                                                       |
| Python        | pybind(11)                    | Yes w/ External IPC   | No                    | N/A               | User-Choice       | Contained | Self-Managed          | [Any Modern Device](https://pybind11.readthedocs.io/en/stable/#supported-compilers)                                                       |
| Lua           | 5.4.3                         | Yes                   | No                    | N/A               | No                | Exposed   | Self-Managed          | [Windows, Linux, MacOS, FreeBSD, Microsoft Consoles, Sony Consoles, Nintendo Consoles, Android, iOS](http://lua-users.org/wiki/LuaDistributions)                                                       |
| LuaJIT        | 2.0.5                         | Yes                   | Yes                   | N/A               | No                | Exposed   | Self-Managed          | [Windows, Linux, MacOS, FreeBSD, Microsoft Consoles, Sony Consoles, Nintendo Consoles, Android, iOS](http://lua-users.org/wiki/LuaDistributions)                                                       |
| Javascript    | V8                            | Yes                   | N/A                   | N/A               | No                | Exposed   | Self-Managed          | Any Modern Device                                                       |

Soon supporting:
| Language      | Environment                   | Native Runtime        | Just-In-Time (JIT)    | Debugger Support  | Strongly Typed    | Allocator | GC Managed            |
| ---           | ---                           | ---                   | ---                   | ---               | ---               | ---       | ---                   |
| Julia         | 1.7.1                         | N/A                   | N/A                   | N/A               | User-Choice       | N/A       | N/A                   |
| Java          | N/A                           | Yes w/ VM             | Yes                   | N/A               | Yes               | N/A       | N/A                   |

# Benchmarks
## Array Algorithms
[Algorithms](https://www.geeksforgeeks.org/fundamentals-of-algorithms/)
|                       | Cpp   | Csharp        | Javascript    | Lua   | LuaJIT        | Python        |
| ---           | ---           | ---           | ---           | ---           | ---           | ---           |
 Linear Search | 46.8ms/3.25MBs | 83.6ms/13.02MBs | 59.6ms/15.44MBs | 44.6ms/3.38MBs | 45.6ms/3.29MBs | 77.2ms/10.16MBs
 Binary Search | 59.0ms/3.30MBs | 71.4ms/13.19MBs | 57.4ms/18.05MBs | 45.5ms/3.28MBs | 55.1ms/3.29MBs | 77.8ms/10.17MBs
 Jump Search | 86.4ms/3.24MBs | 74.8ms/13.33MBs | 58.8ms/14.87MBs | 43.8ms/3.41MBs | 52.6ms/3.41MBs | 82.9ms/10.21MBs
 Interpolation Search | 58.4ms/3.25MBs | 73.4ms/13.24MBs | 62.3ms/17.63MBs | 43.5ms/3.28MBs | 46.1ms/3.37MBs | 91.5ms/10.29MBs
 Exponential Search | 42.3ms/3.25MBs | 73.2ms/13.33MBs | 56.2ms/12.60MBs | 41.5ms/3.38MBs | 44.2ms/3.34MBs | 85.8ms/9.97MBs
 Selection Sort | 43.9ms/3.31MBs | 121.0ms/16.83MBs | 63.9ms/10.82MBs | 55.7ms/3.34MBs | 45.1ms/3.42MBs | 78.2ms/9.91MBs
 Bubble Sort | 44.3ms/3.25MBs | 120.6ms/16.97MBs | 62.5ms/17.29MBs | 49.3ms/3.28MBs | 42.2ms/3.45MBs | 76.6ms/10.09MBs
 Insertion Sort | 40.0ms/3.41MBs | 122.5ms/16.94MBs | 57.1ms/15.43MBs | 48.3ms/3.39MBs | 45.7ms/3.47MBs | 93.5ms/10.01MBs
 Quick Sort | 43.6ms/3.27MBs | 121.7ms/16.99MBs | 62.4ms/15.08MBs | 50.7ms/3.38MBs | 42.7ms/3.44MBs | 80.0ms/9.92MBs
 Radix Sort | 42.8ms/3.31MBs | 120.6ms/16.84MBs | 66.1ms/21.99MBs | 55.2ms/3.49MBs | 43.8ms/3.33MBs | 105.1ms/10.60MBs
## Tree Algorithms
Soon coming...
<!-- |                       | C++       | CPython   | CSharp    | Lua       | Julia | Javascript    | Typescript    |
| ---                   | ---       | ---       | ---       | ---   | ---    | ---           | ---        |
| General Tree Insertion    | 0.00ns/0b | 0.00ns/0b | 0.00ns/0b | 0.00ns/0b | 0.00ns/0b |
| General Tree Deletion     | 0.00ns/0b | 0.00ns/0b | 0.00ns/0b | 0.00ns/0b | 0.00ns/0b |
| General Tree Search       | 0.00ns/0b | 0.00ns/0b | 0.00ns/0b | 0.00ns/0b | 0.00ns/0b |
| ---                       | ---       | ---       | ---       | ---   | ---    | ---           | ---        |
| AVL Tree Insertion        | 0.00ns/0b | 0.00ns/0b | 0.00ns/0b | 0.00ns/0b | 0.00ns/0b |
| AVL Tree Deletion         | 0.00ns/0b | 0.00ns/0b | 0.00ns/0b | 0.00ns/0b | 0.00ns/0b |
| AVL Tree Search           | 0.00ns/0b | 0.00ns/0b | 0.00ns/0b | 0.00ns/0b | 0.00ns/0b |
| ---                       | ---       | ---       | ---       | ---   | ---    | ---           | ---        |
| Binary Tree Insertion     | 0.00ns/0b | 0.00ns/0b | 0.00ns/0b | 0.00ns/0b | 0.00ns/0b |
| Binary Tree Deletion      | 0.00ns/0b | 0.00ns/0b | 0.00ns/0b | 0.00ns/0b | 0.00ns/0b |
| Binary Tree Search        | 0.00ns/0b | 0.00ns/0b | 0.00ns/0b | 0.00ns/0b | 0.00ns/0b |
| ---                       | ---       | ---       | ---       | ---   | ---    | ---           | ---        |
| B-Tree Tree Insertion     | 0.00ns/0b | 0.00ns/0b | 0.00ns/0b | 0.00ns/0b | 0.00ns/0b |
| B-Tree Tree Deletion      | 0.00ns/0b | 0.00ns/0b | 0.00ns/0b | 0.00ns/0b | 0.00ns/0b |
| B-Tree Tree Search        | 0.00ns/0b | 0.00ns/0b | 0.00ns/0b | 0.00ns/0b | 0.00ns/0b |
| ---                       | ---       | ---       | ---       | ---   | ---    | ---           | ---        |
| Red-Black Tree Insertion  | 0.00ns/0b | 0.00ns/0b | 0.00ns/0b | 0.00ns/0b | 0.00ns/0b |
| Red-Black Tree Deletion   | 0.00ns/0b | 0.00ns/0b | 0.00ns/0b | 0.00ns/0b | 0.00ns/0b |
| Red-Black Tree Search     | 0.00ns/0b | 0.00ns/0b | 0.00ns/0b | 0.00ns/0b | 0.00ns/0b |
| ---                       | ---       | ---       | ---       | ---   | ---    | ---           | ---        |
| Splay Tree Insertion      | 0.00ns/0b | 0.00ns/0b | 0.00ns/0b | 0.00ns/0b | 0.00ns/0b |
| Splay Tree Deletion       | 0.00ns/0b | 0.00ns/0b | 0.00ns/0b | 0.00ns/0b | 0.00ns/0b |
| Splay Tree Search         | 0.00ns/0b | 0.00ns/0b | 0.00ns/0b | 0.00ns/0b | 0.00ns/0b |
| ---                       | ---       | ---       | ---       | ---   | ---    | ---           | ---        |
| Treap Tree Insertion      | 0.00ns/0b | 0.00ns/0b | 0.00ns/0b | 0.00ns/0b | 0.00ns/0b |
| Treap Tree Deletion       | 0.00ns/0b | 0.00ns/0b | 0.00ns/0b | 0.00ns/0b | 0.00ns/0b |
| Treap Tree Search         | 0.00ns/0b | 0.00ns/0b | 0.00ns/0b | 0.00ns/0b | 0.00ns/0b | -->
## Hashmaps Algorithms
Soon coming...
<!-- |                           | C++       | CPython   | CSharp    | Lua   | Julia | Javascript    | Typescript  |
| ---                       | ---       | ---       | ---       | ---   | ---    | ---           | ---        | -->

 ## Mathematical Algorithms
 Soon coming...
<!-- [Mathematical Algorithms](https://www.geeksforgeeks.org/mathematical-algorithms/)
|                           | C++       | CPython   | CSharp    | Lua   | Julia | Javascript    | Typescript  |
| ---                       | ---       | ---       | ---       | ---   | ---    | ---           | ---        | -->

# Benchmarking Specs
| Environment           | (v)CPUs   | RAM   | CPU Hardware      | Operating System  |
| ---                   | ---       | ---   | ---               | ---               |
| Personal Machine      | 6         | 16GB  | I5-8600K          | Windows 10 Pro    |
<!-- | AWS EC2 (t2.micro)    | 2         | 1GB   | N/A          | Ubuntu 18.04      | -->
Soon testing on a EC2...