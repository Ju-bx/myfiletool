#include <iostream>
#include <vector>

int main()
{
    std::vector<int> v(5);          // 5 個元素：v[0] ~ v[4]

    for (int i = 0; i <= 5; i++)    // bug：<= 會跑到 i = 5
    {
        v[i] = i;
    }

    std::cout << "done\n";
    return 0;
}