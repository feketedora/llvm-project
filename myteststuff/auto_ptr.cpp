//
// Created by dfekete on 2024.03.15..
//


#include <iostream>
#include <memory>
#include <utility>

static void print (std::unique_ptr<int> ptr)
{
  std::cout << (ptr.get() ? *ptr : -1) << "\n";
}

int main (int argc, char** argv)
{
  std::unique_ptr<int> ptr(new int(42));
  print(std::move(ptr));
  return 0;
}
