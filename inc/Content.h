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

class CRecordList
{
private:
  wxString fName;
  std::vector <CRecord*> array;
public:
  // Constructors
  CRecordList(const wxString& name)
              : fName(name) {};
  CRecordList(const CRecordList& value);

  // Destructor
  ~CRecordList() { Clear(); };

  // Methods
  void Assign(const CRecordList& value);
  void Clear();
  void Sort();

  // Elements' operations
  size_t Add(const CRecord& value);
  void Delete(const size_t index);
  int Find(const CRecord& value) const;
  CRecord& GetRecord(const size_t index) const { return *(array[index]); };
  void Switch(const size_t indexA, const size_t indexB);

  // Properties
  const size_t GetCount() const { return array.size(); };
  const wxString GetName() const { return fName; };
  void SetName(const wxString& name) { fName = name; };

  // Operators
  CRecord& operator[](const size_t index) const { return GetRecord(index); };
  CRecordList& operator=(const CRecordList& right) { Assign(right); return *this; };
};

class CContent
{
private:
  std::vector <CRecordList*> array;
public:
  // Constructors
  CContent() {};

  // Destructor
  ~CContent() { Clear(); };

  // Methods
  void Clear();
  void Sort();

  // Elements' operations
  size_t Add(CRecordList* value);
  void Delete(const size_t index);
  CRecordList* GetList(const size_t index) const { return array[index]; };
  void Move(const size_t index, const size_t to);

  // Properties
  const size_t GetCount() const { return array.size(); };

  // Operators
  CRecordList* operator[](const size_t index) const { return GetList(index); };
};

#endif // CONTENT_H
