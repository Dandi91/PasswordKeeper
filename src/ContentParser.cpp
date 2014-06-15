#include "ContentParser.h"

#include <wx/string.h>
#include <wx/mstream.h>

void ParseFieldFromStream(wxMemoryInputStream& stream, wxString& field)
{
  size_t fieldLength;
  stream.Read(&fieldLength, sizeof(fieldLength));
  field = wxString::FromUTF8((char*)stream.GetInputStreamBuffer()->GetBufferPos(), fieldLength);
  stream.SeekI(fieldLength, wxFromCurrent);
}

void ParseFieldToStream(wxMemoryOutputStream& stream, const wxString& field)
{
  wxScopedCharBuffer charbuff;
  charbuff = field.ToUTF8();
  size_t length = charbuff.length();
  stream.Write(&length, sizeof(length));
  stream.Write(charbuff, length);
}

void ParseRecordFromStream(wxMemoryInputStream& stream, CRecord& record)
{
  ParseFieldFromStream(stream, record.name);
  ParseFieldFromStream(stream, record.login);
  ParseFieldFromStream(stream, record.email);
  ParseFieldFromStream(stream, record.password);
}

void ParseRecordToStream(wxMemoryOutputStream& stream, const CRecord& record)
{
  ParseFieldToStream(stream, record.name);
  ParseFieldToStream(stream, record.login);
  ParseFieldToStream(stream, record.email);
  ParseFieldToStream(stream, record.password);
}

void ParseSectionFromStream(wxMemoryInputStream& stream, CContent& content)
{
  // Retrieve section's name
  wxString sectionName;
  ParseFieldFromStream(stream, sectionName);
  // Add new section (CRecordList)
  CRecordList* newList = new CRecordList(sectionName);
  content.Add(newList);
  // Retrieve records
  size_t recordCount;
  stream.Read(&recordCount, sizeof(recordCount));
  while (recordCount > 0)
  {
    CRecord record;
    ParseRecordFromStream(stream, record);
    newList->Add(record);
    recordCount--;
  }
}

void ParseSectionToStream(wxMemoryOutputStream& stream, const CRecordList* content)
{
  // Write section's name
  ParseFieldToStream(stream, content->GetName());
  // Write records
  size_t i = 0, recordCount = content->GetCount();
  stream.Write(&recordCount, sizeof(recordCount));
  while (i < recordCount)
    ParseRecordToStream(stream, (*content)[i++]);
}

void CContentParser::ParseFromBuffer(const wxMemoryBuffer& buffer, CContent& content)
{
  wxMemoryInputStream stream(buffer.GetData(), buffer.GetDataLen());

  stream.SeekI(0);
  content.Clear();
  size_t sectionCount;
  stream.Read(&sectionCount, sizeof(sectionCount));
  while (sectionCount-- > 0)
    ParseSectionFromStream(stream, content);
}

void CContentParser::ParseToBuffer(const CContent& content, wxMemoryBuffer& buffer)
{
  wxMemoryOutputStream stream;

  size_t i = 0, sectionCount = content.GetCount();
  stream.Write(&sectionCount, sizeof(sectionCount));
  while (i < sectionCount)
    ParseSectionToStream(stream, content[i++]);

  buffer.AppendData(stream.GetOutputStreamBuffer()->GetBufferStart(), stream.GetLength());
}
