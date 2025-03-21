/*
 * Copyright (c) 2003, 2013, Oracle and/or its affiliates. All rights reserved.
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 *
 * This code is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2 only, as
 * published by the Free Software Foundation.
 *
 * This code is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 * version 2 for more details (a copy is included in the LICENSE file that
 * accompanied this code).
 *
 * You should have received a copy of the GNU General Public License version
 * 2 along with this work; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 * Please contact Oracle, 500 Oracle Parkway, Redwood Shores, CA 94065 USA
 * or visit www.oracle.com if you need additional information or have any
 * questions.
 *
 */

#ifndef SHARE_VM_OOPS_CONSTMETHODOOP_HPP
#define SHARE_VM_OOPS_CONSTMETHODOOP_HPP

#include "oops/oop.hpp"

// An ConstMethod* represents portions of a Java method which
// do not vary.
//
// Memory layout (each line represents a word). Note that most
// applications load thousands of methods, so keeping the size of this
// structure small has a big impact on footprint.
//
// |------------------------------------------------------|
// | header                                               |
// | klass                                                |
// |------------------------------------------------------|
// | fingerprint 1                                        |
// | fingerprint 2                                        |
// | constants                      (oop)                 |
// | stackmap_data                  (oop)                 |
// | constMethod_size                                     |
// | interp_kind  | flags    | code_size                  |
// | name index              | signature index            |
// | method_idnum            | max_stack                  |
// | max_locals              | size_of_parameters         |
// |------------------------------------------------------|
// |                                                      |
// | byte codes                                           |
// |                                                      |
// |------------------------------------------------------|
// | compressed linenumber table                          |
// |  (see class CompressedLineNumberReadStream)          |
// |  (note that length is unknown until decompressed)    |
// |  (access flags bit tells whether table is present)   |
// |  (indexed from start of ConstMethod*)                |
// |  (elements not necessarily sorted!)                  |
// |------------------------------------------------------|
// | localvariable table elements + length (length last)  |
// |  (length is u2, elements are 6-tuples of u2)         |
// |  (see class LocalVariableTableElement)               |
// |  (access flags bit tells whether table is present)   |
// |  (indexed from end of ConstMethod*)                  |
// |------------------------------------------------------|
// | exception table + length (length last)               |
// |  (length is u2, elements are 4-tuples of u2)         |
// |  (see class ExceptionTableElement)                   |
// |  (access flags bit tells whether table is present)   |
// |  (indexed from end of ConstMethod*)                  |
// |------------------------------------------------------|
// | checked exceptions elements + length (length last)   |
// |  (length is u2, elements are u2)                     |
// |  (see class CheckedExceptionElement)                 |
// |  (access flags bit tells whether table is present)   |
// |  (indexed from end of ConstMethod*)                  |
// |------------------------------------------------------|
// | method parameters elements + length (length last)    |
// |  (length is u2, elements are u2, u4 structures)      |
// |  (see class MethodParametersElement)                 |
// |  (access flags bit tells whether table is present)   |
// |  (indexed from end of ConstMethod*)                  |
// |------------------------------------------------------|
// | generic signature index (u2)                         |
// |  (indexed from start of constMethodOop)              |
// |------------------------------------------------------|
// | annotations arrays - method, parameter, type, default|
// | pointer to Array<u1> if annotation is present        |
// |------------------------------------------------------|
//
// IMPORTANT: If anything gets added here, there need to be changes to
// ensure that ServicabilityAgent doesn't get broken as a result!


// Utility class describing elements in checked exceptions table inlined in Method*.
class CheckedExceptionElement VALUE_OBJ_CLASS_SPEC {
 public:
  u2 class_cp_index;
};


// Utility class describing elements in local variable table inlined in Method*.
class LocalVariableTableElement VALUE_OBJ_CLASS_SPEC {
 public:
  u2 start_bci;
  u2 length;
  u2 name_cp_index;
  u2 descriptor_cp_index;
  u2 signature_cp_index;
  u2 slot;
};

// Utility class describing elements in exception table
class ExceptionTableElement VALUE_OBJ_CLASS_SPEC {
 public:
  u2 start_pc;
  u2 end_pc;
  u2 handler_pc;
  u2 catch_type_index;
};

// Utility class describing elements in method parameters
class MethodParametersElement VALUE_OBJ_CLASS_SPEC {
 public:
  u2 name_cp_index;
  u2 flags;
};

class KlassSizeStats;

