/**
 * PythonCppApiVariant class
 * @author Rui Li
 * @date   03/10/2010
 */

#ifndef PYTHONCPPAPIVARIANT_H
#define PYTHONCPPAPIVARIANT_H

#include <string>
#include <map>
#include <vector>

namespace py
{
  class PythonCppApiVariant
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

    PythonCppApiVariant(void);
    virtual ~PythonCppApiVariant(void);

    PythonCppApiVariant(const char* str);
    PythonCppApiVariant(const PythonCppApiVariant& other);
    PythonCppApiVariant& operator =(const PythonCppApiVariant& other);

    PythonCppApiVariant(const int& i);
    PythonCppApiVariant& operator =(const int &i);
    int toInt(bool* ok=0, int defaultValue=0) const;

    PythonCppApiVariant(const char& c);
    PythonCppApiVariant& operator =(const char& c);
    char toChar(bool* ok=0, char defaultChar=0) const;

    PythonCppApiVariant(const short& s);
    PythonCppApiVariant& operator = (const short& s);
    short toShort(bool* ok=0, short defaultShort=0) const;

    PythonCppApiVariant(const long& l);
    PythonCppApiVariant& operator =(const long& l);
    long toLong(bool* ok=0, long defaultLong=0) const;

    PythonCppApiVariant(const long long & ll);
    PythonCppApiVariant& operator =(const long long & ll);
    long long toLongLong(bool* ok=0, long long defaultLongLong=0) const;

    PythonCppApiVariant(const float& f);
    PythonCppApiVariant& operator =(const float& f);
    float toFloat(bool* ok=0, float defaultFloat=0) const;

    PythonCppApiVariant(const double& d);
    PythonCppApiVariant& operator =(const double& d);
    double toDouble(bool* ok=0, double defaultDouble=0) const;

    PythonCppApiVariant(const std::string& str);
    PythonCppApiVariant& operator =(const std::string& str);
    std::string toString(bool* ok=0, const std::string& defaultString="") const;

    PythonCppApiVariant(const std::vector<std::string>& vec);
    PythonCppApiVariant& operator =(const std::vector<std::string>& vec);
    std::vector<std::string> toVector(bool* ok=0) const;

    PythonCppApiVariant(const std::map<std::string,std::string>& mp);
    PythonCppApiVariant& operator =(const std::map<std::string, std::string>& mp);
    std::map<std::string,std::string> toMap(bool* ok=0) const;
    std::map<std::string,std::string>* mapPointer() const;

    explicit PythonCppApiVariant(const bool &b);
    PythonCppApiVariant& operator =(const bool& b);
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

