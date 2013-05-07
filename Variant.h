/**
 * Variant class
 * @author Rui Li
 * @date   03/10/2010
 */

#ifndef VARIANT_H
#define VARIANT_H

#include <string>
#include <map>
#include <vector>

namespace py
{
  class Variant
  {
  public:
    enum Type
    {
      ComplexType = (1 << 31),
      Invalid = 0,
      Char = 1,
      Int = 2,
      Short = 3,
      Long = 4,
      LongLong = 5,
      Float = 6,
      Double = 7,
      Bool = 8,
      /// ComplexType
      String = 9 | ComplexType,
      StringList = 10 | ComplexType,
      StringMap = 11 | ComplexType,
    };

    Variant(void);
    virtual ~Variant(void);

    Variant(const char* str);
    Variant(const Variant& other);
    Variant& operator =(const Variant& other);

    Variant(const int& i);
    Variant& operator =(const int &i);
    int toInt(bool* ok=0, int defaultValue=0) const;

    Variant(const char& c);
    Variant& operator =(const char& c);
    char toChar(bool* ok=0, char defaultChar=0) const;

    Variant(const short& s);
    Variant& operator = (const short& s);
    short toShort(bool* ok=0, short defaultShort=0) const;

    Variant(const long& l);
    Variant& operator =(const long& l);
    long toLong(bool* ok=0, long defaultLong=0) const;

    Variant(const long long & ll);
    Variant& operator =(const long long & ll);
    long long toLongLong(bool* ok=0, long long defaultLongLong=0) const;

    Variant(const float& f);
    Variant& operator =(const float& f);
    float toFloat(bool* ok=0, float defaultFloat=0) const;

    Variant(const double& d);
    Variant& operator =(const double& d);
    double toDouble(bool* ok=0, double defaultDouble=0) const;

    Variant(const std::string& str);
    Variant& operator =(const std::string& str);
    std::string toString(bool* ok=0, const std::string& defaultString="") const;

    Variant(const std::vector<std::string>& vec);
    Variant& operator =(const std::vector<std::string>& vec);
    std::vector<std::string> toVector(bool* ok=0) const;

    Variant(const std::map<std::string,std::string>& mp);
    Variant& operator =(const std::map<std::string, std::string>& mp);
    std::map<std::string,std::string> toMap(bool* ok=0) const;
    std::map<std::string,std::string>* mapPointer() const;

    explicit Variant(const bool &b);
    Variant& operator =(const bool& b);
    bool toBool(bool *ok=0, bool defaultBool=false) const;

    inline Type getType() const
    {
      return m_type;
    }

    inline bool isComplexType() const
    {
      return m_type&ComplexType;
    }

    inline bool isValid() const
    {
      return m_type==Invalid;
    }
  
  private:
    union __PrivateUnion
    {
      char c;
      int i;
      short s;
      long l;
      long long ll;
      float f;
      double d;
      bool b;
      void* ptr;
    };

    Type m_type;
    __PrivateUnion m_data;

    void freeComplexType();
  };
}

#endif

