#include <iostream>

#include "Json.h"

using JSON = wjfeng::json::Json;

void test1() {
  // 需要提供JSON的构造函数
  JSON v1;
  JSON v2{true};
  JSON v3{123};
  JSON v4{1.23};
  JSON v5{"hello world"};

  // 需要提供运类型转换运算符 converting operator 用于将类的对象转换为其他类型
  bool b = v2;
  int i = v3;
  double f = v4;
  // const std::string& s = v5;
}

// 测试数组
void test2() {
  JSON arr;
  arr[0] = true;
  arr[1] = 123;

  arr.append(1.23);
  arr.append("hello world");

  std::cout << arr.str() << std::endl;

  bool b = arr[0];
  int i = arr[1];
  double f = arr[2];

  // const std::string &s = arr[3];
}

// 测试对象
void test3() {
  JSON obj;
  obj["bool"] = true;
  obj["int"] = 123;
  obj["double"] = 1.23;
  obj["str"] = "hello world";

  std::cout << obj.str() << std::endl;
}

void test4() {
  JSON arr;
  arr[0] = true;
  arr[1] = 123;
  arr[2] = 1.23;
  arr[3] = "hello world";

  for (auto it = arr.begin(); it != arr.end(); ++it) {
    std::cout << (*it).str() << std::endl;
  }

  std::cout << arr.has(0) << std::endl;
  std::cout << arr.has(4) << std::endl;
}

int main() {
  test1();
  test2();
  test3();
  test4();

  std::cout << "test toyJSON" << std::endl;
  return 0;
}