#include "Variant.h"

using namespace std;


#define CheckAndFreeComplex()\
  if (isComplexType())\
    {\
    freeComplexType();\
    }

py::Variant::Variant(void)
{
  m_type = Invalid;
}

py::Variant::Variant(const int& i)
{
  m_type = Invalid;
  this->operator =(i);
}

py::Variant::Variant(const char& c)
{
  m_type = Invalid;
  this->operator =(c);
}

py::Variant::Variant(const short& s)
{
  m_type = Invalid;
  this->operator =(s);
}

py::Variant::Variant(const long& l)
{
  m_type = Invalid;
  this->operator =(l);
}

py::Variant::Variant(const long long & ll)
{
  m_type = Invalid;
  this->operator =(ll);
}

py::Variant::Variant(const float& f)
{
  m_type = Invalid;
  this->operator =(f);
}

py::Variant::Variant(const double& d)
{
  m_type = Invalid;
  this->operator =(d);
}

py::Variant::Variant(const std::string& str)
{
  m_type = Invalid;
  this->operator =(str);
}

py::Variant::Variant(const std::vector<std::string>& vec)
{
  m_type = Invalid;
  this->operator =(vec);
}

py::Variant::Variant(const std::map<std::string, std::string>& mp)
{
  m_type = Invalid;
  this->operator =(mp);
}

py::Variant::Variant(const bool& b)
{
  m_type = Invalid;
  this->operator =(b);
}

py::Variant::Variant(const char* str)
{
  m_type = Invalid;
  this->operator =(std::string(str));
}

py::Variant::Variant(const Variant& other)
{
  m_type = Invalid;
  this->operator =(other);
}

py::Variant::~Variant(void)
{
  CheckAndFreeComplex();
}

py::Variant& py::Variant::operator=(const int& i)
{
  CheckAndFreeComplex();
  m_data.i = i;
  m_type = Int;
  return *this;
}

py::Variant& py::Variant::operator=(const char& c)
{
  CheckAndFreeComplex();
  m_data.c = c;
  m_type = Char;
  return *this;
}

py::Variant& py::Variant::operator=(const short& s)
{
  CheckAndFreeComplex();
  m_data.s = s;
  m_type = Short;
  return *this;
}

py::Variant& py::Variant::operator=(const long& l)
{
  CheckAndFreeComplex();
  m_data.l = l;
  m_type = Long;
  return *this;
}

py::Variant& py::Variant::operator=(const long long & ll)
{
  CheckAndFreeComplex();
  m_data.ll = ll;
  m_type = LongLong;
  return *this;
}

py::Variant& py::Variant::operator=(const float& f)
{
  CheckAndFreeComplex();
  m_data.f = f;
  m_type = Float;
  return *this;
}

py::Variant& py::Variant::operator=(const double& d)
{
  CheckAndFreeComplex();
  m_data.d = d;
  m_type = Double;
  return *this;
}

py::Variant& py::Variant::operator=(const std::string& str)
{
  CheckAndFreeComplex();
  m_data.ptr = new string(str);
  m_type = String;
  return *this;
}

py::Variant& py::Variant::operator=(const std::vector<std::string>& vec)
{
  CheckAndFreeComplex();
  m_data.ptr = new std::vector<string>(vec);
  m_type = StringList;
  return *this;
}

py::Variant& py::Variant::operator=(const std::map<std::string,std::string>& mp)
{
  CheckAndFreeComplex();
  m_data.ptr = new std::map<string, string>(mp);
  m_type = StringMap;
  return *this;
}

py::Variant& py::Variant::operator=(const bool& b)
{
  CheckAndFreeComplex();
  m_data.b = b;
  m_type = Bool;
  return *this;
}

py::Variant& py::Variant::operator=(const py::Variant& other)
{
  m_type = Invalid;
  if (other.isComplexType())
    {
    switch(other.m_type)
      {
      case String:
        this->operator=(other.toString());
        break;
      case StringList:
        this->operator=(other.toVector());
        break;
      case StringMap:
        this->operator=(other.toMap());
        break;
      }
    }
  else
    {
    m_data = other.m_data;
    }
  m_type = other.m_type;

  return *this;
}

