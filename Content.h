#ifndef CONTENT_H
#define CONTENT_H

#include <wx/string.h>

class CRecord
{
public:
  wxString name;
  wxString login;
  wxString email;
  wxString password;

  CRecord& operator=(const CRecord& right)
  {
    if (&right == this)
      return *this;
    name = right.name;
    login = right.login;
    email = right.email;
    password = right.password;
    return *this;
  }

  friend bool operator==(const CRecord& left, const CRecord& right)
  {
    return (left.name == right.name) && (left.login == right.login) &&
          (left.email == right.email) && (left.password == right.password);
  }

  friend bool operator!=(const CRecord& left, const CRecord& right)
  {
    return !(left == right);
  }
};

class CContent
{
private:
  WX_DEFINE_ARRAY_PTR(CRecord *, content);
public:
  // Constructors
  CContent();
  CContent(const CContent& value);

  // Destructor
  ~CContent();

  // Methods
  void Assign(const CContent& value);
  void Clear();

  // Elements' operations
  void Delete(const size_t index);
  CRecord* GetItem(const size_t index);
  void Switch(const size_t indexA, const size_t indexB);

  // Operators

};

#endif // CONTENT_H
