#ifndef TOY_JSON_
#define TOY_JSON_

#include <map>
#include <string>
#include <vector>

namespace wjfeng {
namespace json {
class Json {
 public:
  // 用枚举类型来定义json的数据类型
  enum Type {
    json_null = 0,
    json_bool,
    json_int,
    json_double,
    json_string,
    json_array,  // 数组
    json_object  // 对象
  };

 private:
  // 联合体的内存占用是由最大的成员决定的，这里是double或指针
  union Value {
    bool m_bool;
    int m_int;
    double m_double;
    std::string *m_string;
    std::vector<Json> *m_array;
    std::map<std::string, Json> *m_object;  // JSON的键只能是string
  };

 private:
  Type m_type;
  Value m_value;

 public:
  Json();
  Json(bool value);
  Json(int value);
  Json(double value);
  Json(const char *value);
  Json(const std::string &value);

  Json(Type type);
  Json(const Json &other);

  void operator=(const Json &other);

  void copy(const Json &other);
  void clear();

 public:
  operator bool();
  operator int();
  operator double();
  // operator string();

 public:
  Json &operator[](int index);
  void append(const Json &other);

  Json &operator[](const std::string &key);
  Json &operator[](const char *key);

  std::string str() const;
};
}  // namespace json
}  // namespace wjfeng

#endif  // TOY_JSON_