# 基于跳表实现的轻量级键值型存储引擎

## 功能
- insert(K, V) - 插入键值对(K, V)
- del(K) - 删除键为K的数据
- serach(K) - 查找键为K的数据
- print() - 打印跳表的目前存储的内容
- dump_file(const char* filename) - 将跳表存储的所有键值对存储到filename中, 在文件前两行会写入K和V的类型
- load_file(const char* filename) - 将filename中的数据恢复到跳表中, 如果当前跳表存储的键值对类型与文件中存储的不同就不会执行恢复操作

## 性能

跳表的高度为18的情况下(压力测试时关闭cout输出):

### 插入

| 数据规模(万) | 执行时间(秒) |
| ------------ | ------------ |
| 10           | 0.057124     |
| 50           | 0.312799     |
| 100          | 0.848773     |

QPS为117.8w

### 查找

| 数据规模(万) | 执行时间(秒) |
| ------------ | ------------ |
| 10           | 0.040515     |
| 50           | 0.336805     |
| 100          | 1.0396       |

QPS为96.2w

## 构建项目
### 需要的工具
- [GCC](http://gcc.gnu.org/)
- [CMake](cmake.org)
- [Boost](boost.org)
- [make](http://www.gnu.org/software/make/)

### 构建流程
```bash
git clone https://github.com/AhDaii/SkipList.git
cd SkipList
cmake .
cmake --build .
./main or ./stresstest
```

本项目使用[heapusage](https://github.com/d99kris/heapusage)检测内存泄漏
