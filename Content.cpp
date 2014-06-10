#include "Content.h"

CContent::CContent()
{

}

CContent::CContent(const CContent& value)
{
  Assign(value);
}

CContent::~CContent()
{
  Clear();
}

void CContent::Assign(const CContent& value)
{
  Clear();
  content = value.content;
}

void CContent::Clear()
{
  for (size_t i = 0; i < content.size(); ++i)
    delete content[i];
  content.resize(0);
}

void CContent::Sort()
{
  if (content.size() < 2)
    return;
  for (size_t i = 0; i < content.size(); ++i)
  {
    size_t max = 0;
    for (size_t j = i + 1; j < content.size(); ++j)
    {
      if (content[max]->name.Cmp(content[j]->name) > 0)
        max = j;
    }
    Switch(i, max);
  }
}

size_t CContent::Add(const CRecord& value)
{
  CRecord* rec = new CRecord;
  *rec = value;
  content.push_back(rec);
  return GetCount() - 1;
}

void CContent::Delete(const size_t index)
{
  delete content[index];
  content.erase(content.begin() + index);
}

int CContent::Find(const CRecord& value) const
{
  for (size_t i = 0; i < content.size(); ++i)
    if (*content[i] == value)
      return i;
  return -1;
}

CRecord& CContent::GetItem(const size_t index) const
{
  return *(content[index]);
}

void CContent::Switch(const size_t indexA, const size_t indexB)
{
  CRecord temp = *content[indexA];
  *content[indexA] = *content[indexB];
  *content[indexB] = temp;
}

const size_t CContent::GetCount() const
{
  return content.size();
}
