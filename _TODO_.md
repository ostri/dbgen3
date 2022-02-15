- [1. Todo list](#1-todo-list)
  - [1.1. Tests](#11-tests)
    - [1.1.1. integrate all tests under single umbrela](#111-integrate-all-tests-under-single-umbrela)
  - [1.2. Features](#12-features)
    - [1.2.1. shorter gsql definition - sql-set and sql](#121-shorter-gsql-definition---sql-set-and-sql)
    - [1.2.2. better error reporting upon grammar errors](#122-better-error-reporting-upon-grammar-errors)
    - [1.2.3. Fix bug with "?" in sql](#123-fix-bug-with--in-sql)
    - [1.2.4. support for u16string values](#124-support-for-u16string-values)
    - [1.2.5. better api for fetch scroll](#125-better-api-for-fetch-scroll)
  - [1.3. CMAKE](#13-cmake)
    - [1.3.1. restructure the cmake files](#131-restructure-the-cmake-files)
    - [1.3.2. build package](#132-build-package)
    - [1.3.3. install package](#133-install-package)
    - [1.3.4. t1.hpp is generated everywhere](#134-t1hpp-is-generated-everywhere)
  - [1.4. documentation](#14-documentation)
    - [1.4.1. doxygen](#141-doxygen)
      - [1.4.1.1. generated files](#1411-generated-files)
      - [1.4.1.2. source code](#1412-source-code)
    - [1.4.2. README](#142-readme)
- [2. Done](#2-done)
  - [2.1. restructure the grammar](#21-restructure-the-grammar)
  - [2.2. more specific requirements for elemetns within the grammar e.g. regex for id](#22-more-specific-requirements-for-elemetns-within-the-grammar-eg-regex-for-id)
  - [2.3. option to change the parameter / result column names](#23-option-to-change-the-parameter--result-column-names)
  - [2.4. classes in hpp must be in the same order as in gsql](#24-classes-in-hpp-must-be-in-the-same-order-as-in-gsql)
  - [2.5. trim_left -> canonical (left, strip empty leading/trailing lines)](#25-trim_left---canonical-left-strip-empty-leadingtrailing-lines)
  - [2.6. dbgen3 --grammar -> left trim](#26-dbgen3---grammar---left-trim)
  - [2.7. naming of buffer classes query::{par|res|custom}](#27-naming-of-buffer-classes-queryparrescustom)
  - [2.8. get both buffers from the utl instance](#28-get-both-buffers-from-the-utl-instance)
  - [2.9. buf support for null values getters and setters](#29-buf-support-for-null-values-getters-and-setters)
  - [2.10. initialize buffers so that insert / select wont report error](#210-initialize-buffers-so-that-insert--select-wont-report-error)
  - [2.11. features - eleminate phases](#211-features---eleminate-phases)
# 1. Todo list

## 1.1. Tests
### 1.1.1. integrate all tests under single umbrela
## 1.2. Features
### 1.2.1. shorter gsql definition - sql-set and sql
### 1.2.2. better error reporting upon grammar errors
### 1.2.3. Fix bug with "?" in sql
### 1.2.4. support for u16string values
### 1.2.5. better api for fetch scroll
## 1.3. CMAKE
### 1.3.1. restructure the cmake files
### 1.3.2. build package
### 1.3.3. install package
### 1.3.4. t1.hpp is generated everywhere

## 1.4. documentation
### 1.4.1. doxygen
#### 1.4.1.1. generated files
#### 1.4.1.2. source code
### 1.4.2. README

# 2. Done
## 2.1. restructure the grammar
## 2.2. more specific requirements for elemetns within the grammar e.g. regex for id
## 2.3. option to change the parameter / result column names
## 2.4. classes in hpp must be in the same order as in gsql
## 2.5. trim_left -> canonical (left, strip empty leading/trailing lines)
## 2.6. dbgen3 --grammar -> left trim
## 2.7. naming of buffer classes query::{par|res|custom}
## 2.8. get both buffers from the utl instance
## 2.9. buf support for null values getters and setters
## 2.10. initialize buffers so that insert / select wont report error
## 2.11. features - eleminate phases
