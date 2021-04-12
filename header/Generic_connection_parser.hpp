#pragma once

#include "Generic_station_parser.hpp"

namespace travel{
  class Generic_connection_parser: public Generic_station_parser{
  public:
    /**
     * const 修饰返回值，保护返回值在调用时不被修改（不作为左值)
     * const 修饰函数，防止该类的对象在调用该函数时对应的类数据成员被修改
     * const 修饰参数，防止传入参数被修改，在使用引用和指针优化效率同时又不希望参数被修改时使用
     * 注意的是，这里的引用和指针做参数能提高效益，只有在参数类型为自建的参数类型才可行，C++内部参数类型没必要
     */
    const std::unordered_map<uint64_t,std::unordered_map<uint64_t,uint64_t> >& get_connections_hashmap() const{
      return this->connections_hashmap;
    }

  protected:
    virtual void read_connections(const std::string& _filename) = 0;

/**
 * 1、public：类内、类的对象；派生类内、派生类对象--->>均可访问。
 * 2、protected：类内、派生类内--->>可以访问；类的对象、派生类的对象-->>不可访问。
 * 3、private：只有类内部-->>可以访问；类的对象、派生类、派生类的对象，统统的-->>不可访问。
 */
  protected:
    // 无序map容器，底层为hash_map
    std::unordered_map<uint64_t,std::unordered_map<uint64_t,uint64_t> > connections_hashmap;
  };
}
