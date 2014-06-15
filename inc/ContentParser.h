#ifndef CONTENTPARSER_H
#define CONTENTPARSER_H

#include "Content.h"
#include <wx/buffer.h>

class CContentParser
{
public:
  static void ParseFromBuffer(const wxMemoryBuffer& buffer, CContent& content);
  static void ParseToBuffer(const CContent& content, wxMemoryBuffer& buffer);
};

#endif // CONTENTPARSER_H