// Class to collect the sizes of ConstMethod inline tables
#define INLINE_TABLES_DO(do_element)            \
  do_element(localvariable_table_length)        \
  do_element(compressed_linenumber_size)        \
  do_element(exception_table_length)            \
  do_element(checked_exceptions_length)         \
  do_element(method_parameters_length)          \
  do_element(generic_signature_index)           \
  do_element(method_annotations_length)         \
  do_element(parameter_annotations_length)      \
  do_element(type_annotations_length)           \
  do_element(default_annotations_length)

#define INLINE_TABLE_DECLARE(sym)    int _##sym;
#define INLINE_TABLE_PARAM(sym)      int sym,
#define INLINE_TABLE_INIT(sym)       _##sym(sym),
#define INLINE_TABLE_NULL(sym)       _##sym(0),
#define INLINE_TABLE_ACCESSOR(sym)   int sym() const { return _##sym; }

class InlineTableSizes : StackObj {
  // declarations
  INLINE_TABLES_DO(INLINE_TABLE_DECLARE)
  int _end;
 public:
  InlineTableSizes(
      INLINE_TABLES_DO(INLINE_TABLE_PARAM)
      int end) :
      INLINE_TABLES_DO(INLINE_TABLE_INIT)
      _end(end) {}

  // Default constructor for no inlined tables
  InlineTableSizes() :
      INLINE_TABLES_DO(INLINE_TABLE_NULL)
      _end(0) {}

  // Accessors
  INLINE_TABLES_DO(INLINE_TABLE_ACCESSOR)
};
#undef INLINE_TABLE_ACCESSOR
#undef INLINE_TABLE_NULL
#undef INLINE_TABLE_INIT
#undef INLINE_TABLE_PARAM
#undef INLINE_TABLE_DECLARE


class ConstMethod : public MetaspaceObj {
  friend class VMStructs;

public:
  typedef enum { NORMAL, OVERPASS } MethodType;

private:
  enum {
    _has_linenumber_table = 0x0001,
    _has_checked_exceptions = 0x0002,
    _has_localvariable_table = 0x0004,
    _has_exception_table = 0x0008,
    _has_generic_signature = 0x0010,
    _has_method_parameters = 0x0020,
    _is_overpass = 0x0040,
    _has_method_annotations = 0x0080,
    _has_parameter_annotations = 0x0100,
    _has_type_annotations = 0x0200,
    _has_default_annotations = 0x0400
  };

  // Bit vector of signature
  // Callers interpret 0=not initialized yet and
  // -1=too many args to fix, must parse the slow way.
  // The real initial value is special to account for nonatomicity of 64 bit
  // loads and stores.  This value may updated and read without a lock by
  // multiple threads, so is volatile.
  volatile uint64_t _fingerprint;
  // 保存对从常量池的索引
  ConstantPool*     _constants;                  // Constant pool

  // Raw stackmap data for the method
  Array<u1>*        _stackmap_data;

  int               _constMethod_size;
  // 访问标识符
  u2                _flags;

  // Size of Java bytecodes allocated immediately after Method*.
  u2                _code_size;
  u2                _name_index;                 // Method name (index in constant pool)
  u2                _signature_index;            // Method signature (index in constant pool)
  // 对于方法来说，这是唯一ID，这个ID的值通常是methods数据下表索引。
  u2                _method_idnum;               // unique identification number for the method within the class
                                                 // initially corresponds to the index into the methods array.
                                                 // but this may change with redefinition
  u2                _max_stack;                  // Maximum number of entries on the expression stack
  u2                _max_locals;                 // Number of local variables used by this method
  u2                _size_of_parameters;         // size of the parameter block (receiver + arguments) in words

  // Constructor
  ConstMethod(int byte_code_size,
              InlineTableSizes* sizes,
              MethodType is_overpass,
              int size);
public:

  static ConstMethod* allocate(ClassLoaderData* loader_data,
                               int byte_code_size,
                               InlineTableSizes* sizes,
                               MethodType mt,
                               TRAPS);

  bool is_constMethod() const { return true; }

  // Inlined tables
  void set_inlined_tables_length(InlineTableSizes* sizes);

  bool has_generic_signature() const
    { return (_flags & _has_generic_signature) != 0; }

  bool has_linenumber_table() const
    { return (_flags & _has_linenumber_table) != 0; }

  bool has_checked_exceptions() const
    { return (_flags & _has_checked_exceptions) != 0; }

  bool has_localvariable_table() const
    { return (_flags & _has_localvariable_table) != 0; }

