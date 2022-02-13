- [1. Todo list](#1-todo-list)
  - [1.1. integrate all tests under single umbrela](#11-integrate-all-tests-under-single-umbrela)
  - [1.2. eleminate phases](#12-eleminate-phases)
  - [1.3. shorter gsql definition - sql-set and sql](#13-shorter-gsql-definition---sql-set-and-sql)
  - [1.4. better error reporting upon grammar errors](#14-better-error-reporting-upon-grammar-errors)
  - [1.5. restructure the cmake files](#15-restructure-the-cmake-files)
  - [1.6. build package](#16-build-package)
  - [1.7. install package](#17-install-package)
  - [1.8. Fix bug with "?" in sql](#18-fix-bug-with--in-sql)
  - [1.9. t1.hpp is generated anywhere](#19-t1hpp-is-generated-anywhere)
  - [1.10. support for u16string values](#110-support-for-u16string-values)
  - [1.11. better api for fetch scroll](#111-better-api-for-fetch-scroll)
  - [1.12. documentation](#112-documentation)
    - [1.12.1. doxygen](#1121-doxygen)
      - [1.12.1.1. generated files](#11211-generated-files)
      - [1.12.1.2. source code](#11212-source-code)
    - [1.12.2. README](#1122-readme)
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
# 1. Todo list

## 1.1. integrate all tests under single umbrela
## 1.2. eleminate phases
## 1.3. shorter gsql definition - sql-set and sql
## 1.4. better error reporting upon grammar errors
## 1.5. restructure the cmake files
## 1.6. build package
## 1.7. install package
## 1.8. Fix bug with "?" in sql
## 1.9. t1.hpp is generated anywhere
## 1.10. support for u16string values
## 1.11. better api for fetch scroll
## 1.12. documentation
### 1.12.1. doxygen
#### 1.12.1.1. generated files
#### 1.12.1.2. source code
### 1.12.2. README

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
