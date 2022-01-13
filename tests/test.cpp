#include <xercesc/dom/DOM.hpp>
#include <string>
#include <iostream>

XERCES_CPP_NAMESPACE_USE

DOMElement* nextChildElement(const DOMElement* parent)
{
  DOMNode* node = (DOMNode*)parent->getFirstChild();
  while (node)
  {
    if (node->getNodeType() == DOMNode::ELEMENT_NODE) return (DOMElement*)node;
    node = node->getNextSibling();
  }
  return nullptr;
}

std::string readTextNode(const DOMElement* el)
{
  std::string sstr;
  DOMNode*    node = el->getFirstChild();
  if (node->getNodeType() == DOMNode::TEXT_NODE)
  {
    char* cstr = XMLString::transcode(node->getNodeValue());
    sstr       = cstr;
    XMLString::release(&cstr);
  }
  return sstr;
}

int main(int argc, char** argv)
{
  XMLPlatformUtils::Initialize();

  XMLCh tempStr[100];
  XMLString::transcode("LS", tempStr, 99);
  DOMImplementation* impl = DOMImplementationRegistry::getDOMImplementation(tempStr);
  DOMLSParser*       parser =
    ((DOMImplementationLS*)impl)->createLSParser(DOMImplementationLS::MODE_SYNCHRONOUS, 0);
  DOMDocument* doc = impl->createDocument(0, 0, 0);

  doc = parser->parseURI("../gsql/t1.gsql");

  DOMElement* el = doc->getDocumentElement(); // <settings>
  std::cerr << reinterpret_cast<uint64_t>(el) << std::endl;
  std::cerr << "'" << readTextNode(el) << "'" << std::endl;
 //   el             = nextChildElement(el);      //   <port>
// //   el             = nextChildElement(el);      //     <reference>Ref1</reference>

//   // No memory leak
//   std::string nodestr;
//   auto        cnt = 1;
//   while (cnt--)
//   {
//     nodestr = readTextNode(el); // nodestr is "Ref1"
//     std::cerr << "'" << nodestr << "'" << std::endl;
//   }
}