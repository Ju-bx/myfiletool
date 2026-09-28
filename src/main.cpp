#include <iostream>

int main(int argc, char* argv[]) {
    // TODO 1: 印出 "argc = " 和 argc 的值
    std::cout << "argc = " << argc << '\n';

    // TODO 2: 用 for 迴圈，i 從 0 到 argc-1
    //         每一行印出 "argv[i] = " 和 argv[i] 的內容
    for (int i = 0 ; i < argc ; i++){
        std::cout << "argv["<< i <<"] = " << argv[i] <<'\n';
    }
       

    return 0;
}

