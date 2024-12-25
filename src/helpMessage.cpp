// 本来时间紧迫不想写help 的
// 但毕竟是我花了许多日夜写出来的程序
// 如果没有 help 究竟不算完整
const char *HelpMessage = R"(
    长参数和短参数都用一个[032m'-'[0m来表示
    如果是[33m-argu[0m的话, 那短参数就是[33m-a[0m
    以下就不展示短参数了

    -help     获取你现在正在看的帮助

    -random   随机生成数据
              用法:
                   [33msortui -random <int> <int> <int>[0m
              用例:
                   [32sortui -random 0 99 40  生成 40 个 0 ~ 99 的随机数据[0m
    
    -file     文件生成数据
              用法:
                   [33msortui -file <string>[0m
              用例:
                   [32sortui -file [33mtest.txt[0m  用[33mtest.txt[0m来生成数据
              [31m注意:[0m
                   我是直接用[34ifstream[0m读取的, 所以第一个数据是数据长度
                   数字与数字之间只能用空白符分隔

    -no-ui    不使用Tui来展示排序过程, 适合在大量数据的情况下使用

以下选项只在no-ui模式下生效

    -speed    控制排序速度
              用法:
                   [33msortui -speed <float> [0m

    -method   使用的排序方法
              用法:
                  [33msortui -method <string> [0m
              可用方法:
              [34mBubble[0m ---- [33m冒泡排序
              [34mCock[0m ---- [33m鸡尾酒排序
              [34mCompose[0m --- [33m复合排序
              [34mInsert[0m ---- [33m插入排序
              [34mSelect[0m ---- [33m选择排序
              [34mQuick[0m ----- [33m快速排序
              [34mMerge[0m ----- [33m归并排序
              [34mHeap[0m -------- [33m堆排序
              [34mShell[0m ----- [33m希儿排序
              [34mBucket[0m ------ [33m桶排序
              [34mRadix[0m ----- [33m基数排序[0m
)";
