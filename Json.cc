#include "Json.h"

#include <sstream>
#include <stdexcept>
#include <cstdlib>

using JSON = wjfeng::json::Json;

JSON::Json() : m_type(json_null) {}

// TODO: why not?
// JSON::Json(bool value) : m_type(json_bool), m_value.m_bool(value) {}
JSON::Json(bool value) : m_type(json_bool) { m_value.m_bool = value; }

JSON::Json(int value) : m_type(json_int) { m_value.m_int = value; }

JSON::Json(double value) : m_type(json_double) { m_value.m_double = value; }

JSON::Json(const char *value) : m_type(json_string) {
  m_value.m_string = new std::string(value);
}

JSON::Json(const std::string &value) : m_type(json_string) {
  m_value.m_string = new std::string(value);
}

JSON::Json(Type type) : m_type(type) {
  switch (m_type) {
    case json_null:
      break;
    case json_bool:
      m_value.m_bool = false;
      break;
    case json_int:
      m_value.m_int = 0;
      break;
    case json_double:
      m_value.m_double = 0.0;
      break;
    case json_string:
      m_value.m_string = new std::string("");
      break;
    case json_array:
      m_value.m_array = new std::vector<JSON>();
      break;
    case json_object:
      m_value.m_object = new std::map<std::string, JSON>();
    default:
      break;
  }
}

JSON::Json(const Json &other) { copy(other); }

void JSON::operator=(const Json &other) {
  clear();  // 先清理现有的内容，避免内存泄漏
  copy(other);
}

bool JSON::operator==(const Json &other) {
  if (m_type != other.m_type) {
    return false;
  }

  switch (m_type)
  {
  case json_null:
    return true;
    break;
  case json_bool:
    return m_value.m_bool == other.m_value.m_bool;
    break;
  case json_int:
    return m_value.m_int == other.m_value.m_int;
    break;
  case json_double:
    return std::abs(m_value.m_double - other.m_value.m_double) < EPSILON;
    break;
  case json_string:
    return *(m_value.m_string) == *(other.m_value.m_string);
    break;
  case json_array:
    // TODO: 这里严格来说应该要递归的检查内部的数据，为了简便起见就直接判断指针了
    return m_value.m_array == other.m_value.m_array;
    break;
  case json_object:
    // TODO: 这里严格来说应该要递归的检查内部的数据，为了简便起见就直接判断指针了
    return m_value.m_object == other.m_value.m_object;
    break;
  default:
    break;
  }
  return false;
}

bool JSON::operator!=(const Json &other) {
  return !(*this == other);
}

void JSON::copy(const Json &other) {
  m_type = other.m_type;
  switch (m_type) {
    case json_null:
      break;
    case json_bool:
      m_value.m_bool = other.m_value.m_bool;
      break;
    case json_int:
      m_value.m_int = other.m_value.m_int;
      break;
    case json_double:
      m_value.m_double = other.m_value.m_double;
      break;
    case json_string:
      // TODO: 这里为了方便起见用了浅拷贝，问题很大，之后要改成深拷贝
      m_value.m_string = other.m_value.m_string;
      break;
    case json_array:
      // TODO: 浅拷贝
      m_value.m_array = other.m_value.m_array;
      break;
    case json_object:
      // TODO: 浅拷贝
      m_value.m_object = other.m_value.m_object;
    default:
      break;
  }
}

void JSON::clear() {
  switch (m_type) {
    case json_null:
      break;
    case json_bool:
      m_value.m_bool = false;
      break;
    case json_int:
      m_value.m_int = 0;
      break;
    case json_double:
      m_value.m_double = 0.0;
      break;
    case json_string:
      delete m_value.m_string;
      break;
    case json_array:
      for (auto it = (m_value.m_array)->begin(); it != (m_value.m_array)->end();
           ++it) {
        it->clear();
      }
      delete m_value.m_array;
      break;
    case json_object:
      for (auto it = (m_value.m_object)->begin();
           it != (m_value.m_object)->end(); ++it) {
        (it->second).clear();
      }
      delete m_value.m_object;
      break;
    default:
      break;
  }
  m_type = json_null;
}

JSON::operator bool() {
  if (m_type != json_bool) {
    throw new std::logic_error("type erorr, not bool value.");
  }
  return m_value.m_bool;
}

JSON::operator int() {
  if (m_type != json_int) {
    throw new std::logic_error("type erorr, not int value.");
  }
  return m_value.m_int;
}

