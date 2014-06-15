#include "Content.h"

///////////////////////////////////////////////////////////////
// CRecordList
// List for storing and sorting CRecords
// Elements are owned by list itself
// When add new item, it is _copied_ to the list
// (see CRecordList::Add for details)
// When assigning, entire content is copied to the other list
///////////////////////////////////////////////////////////////

CRecordList::CRecordList(const CRecordList& value)
{
  Assign(value);
}

void CRecordList::Assign(const CRecordList& value)
{
  Clear();
  for (size_t i = 0; i < value.array.size(); ++i)
    Add(value[i]);
}

void CRecordList::Clear()
{
  for (size_t i = 0; i < array.size(); ++i)
    delete array[i];
  array.resize(0);
}

void CRecordList::Sort()
{
  if (array.size() < 2)
    return;
  for (size_t i = 0; i < array.size() - 1; ++i)
  {
    size_t max = i;
    for (size_t j = i + 1; j < array.size(); ++j)
      if (array[max]->name.CmpNoCase(array[j]->name) > 0)
        max = j;
    Switch(i, max);
  }
}

size_t CRecordList::Add(const CRecord& value)
{
  CRecord* rec = new CRecord;
  *rec = value;
  array.push_back(rec);
  return GetCount() - 1;
}

void CRecordList::Delete(const size_t index)
{
  delete array[index];
  array.erase(array.begin() + index);
}

int CRecordList::Find(const CRecord& value) const
{
  for (size_t i = 0; i < array.size(); ++i)
    if (*array[i] == value)
      return i;
  return -1;
}

void CRecordList::Switch(const size_t indexA, const size_t indexB)
{
  if (indexA != indexB)
  {
    CRecord* temp = array[indexA];
    array[indexA] = array[indexB];
    array[indexB] = temp;
  }
}

////////////////////////////////////////////////////////
// CContent
// List for storing CRecordLists
// Elements are owned by list itself
// When add new item, its pointer is added to the list
// (see CContent::Add for details)
////////////////////////////////////////////////////////

void CContent::Clear()
{
  for (size_t i = 0; i < array.size(); ++i)
    delete array[i];
  array.resize(0);
}

void CContent::Sort()
{
  for (size_t i = 0; i < array.size(); ++i)
    array[i]->Sort();
}

size_t CContent::Add(CRecordList* value)
{
  array.push_back(value);
  return GetCount() - 1;
}

void CContent::Delete(const size_t index)
{
  delete array[index];
  array.erase(array.begin() + index);
}

void CContent::Move(const size_t index, const size_t to)
{
  if (index != to)
  {
    CRecordList* temp = array[index];
    array.erase(array.begin() + index);
    array.insert(array.begin() + to, temp);
  }
}
