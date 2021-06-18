//
// Created by yuanh on 2021/3/25.
//

#include <iostream>
#include <string>

using namespace std;

int main()
{
    char buffer[1024];
    gets(buffer);
    cout << buffer;

    return 0;
}