JSON::operator double() {
  if (m_type != json_double) {
    throw new std::logic_error("type erorr, not double value.");
  }
  return m_value.m_double;
}

// JSON::operator string() {
//   if (m_type != json_string) {
//     throw new std::logic_error("type erorr, not string value.");
//   }
//   return *(m_value.m_string);
// }

JSON &JSON::operator[](int index) {
  if (m_type != json_array) {
    clear();
    m_type = json_array;
    m_value.m_array = new std::vector<JSON>();
  }
  if (index < 0) {
    throw new std::logic_error("array[] index < 0");
  }

  int size = (m_value.m_array)->size();
  // 扩容
  if (index >= size) {
    for (int i = size; i <= index; ++i) {
      (m_value.m_array)->push_back(JSON());
    }
  }
  return (m_value.m_array)->at(index);
}

void JSON::append(const JSON &other) {
  if (m_type != json_array) {
    m_type = json_array;
    m_value.m_array = new std::vector<JSON>();
  }
  (m_value.m_array)->push_back(other);
}

JSON &JSON::operator[](const std::string &key) {
  if (m_type != json_object) {
    clear();
    m_type = json_object;
    m_value.m_object = new std::map<std::string, JSON>();
  }
  return (*(m_value.m_object))[key];
}

JSON &JSON::operator[](const char *key) {
  std::string name(key);
  return (*(this))[name];  // 复用
}

std::string JSON::str() const {
  std::stringstream ss;
  switch (m_type) {
    case json_null:
      ss << "null";
      break;
    case json_bool:
      if (m_value.m_bool) {
        ss << "true";
      } else {
        ss << "false";
      }
      break;
    case json_int:
      ss << m_value.m_int;
      break;
    case json_double:
      ss << m_value.m_double;
      break;
    case json_string:
      ss << '\"' << *(m_value.m_string) << '\"';
      break;
    case json_array:
      ss << '[';
      for (auto it = (m_value.m_array)->begin(); it != (m_value.m_array)->end();
           ++it) {
        if (it != (m_value.m_array)->begin()) {
          ss << ',';
        }
        ss << it->str();
      }
      ss << ']';
      break;
    case json_object:
      ss << '{';
      for (auto it = (m_value.m_object)->begin();
           it != (m_value.m_object)->end(); ++it) {
        if (it != (m_value.m_object)->begin()) {
          ss << ',';
        }
        ss << '\"' << it->first << '\"' << ':' << it->second.str();
      }
      ss << '}';
      break;
    default:
      break;
  }

  return ss.str();
}

bool JSON::asBool() const {
  if (m_type != json_bool) {
    throw new std::logic_error("type error, not bool value");
  }
  return m_value.m_bool;
}

int JSON::asInt() const {
   if (m_type != json_int) {
    throw new std::logic_error("type error, not int value");
  }
  return m_value.m_int;
}

double JSON::asDouble() const {
   if (m_type != json_double) {
    throw new std::logic_error("type error, not double value");
  }
  return m_value.m_double;
}

// TODO: 返回值类型需要优化
std::string SON::asString() const {
   if (m_type != json_string) {
    throw new std::logic_error("type error, not string value");
  }
  return *(m_value.m_string);
}

bool JSON::has(int index) {
  if (m_type != json_array) {
    return false;
  }
  int size = (m_value.m_array)->size();
  return (index >= 0 && index < size);
}

bool JSON::has(const std::string& key) {
   if (m_type != json_object) {
    return false;
  }
  return (m_value.m_object)->find(key) != (m_value.m_object)->end(); 
}

bool JSON::has(const char* key) {
  std::string name(key);
  return has(name);
}

void JSON::remove(int index) {
  if(m_type != json_array) {
    return;
  }
  int size = (m_value.m_array)->size();
  if (index < 0 || index >= size) {
    return;
  (m_value.m_aaray)->at(index).clear();
  (m_value.m_aaray)->erase((m_value.m_array)->begin() + index);
}

void JSON::remove(const std::string &key) {
  auto it = m_value.m_object->find(key);
  if (it == (m_value.m_object)->end()) {
    return;
  }
  (*(m_value.m_object))->at(key).clear();
  (m_value.m_object)->erase(key); 
}

void JSON::remove(const char *key) {
  std::string name(key);
  remove(name);
}