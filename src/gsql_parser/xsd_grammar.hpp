#ifndef XSD_GRAMMAR_HPP
#define XSD_GRAMMAR_HPP
#include <string_view>

namespace dbgen3
{
  using cstr_t = std::string_view;
  constexpr static const cstr_t grammar_ =
    R"k0a1f2k3a4(
      <?xml version="1.0" encoding="UTF-8"?>
      <xs:schema xmlns:xs="http://www.w3.org/2001/XMLSchema">
          <xs:element name="q-set">
              <xs:complexType>
                  <xs:sequence>
                      <xs:element name="hdr" minOccurs="0" maxOccurs="1"/>
                      <xs:element name="q" minOccurs= "1" maxOccurs="unbounded">
                          <xs:complexType>
                              <xs:sequence minOccurs="1" maxOccurs="1">
                                  <xs:element name="qp" minOccurs="0" maxOccurs="1">
                                      <xs:complexType>
                                          <xs:attribute name="id" use="optional" />
                                      </xs:complexType>
                                  </xs:element>
                                  <xs:element name="qr" minOccurs="0" maxOccurs="1">
                                      <xs:complexType>
                                          <xs:attribute name="id" use="optional" />
                                      </xs:complexType>
                                  </xs:element>
                                  <xs:element name="sql-set" minOccurs="1" maxOccurs="1">
                                      <xs:complexType>
                                          <xs:sequence>
                                              <xs:element name="sql" minOccurs="1" maxOccurs="unbounded" />
                                          </xs:sequence>
                                      </xs:complexType>
                                  </xs:element>
                              </xs:sequence>
                              <xs:attribute name="id" use="optional"/>
                          </xs:complexType>
                      </xs:element>
                  </xs:sequence>
                  <xs:attribute name="id" use="optional" />
              </xs:complexType>
          </xs:element>
      </xs:schema>      
    )k0a1f2k3a4";
}; // namespace dbgen3

#endif // XSD_GRAMMAR_HPP