  bool has_exception_handler() const
    { return (_flags & _has_exception_table) != 0; }

  bool has_method_parameters() const
    { return (_flags & _has_method_parameters) != 0; }

  MethodType method_type() const {
    return ((_flags & _is_overpass) == 0) ? NORMAL : OVERPASS;
  }

  void set_method_type(MethodType mt) {
    if (mt == NORMAL) {
      _flags &= ~(_is_overpass);
    } else {
      _flags |= _is_overpass;
    }
  }

  // constant pool
  ConstantPool* constants() const        { return _constants; }
  void set_constants(ConstantPool* c)    { _constants = c; }

  Method* method() const;

  // stackmap table data
  Array<u1>* stackmap_data() const { return _stackmap_data; }
  void set_stackmap_data(Array<u1>* sd) { _stackmap_data = sd; }
  void copy_stackmap_data(ClassLoaderData* loader_data, u1* sd, int length, TRAPS);
  bool has_stackmap_table() const { return _stackmap_data != NULL; }

  void init_fingerprint() {
    const uint64_t initval = CONST64(0x8000000000000000);
    _fingerprint = initval;
  }

  uint64_t fingerprint() const                   {
    // Since reads aren't atomic for 64 bits, if any of the high or low order
    // word is the initial value, return 0.  See init_fingerprint for initval.
    uint high_fp = (uint)(_fingerprint >> 32);
    if ((int) _fingerprint == 0 || high_fp == 0x80000000) {
      return 0L;
    } else {
      return _fingerprint;
    }
  }

  uint64_t set_fingerprint(uint64_t new_fingerprint) {
#ifdef ASSERT
    // Assert only valid if complete/valid 64 bit _fingerprint value is read.
    uint64_t oldfp = fingerprint();
#endif // ASSERT
    _fingerprint = new_fingerprint;
    assert(oldfp == 0L || new_fingerprint == oldfp,
           "fingerprint cannot change");
    assert(((new_fingerprint >> 32) != 0x80000000) && (int)new_fingerprint !=0,
           "fingerprint should call init to set initial value");
    return new_fingerprint;
  }

  // name
  int name_index() const                         { return _name_index; }
  void set_name_index(int index)                 { _name_index = index; }

  // signature
  int signature_index() const                    { return _signature_index; }
  void set_signature_index(int index)            { _signature_index = index; }

  // generics support
  int generic_signature_index() const            {
    if (has_generic_signature()) {
      return *generic_signature_index_addr();
    } else {
      return 0;
    }
  }
  void set_generic_signature_index(u2 index)    {
    assert(has_generic_signature(), "");
    u2* addr = generic_signature_index_addr();
    *addr = index;
  }

  // Sizing
  static int header_size() {
    return sizeof(ConstMethod)/HeapWordSize;
  }

  // Size needed
  static int size(int code_size, InlineTableSizes* sizes);

  int size() const                    { return _constMethod_size;}
  void set_constMethod_size(int size)     { _constMethod_size = size; }
#if INCLUDE_SERVICES
  void collect_statistics(KlassSizeStats *sz) const;
#endif

  // code size
  int code_size() const                          { return _code_size; }
  void set_code_size(int size) {
    assert(max_method_code_size < (1 << 16),
           "u2 is too small to hold method code size in general");
    assert(0 <= size && size <= max_method_code_size, "invalid code size");
    _code_size = size;
  }

  // linenumber table - note that length is unknown until decompression,
  // see class CompressedLineNumberReadStream.
  // 压缩的代码行号表
  u_char* compressed_linenumber_table() const;         // not preserved by gc
  u2* generic_signature_index_addr() const;
  u2* checked_exceptions_length_addr() const;
  u2* localvariable_table_length_addr() const;
  u2* exception_table_length_addr() const;
  u2* method_parameters_length_addr() const;

  // checked exceptions
  // 异常检查表
  int checked_exceptions_length() const;
  CheckedExceptionElement* checked_exceptions_start() const;

  // localvariable table
  // 本地变量表
  int localvariable_table_length() const;
  LocalVariableTableElement* localvariable_table_start() const;

  // exception table
  // 异常表
  int exception_table_length() const;
  ExceptionTableElement* exception_table_start() const;

  // method parameters table
  // 方法参数
  int method_parameters_length() const;
  MethodParametersElement* method_parameters_start() const;

  // method annotations
  // 方法注解
  bool has_method_annotations() const
    { return (_flags & _has_method_annotations) != 0; }

  bool has_parameter_annotations() const
    { return (_flags & _has_parameter_annotations) != 0; }

