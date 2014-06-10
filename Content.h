#ifndef CONTENT_H
#define CONTENT_H

#include <wx/string.h>
#include <vector>

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
  };

  friend bool operator==(const CRecord& left, const CRecord& right)
  {
    return (left.name == right.name) && (left.login == right.login) &&
          (left.email == right.email) && (left.password == right.password);
  };

  friend bool operator!=(const CRecord& left, const CRecord& right)
  {
    return !(left == right);
  };
};

class CContent
{
private:
  std::vector <CRecord*> content;
public:
  // Constructors
  CContent();
  CContent(const CContent& value);

  // Destructor
  ~CContent();

  // Methods
  void Assign(const CContent& value);
  void Clear();
  void Sort();

  // Elements' operations
  size_t Add(const CRecord& value);
  void Delete(const size_t index);
  int Find(const CRecord& value) const;
  CRecord& GetItem(const size_t index) const;
  void Switch(const size_t indexA, const size_t indexB);

  // Properties
  const size_t GetCount() const;

  // Operators
  CRecord& operator[](const size_t index) const { return GetItem(index); };
  CContent& operator=(const CContent& right) { Assign(right); return *this; };
};

#endif // CONTENT_H