void py::Variant::freeComplexType()
{
  switch(getType())
    {
    case String:
      delete (string*) m_data.ptr;
      break;
    case StringList:
      delete (std::vector<string>*) m_data.ptr;
      break;
    case StringMap:
      delete(std::map<string, string>*) m_data.ptr;
      break;
    }
}

int py::Variant::toInt(bool *ok/*=0*/, int defaultValue /*=0*/) const
{
  if (getType() == Int)
    {
    if (ok)
      {
      *ok = true;
      }
      return m_data.i;
    }
  else
    {
    if (ok)
      {
      *ok = false;
      }
      return defaultValue;
    }
}

char py::Variant::toChar(bool *ok/*=0*/, char defaultChar /*=0*/) const
{
  if (getType() == Char)
    {
    if (ok)
      {
      *ok = true;
      }
      return m_data.c;
    }
  else
    {
    if (ok)
      {
      *ok = false;
      }
      return defaultChar;
    }
}

short py::Variant::toShort(bool *ok/*=0*/, short defaultShort /*=0*/) const
{
  if (getType() == Short)
    {
    if (ok)
      {
      *ok = true;
      }
      return m_data.s;
    }
  else
    {
    if (ok)
      {
      *ok = false;
      }
      return defaultShort;
    }
}

long py::Variant::toLong(bool *ok/*=0*/, long defaultLong /*=0*/) const
{
  if (getType() == Long)
    {
    if (ok)
      {
      *ok = true;
      }
      return m_data.l;
    }
  else
    {
    if (ok)
      {
      *ok = false;
      }
      return defaultLong;
    }
}

long long py::Variant::toLongLong(bool *ok/*=0*/, long long defaultLongLong /*=0*/) const
{
  if (getType() == LongLong)
    {
    if (ok)
      {
      *ok = true;
      }
      return m_data.ll;
    }
  else
    {
    if (ok)
      {
      *ok = false;
      }
      return defaultLongLong;
    }
}

float py::Variant::toFloat(bool *ok/*=0*/, float defaultFloat /*=0*/) const
{
  if (getType() == Float)
    {
    if (ok)
      {
      *ok = true;
      }
      return m_data.f;
    }
  else
    {
    if (ok)
      {
      *ok = false;
      }
      return defaultFloat;
    }
}

double py::Variant::toDouble(bool *ok/*=0*/, double defaultDouble /*=0*/) const
{
  if (getType() == Double)
    {
    if (ok)
      {
      *ok = true;
      }
      return m_data.d;
    }
  else
    {
    if (ok)
      {
      *ok = false;
      }
      return defaultDouble;
    }
}

std::string py::Variant::toString(bool *ok/*=0*/, const std::string& defaultString /*=""*/) const
{
  if (getType() == String)
    {
    if (ok)
      {
      *ok = true;
      }
      return *(string* ) (m_data.ptr);
    }
  else
    {
    if (ok)
      {
      *ok = false;
      }
      return defaultString;
    }
}

std::vector<std::string> py::Variant::toVector(bool *ok/*=0*/) const
{
  if (getType() == StringList)
    {
    if (ok)
      {
      *ok = true;
      }
      return *(vector<string>* ) (m_data.ptr);
    }
  else
    {
    if (ok)
      {
      *ok = false;
      }
      return vector<string>();
    }
}

std::map<std::string,std::string> py::Variant::toMap(bool *ok/*=0*/) const
{
  if (getType() == StringMap)
    {
    if (ok)
      {
      *ok = true;
      }
      return *(map<string,string>* )(m_data.ptr);
    }
  else
    {
    if (ok)
      {
      *ok = false;
      }
    return map<string,string>();
    }
}

bool py::Variant::toBool(bool *ok/*=0*/, bool defaultBool/*=false*/) const
{
  if (getType() == Bool)
    {
    if (ok)
      {
      *ok = true;
      }
    return m_data.b;
    }
  else
    {
      if (ok)
        {
        *ok = false;
        }
    return defaultBool;
    }
}

std::map<std::string, std::string>* py::Variant::mapPointer() const
{
  if (getType() == StringMap)
    {
    return (std::map<std::string, std::string>*) m_data.ptr;
    }
  else
    {
    return 0;
    }
}