  bool has_type_annotations() const
    { return (_flags & _has_type_annotations) != 0; }

  bool has_default_annotations() const
    { return (_flags & _has_default_annotations) != 0; }


  AnnotationArray** method_annotations_addr() const;
  AnnotationArray* method_annotations() const  {
    return has_method_annotations() ? *(method_annotations_addr()) : NULL;
  }
  void set_method_annotations(AnnotationArray* anno) {
    *(method_annotations_addr()) = anno;
  }

  AnnotationArray** parameter_annotations_addr() const;
  AnnotationArray* parameter_annotations() const {
    return has_parameter_annotations() ? *(parameter_annotations_addr()) : NULL;
  }
  void set_parameter_annotations(AnnotationArray* anno) {
    *(parameter_annotations_addr()) = anno;
  }

  AnnotationArray** type_annotations_addr() const;
  AnnotationArray* type_annotations() const {
    return has_type_annotations() ? *(type_annotations_addr()) : NULL;
  }
  void set_type_annotations(AnnotationArray* anno) {
    *(type_annotations_addr()) = anno;
  }

  AnnotationArray** default_annotations_addr() const;
  AnnotationArray* default_annotations() const {
    return has_default_annotations() ? *(default_annotations_addr()) : NULL;
  }
  void set_default_annotations(AnnotationArray* anno) {
    *(default_annotations_addr()) = anno;
  }

  int method_annotations_length() const {
    return has_method_annotations() ? method_annotations()->length() : 0;
  }
  int parameter_annotations_length() const {
    return has_parameter_annotations() ? parameter_annotations()->length() : 0;
  }
  int type_annotations_length() const {
    return has_type_annotations() ? type_annotations()->length() : 0;
  }
  int default_annotations_length() const {
    return has_default_annotations() ? default_annotations()->length() : 0;
  }

  // Copy annotations from other ConstMethod
  void copy_annotations_from(ConstMethod* cm);

  // byte codes
  void    set_code(address code) {
    if (code_size() > 0) {
      memcpy(code_base(), code, code_size());
    }
  }
  // 存储在ConstMethod本身占用的内存之后
  address code_base() const            { return (address) (this+1); }
  address code_end() const             { return code_base() + code_size(); }
  bool    contains(address bcp) const  { return code_base() <= bcp
                                                     && bcp < code_end(); }
  // Offset to bytecodes
  static ByteSize codes_offset()
                            { return in_ByteSize(sizeof(ConstMethod)); }

  static ByteSize constants_offset()
                            { return byte_offset_of(ConstMethod, _constants); }

  static ByteSize max_stack_offset()
                            { return byte_offset_of(ConstMethod, _max_stack); }
  static ByteSize size_of_locals_offset()
                            { return byte_offset_of(ConstMethod, _max_locals); }
  static ByteSize size_of_parameters_offset()
                            { return byte_offset_of(ConstMethod, _size_of_parameters); }


  // Unique id for the method
  static const u2 MAX_IDNUM;
  static const u2 UNSET_IDNUM;
  u2 method_idnum() const                        { return _method_idnum; }
  void set_method_idnum(u2 idnum)                { _method_idnum = idnum; }

  // max stack
  int  max_stack() const                         { return _max_stack; }
  void set_max_stack(int size)                   { _max_stack = size; }

  // max locals
  int  max_locals() const                        { return _max_locals; }
  void set_max_locals(int size)                  { _max_locals = size; }

  // size of parameters
  int  size_of_parameters() const                { return _size_of_parameters; }
  void set_size_of_parameters(int size)          { _size_of_parameters = size; }

  // Deallocation for RedefineClasses
  void deallocate_contents(ClassLoaderData* loader_data);
  bool is_klass() const { return false; }
  DEBUG_ONLY(bool on_stack() { return false; })

private:
  // Since the size of the compressed line number table is unknown, the
  // offsets of the other variable sized sections are computed backwards
  // from the end of the ConstMethod*.

  // First byte after ConstMethod*
  address constMethod_end() const
                          { return (address)((intptr_t*)this + _constMethod_size); }

  // Last short in ConstMethod*
  u2* last_u2_element() const;

 public:
  // Printing
  void print_on      (outputStream* st) const;
  void print_value_on(outputStream* st) const;

  const char* internal_name() const { return "{constMethod}"; }

  // Verify
  void verify_on(outputStream* st);
};

#endif // SHARE_VM_OOPS_CONSTMETHODOOP_HPP
