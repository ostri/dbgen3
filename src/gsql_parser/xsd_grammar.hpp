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
        <!-- id-name .........................................................................-->
        <xs:simpleType name="id-name">
          <xs:annotation>
            <xs:documentation>
                id-name type which denotes also c++ identifier with the following requirements
                - it starts with small or capital letter or _
                - remaining part can have arbitrary letter, decimal digit or _
                - minimum length is 1
                - maximum length is 32 characters
            </xs:documentation>
          </xs:annotation>
          <xs:restriction base="xs:string">
            <xs:pattern value="[A-Za-z_][A-Za-z_0-9-]*"></xs:pattern>
            <xs:maxLength value="32"/>
            <xs:minLength value="1"/> 
          </xs:restriction>
        </xs:simpleType>
        <!-- list of id names ...............................................................-->
        <xs:simpleType name="id-name-list">
          <xs:annotation>
            <xs:documentation>
                list of id_names separatd by whitespace
            </xs:documentation>
          </xs:annotation>
          <xs:list itemType="id-name"></xs:list>
        </xs:simpleType> 
        <!-- list of integers ...............................................................-->
        <xs:simpleType name="integer-list">
          <xs:list itemType="xs:integer"/>
        </xs:simpleType>
        <!-- definition of a buffer .........................................................-->
        <xs:annotation>
          <xs:documentation>
              definition of the buffer with the following attributes
              - id 
              - skip
              - columns
            </xs:documentation>
        </xs:annotation>
        <xs:complexType name="buffer_type">
          <xs:attribute name="id"      type="id-name"      use="optional" />
          <xs:attribute name="columns" type="id-name-list" use="optional" />
          <xs:attribute name="skip"    type="xs:boolean"   use="optional" />
        </xs:complexType>
        <!-- enum RDBMS ....................................................................-->
        <xs:simpleType name="rdbms_type">
          <xs:annotation>
            <xs:documentation>
              supported rdbms types
              - generic sql
              - db2
              - oracle
              - mssql
              - mariadb
            </xs:documentation>
          </xs:annotation>
          <xs:restriction base="xs:string">
            <xs:enumeration value="sql"/>
            <xs:enumeration value="db2"/>
            <xs:enumeration value="oracle"/>
            <xs:enumeration value="mssql"/>
            <xs:enumeration value="mariadb"/>
          </xs:restriction>
        </xs:simpleType>
        <!-- sql-set ...................................................................-->
        <xs:complexType name="sql-set">
          <xs:annotation>
            <xs:documentation>
              sequence of sql descriptions
            </xs:documentation>
          </xs:annotation>
          <xs:sequence>
            <xs:element name="sql" minOccurs="1" maxOccurs="unbounded" type="mixed">
              <xs:element name="prepare" type="xs:string" minOccurs="0" maxOccurs="1"/>
              <xs:attribute name="rdbms" type="rdbms_type" use="optional" />
            </xs:element>
          </xs:sequence>
        </xs:complexType>
        <!-- simple-sql................................................................-->
        <xs:complexType name="simple-sql">
          <xs:annotation>
            <xs:documentation>
              sequence of sql descriptions
            </xs:documentation>
          </xs:annotation>
          <xs:sequence>
            <xs:element name="sql" minOccurs="0" maxOccurs="unbounded" type="mixed">
              <xs:element name="prepare" type="xs:string" minOccurs="0" maxOccurs="1"/>
              <xs:attribute name="rdbms" type="rdbms_type" use="optional" />
            </xs:element>
          </xs:sequence>
        </xs:complexType>
        <!-- query ....................................................................-->
        <xs:complexType name="query-type">
          <xs:annotation>
            <xs:documentation>
              definition of a query
            </xs:documentation>
          </xs:annotation>
          <xs:sequence minOccurs="1" maxOccurs="1">
            <xs:element name="qp" type="buffer_type" minOccurs="0" maxOccurs="1"/>
            <xs:element name="qr" type="buffer_type" minOccurs="0" maxOccurs="1"/>
            <xs:sequence>
              <xs:element name="sql" minOccurs="0" maxOccurs="unbounded">
                  <xs:element name="prepare" type="xs:string" minOccurs="0" maxOccurs="1"/>
                  <xs:attribute name="rdbms" type="rdbms_type" use="optional" />
              </xs:element>
            </xs:sequence>
          </xs:sequence>
          <xs:attribute name="id" type="id-name" use="optional"/>
          <xs:attribute name="allowed" type="integer-list" use="optional" /> 
        </xs:complexType>
        <!-- query ....................................................................-->
        <xs:complexType name="query-set-type">
          <xs:annotation>
            <xs:documentation>
              definition of a set of queries
              - hdr it prints on top of the generated file "as is"
              - q set of queries
            </xs:documentation>
          </xs:annotation>
          <xs:sequence>
              <xs:element name="hdr" minOccurs="0" maxOccurs="1"/>
              <xs:element name="q"   type="query-type" minOccurs= "1" maxOccurs="unbounded"/>
          </xs:sequence>
          <xs:attribute name="id" type="id-name" use="optional" />
        </xs:complexType>
        <!-- rest of the document ......................................................-->
        <xs:element name="q-set" type="query-set-type"/>
      </xs:schema>      
    )k0a1f2k3a4";
}; // namespace dbgen3

#endif // XSD_GRAMMAR_HPP
