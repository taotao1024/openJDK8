/*
 * Copyright (c) 1997, 2014, Oracle and/or its affiliates. All rights reserved.
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

#ifndef SHARE_VM_OOPS_INSTANCEKLASS_HPP
#define SHARE_VM_OOPS_INSTANCEKLASS_HPP

#include "classfile/classLoaderData.hpp"
#include "memory/referenceType.hpp"
#include "oops/annotations.hpp"
#include "oops/constMethod.hpp"
#include "oops/fieldInfo.hpp"
#include "oops/instanceOop.hpp"
#include "oops/klassVtable.hpp"
#include "runtime/atomic.hpp"
#include "runtime/handles.hpp"
#include "runtime/os.hpp"
#include "utilities/accessFlags.hpp"
#include "utilities/bitMap.inline.hpp"
#include "utilities/macros.hpp"
#include "trace/traceMacros.hpp"

// An InstanceKlass is the VM level representation of a Java class.
// It contains all information needed for at class at execution runtime.

//  InstanceKlass layout:
//    [C++ vtbl pointer           ] Klass
//    [subtype cache              ] Klass
//    [instance size              ] Klass
//    [java mirror                ] Klass
//    [super                      ] Klass
//    [access_flags               ] Klass
//    [name                       ] Klass
//    [first subklass             ] Klass
//    [next sibling               ] Klass
//    [array klasses              ]
//    [methods                    ]
//    [local interfaces           ]
//    [transitive interfaces      ]
//    [fields                     ]
//    [constants                  ]
//    [class loader               ]
//    [source file name           ]
//    [inner classes              ]
//    [static field size          ]
//    [nonstatic field size       ]
//    [static oop fields size     ]
//    [nonstatic oop maps size    ]
//    [has finalize method        ]
//    [deoptimization mark bit    ]
//    [initialization state       ]
//    [initializing thread        ]
//    [Java vtable length         ]
//    [oop map cache (stack maps) ]
//    [EMBEDDED Java vtable             ] size in words = vtable_len
//    [EMBEDDED nonstatic oop-map blocks] size in words = nonstatic_oop_map_size
//      The embedded nonstatic oop-map blocks are short pairs (offset, length)
//      indicating where oops are located in instances of this klass.
//    [EMBEDDED implementor of the interface] only exist for interface
//    [EMBEDDED host klass        ] only exist for an anonymous class (JSR 292 enabled)


// forward declaration for class -- see below for definition
class SuperTypeClosure;
class JNIid;
class jniIdMapBase;
class BreakpointInfo;
class fieldDescriptor;
class DepChange;
class nmethodBucket;
class PreviousVersionNode;
class JvmtiCachedClassFieldMap;
class MemberNameTable;

// This is used in iterators below.
class FieldClosure: public StackObj {
public:
  virtual void do_field(fieldDescriptor* fd) = 0;
};

#ifndef PRODUCT
// Print fields.
// If "obj" argument to constructor is NULL, prints static fields, otherwise prints non-static fields.
class FieldPrinter: public FieldClosure {
   oop _obj;
   outputStream* _st;
 public:
   FieldPrinter(outputStream* st, oop obj = NULL) : _obj(obj), _st(st) {}
   void do_field(fieldDescriptor* fd);
};
#endif  // !PRODUCT

// ValueObjs embedded in klass. Describes where oops are located in instances of
// this klass.
class OopMapBlock VALUE_OBJ_CLASS_SPEC {
 public:
  // Byte offset of the first oop mapped by this block.
  int offset() const          { return _offset; }
  void set_offset(int offset) { _offset = offset; }

  // Number of oops in this block.
  uint count() const         { return _count; }
  void set_count(uint count) { _count = count; }

  // sizeof(OopMapBlock) in HeapWords.
  // 计算OopMapBlock本身占用的内存空间，在64位系统中为一个字
  static const int size_in_words() {
    return align_size_up(int(sizeof(OopMapBlock)), HeapWordSize) >>
      LogHeapWordSize;
  }

 private:
  int  _offset; // 第一个所引用的oop相对于当前oop地址的偏移量
  uint _count;  // 有count个连续存放的oop
};

struct JvmtiCachedClassFileData;

class InstanceKlass: public Klass {
  friend class VMStructs;
  friend class ClassFileParser;
  friend class CompileReplay;

 protected:
  // Constructor
  InstanceKlass(int vtable_len,
                int itable_len,
                int static_field_size,
                int nonstatic_oop_map_size,
                ReferenceType rt,
                AccessFlags access_flags,
                bool is_anonymous);
  /**
   * 下HotSpot VM创建Klass类的实例的过程
   * InstanceKlass::allocate_instance_klass()函数创建InstanceKlass实例。
   * 在创建时首先需要分配内存，这涉及C++对new运算符重载的调用，通过重载new运算符的函数为对象分配内存空间，
   * 然后再调用类的构造函数初始化相关的属性。
   */
 public:
  static InstanceKlass* allocate_instance_klass(
                                          ClassLoaderData* loader_data,
                                          int vtable_len,
                                          int itable_len,
                                          int static_field_size,
                                          int nonstatic_oop_map_size,
                                          ReferenceType rt,
                                          AccessFlags access_flags,
                                          Symbol* name,
                                          Klass* super_klass,
                                          bool is_anonymous,
                                          TRAPS);

  InstanceKlass() { assert(DumpSharedSpaces || UseSharedSpaces, "only for CDS"); }

  // See "The Java Virtual Machine Specification" section 2.16.2-5 for a detailed description
  // of the class loading & initialization procedure, and the use of the states.
  enum ClassState {
    // 已经分配内存，在InstanceKlass的构造函数中通常会将_init_state初始化为这个状态。
    allocated,                          // allocated (but not yet linked)
    // 表示类已经装载并且已经插入继承体系
    // 中，在SystemDictionary::add_to_hierarchy()函
    // 数中会更新InstanceKlass的_init_state属性为此状态。
    loaded,                             // loaded and inserted in class hierarchy (but not linked yet)
    // 类已经连接，但还没有初始化
    // 表示已经成功连接/校验，只在InstanceKlass::link_class_impl()方法中更新为这个状态。
    linked,                             // successfully linked/verified (but not initialized yet)
    // being_initialized、fully_initialized与initialization_error：在类的初始化函数
    // Instance-Klass::initialize_impl()中会用到，
    // 分别表示类的初始化过程中的不同状态——正在初始化、已经完成初始化和初始化出错，
    // 函数会根据不同的状态执行不同的逻辑。
    // 正在进行类的初始化
    being_initialized,                  // currently running class initializer
    // 完成类的初始化
    fully_initialized,                  // initialized (successfull final state)
    // 在初始化的过程中出错
    initialization_error                // error happened during initialization
  };

  static int number_of_instance_classes() { return _total_instanceKlass_count; }

 private:
  static volatile int _total_instanceKlass_count;

 protected:
  // Annotations for this class
  Annotations*    _annotations;
  // Array classes holding elements of this class.
  // 数组元素为该类型的数组Klass指针
  Klass*          _array_klasses;
  // Constant pool for this class.
  // ConstantPool常量池类型的指针 用来指向保存常量池信息的ConstantPool实例
  ConstantPool* _constants;
  // The InnerClasses attribute and EnclosingMethod attribute. The
  // _inner_classes is an array of shorts. If the class has InnerClasses
  // attribute, then the _inner_classes array begins with 4-tuples of shorts
  // [inner_class_info_index, outer_class_info_index,
  // inner_name_index, inner_class_access_flags] for the InnerClasses
  // attribute. If the EnclosingMethod attribute exists, it occupies the
  // last two shorts [class_index, method_index] of the array. If only
  // the InnerClasses attribute exists, the _inner_classes array length is
  // number_of_inner_classes * 4. If the class has both InnerClasses
  // and EnclosingMethod attributes the _inner_classes array length is
  // number_of_inner_classes * 4 + enclosing_method_attribute_size.
  Array<jushort>* _inner_classes;

  // the source debug extension for this klass, NULL if not specified.
  // Specified as UTF-8 string without terminating zero byte in the classfile,
  // it is stored in the instanceklass as a NULL-terminated UTF-8 string
  char*           _source_debug_extension;
  // Array name derived from this class which needs unreferencing
  // if this class is unloaded.
  // 以该类型作为数组组件类型（指的是数组去掉一个维度的类型）的数组名称
  // 如果当前instanceKlass实体表示Object类，则名称为"[Ljava/lang/Object"
  Symbol*         _array_name;

  // Number of heapOopSize words used by non-static fields in this klass
  // (including inherited fields but after header_size()).
  // 非静态字段需要占用的内存空间 以字为单位
  // 在为该InstanceKlass实例表示的Java类所创建的对象(oop)分配内存时，会参考此属性的值分配内存
  // 在类解析时会事先计算好这个值
  int             _nonstatic_field_size;
  // 静态字段需要占用的内存空间 以字为单位
  int             _static_field_size;    // number words used by static fields (oop and non-oop) in this klass
  // Constant pool index to the utf8 entry of the Generic signature,
  // or 0 if none.
  // 保存Java类的签名 在常量池中的索引
  u2              _generic_signature_index;
  // Constant pool index to the utf8 entry for the name of source file
  // containing this klass, 0 if not specified.
  // 保存Java类的源文件名 在常量池中的索引
  u2              _source_file_name_index;
  // Java类包含的静态引用类型字段的数量
  u2              _static_oop_field_count;// number of static oop fields in this klass
  // Java类包含的字段总数量
  u2              _java_fields_count;    // The number of declared Java fields
  // OopMapBlock需要占用的内存空间以字为单位。
  // OopMapBlock使用<偏移量, 数量>描述Java类(InstanceKlass实例)中的各个非静态对象类型的变量在
  // Java对象中的具体位置，这样垃圾回收时就能找到Java对象中引用的其他对象
  int             _nonstatic_oop_map_size;// size in words of nonstatic oop map blocks

  // _is_marked_dependent can be set concurrently, thus cannot be part of the
  // _misc_flags.
  bool            _is_marked_dependent;  // used for marking during flushing and deoptimization
  bool            _has_unloaded_dependent;

  enum {
    _misc_rewritten                = 1 << 0, // methods rewritten.
    _misc_has_nonstatic_fields     = 1 << 1, // for sizing with UseCompressedOops
    _misc_should_verify_class      = 1 << 2, // allow caching of preverification
    _misc_is_anonymous             = 1 << 3, // has embedded _host_klass field
    _misc_is_contended             = 1 << 4, // marked with contended annotation
    _misc_has_default_methods      = 1 << 5, // class/superclass/implemented interfaces has default methods
    _misc_declares_default_methods = 1 << 6  // directly declares default methods (any access)
  };
  u2              _misc_flags;
  // 类的次版本号
  u2              _minor_version;        // minor version number of class file
  // 类的主版本号
  u2              _major_version;        // major version number of class file
  // 执行Java类初始化的Thread指针
  Thread*         _init_thread;          // Pointer to current thread doing initialization (to handle recusive initialization)
  // Java虚函数表(vtable)所占用的内存空间 以字为单位
  int             _vtable_len;           // length of Java vtable (in words)
  // Java接口函数表(itable)所占用的内存空间 以字为单位
  int             _itable_len;           // length of Java itable (in words)
  OopMapCache*    volatile _oop_map_cache;   // OopMapCache for all methods in the klass (allocated lazily)
  MemberNameTable* _member_names;        // Member names
  JNIid*          _jni_ids;              // First JNI identifier for static fields in this class
  jmethodID*      _methods_jmethod_ids;  // jmethodIDs corresponding to method_idnum, or NULL if none
  nmethodBucket*  _dependencies;         // list of dependent nmethods
  nmethod*        _osr_nmethods_head;    // Head of list of on-stack replacement nmethods for this class
  BreakpointInfo* _breakpoints;          // bpt lists, managed by Method*
  // Array of interesting part(s) of the previous version(s) of this
  // InstanceKlass. See PreviousVersionWalker below.
  GrowableArray<PreviousVersionNode *>* _previous_versions;
  // JVMTI fields can be moved to their own structure - see 6315920
  // JVMTI: cached class file, before retransformable agent modified it in CFLH
  JvmtiCachedClassFileData* _cached_class_file;

  volatile u2     _idnum_allocated_count;         // JNI/JVMTI: increments with the addition of methods, old ids don't change

  // Class states are defined as ClassState (see above).
  // Place the _init_state here to utilize the unused 2-byte after
  // _idnum_allocated_count.
  // 表示类的状态，为枚举类型ClassState定义了如下的常量值：
  // allocated              已分配内存
  // loaded                 读取Class文件信息并加载到内存中
  // linked                 已连接成功和校验
  // being_initialized      正在初始化
  // fully_initialized      已经完成初始化
  // initialization_error   从初始化发生错误
  u1              _init_state;                    // state of class
  // 引用类型 表示当前的InstanceKlass实例的引用类型 可能是强引用、软引用、弱引用等
  u1              _reference_type;                // reference type

  JvmtiCachedClassFieldMap* _jvmti_cached_class_field_map;  // JVMTI: used during heap iteration

  NOT_PRODUCT(int _verify_count;)  // to avoid redundant verifies

  // Method array.
  // 保存方法的指针数组
  Array<Method*>* _methods;
  // Default Method Array, concrete methods inherited from interfaces
  // 保存方法的指针数组，是从接口中继承的默认方法
  Array<Method*>* _default_methods;
  // Interface (Klass*s) this class declares locally to implement.
  // 保存方法的指针数组，时直接实现的接口Klass
  Array<Klass*>* _local_interfaces;
  // Interface (Klass*s) this class implements transitively.
  // 保存接口的指针数组，包含_local_interfaces和间接实现的接口
  Array<Klass*>* _transitive_interfaces;
  // Int array containing the original order of method in the class file (for JVMTI).
  Array<int>*     _method_ordering;
  // Int array containing the vtable_indices for default_methods
  // offset matches _default_methods offset
  // 默认方法在虚函数表中的索引
  Array<int>*     _default_vtable_indices;

  // Instance and static variable information, starts with 6-tuples of shorts
  // [access, name index, sig index, initval index, low_offset, high_offset]
  // for all fields, followed by the generic signature data at the end of
  // the array. Only fields with generic signature attributes have the generic
  // signature data set in the array. The fields array looks like following:
  //
  // f1: [access, name index, sig index, initial value index, low_offset, high_offset]
  // f2: [access, name index, sig index, initial value index, low_offset, high_offset]
  //      ...
  // fn: [access, name index, sig index, initial value index, low_offset, high_offset]
  //     [generic signature index]
  //     [generic signature index]
  //     ...
  // 类的字段属性 每个字段有6个属性 分别是 access、name index、sig index、initial value index、low_offset、hign_offset
  // 他们组成一个元组
  // access                    表示访问控制属性
  // name index                获取属性名
  // initial value index       获取初始值
  // low_offset、hign_offset   获取该属性在内存中的偏移量
  Array<u2>*      _fields;

  // embedded Java vtable follows here
  // embedded Java itables follows here
  // embedded static fields follows here
  // embedded nonstatic oop-map blocks follows here
  // embedded implementor of this interface follows here
  //   The embedded implementor only exists if the current klass is an
  //   iterface. The possible values of the implementor fall into following
  //   three cases:
  //     NULL: no implementor.
  //     A Klass* that's not itself: one implementor.
  //     Itself: more than one implementors.
  // embedded host klass follows here
  //   The embedded host klass only exists in an anonymous class for
  //   dynamic language support (JSR 292 enabled). The host class grants
  //   its access privileges to this class also. The host class is either
  //   named, or a previously loaded anonymous class. A non-anonymous class
  //   or an anonymous class loaded through normal classloading does not
  //   have this embedded field.
  //

  friend class SystemDictionary;

 public:
  bool has_nonstatic_fields() const        {
    return (_misc_flags & _misc_has_nonstatic_fields) != 0;
  }
  void set_has_nonstatic_fields(bool b)    {
    if (b) {
      _misc_flags |= _misc_has_nonstatic_fields;
    } else {
      _misc_flags &= ~_misc_has_nonstatic_fields;
    }
  }

  // field sizes
  int nonstatic_field_size() const         { return _nonstatic_field_size; }
  void set_nonstatic_field_size(int size)  { _nonstatic_field_size = size; }

  int static_field_size() const            { return _static_field_size; }
  void set_static_field_size(int size)     { _static_field_size = size; }

  int static_oop_field_count() const       { return (int)_static_oop_field_count; }
  void set_static_oop_field_count(u2 size) { _static_oop_field_count = size; }

  // Java vtable
  int  vtable_length() const               { return _vtable_len; }
  void set_vtable_length(int len)          { _vtable_len = len; }

  // Java itable
  int  itable_length() const               { return _itable_len; }
  void set_itable_length(int len)          { _itable_len = len; }

  // array klasses
  Klass* array_klasses() const             { return _array_klasses; }
  void set_array_klasses(Klass* k)         { _array_klasses = k; }

  // methods
  Array<Method*>* methods() const          { return _methods; }
  void set_methods(Array<Method*>* a)      { _methods = a; }
  Method* method_with_idnum(int idnum);

  // method ordering
  Array<int>* method_ordering() const     { return _method_ordering; }
  void set_method_ordering(Array<int>* m) { _method_ordering = m; }
  void copy_method_ordering(intArray* m, TRAPS);

  // default_methods
  Array<Method*>* default_methods() const  { return _default_methods; }
  void set_default_methods(Array<Method*>* a) { _default_methods = a; }

  // default method vtable_indices
  Array<int>* default_vtable_indices() const { return _default_vtable_indices; }
  void set_default_vtable_indices(Array<int>* v) { _default_vtable_indices = v; }
  Array<int>* create_new_default_vtable_indices(int len, TRAPS);

  // interfaces
  Array<Klass*>* local_interfaces() const          { return _local_interfaces; }
  void set_local_interfaces(Array<Klass*>* a)      {
    guarantee(_local_interfaces == NULL || a == NULL, "Just checking");
    _local_interfaces = a; }

  Array<Klass*>* transitive_interfaces() const     { return _transitive_interfaces; }
  void set_transitive_interfaces(Array<Klass*>* a) {
    guarantee(_transitive_interfaces == NULL || a == NULL, "Just checking");
    _transitive_interfaces = a;
  }

 private:
  friend class fieldDescriptor;
  FieldInfo* field(int index) const { return FieldInfo::from_field_array(_fields, index); }

 public:
  int     field_offset      (int index) const { return field(index)->offset(); }
  int     field_access_flags(int index) const { return field(index)->access_flags(); }
  Symbol* field_name        (int index) const { return field(index)->name(constants()); }
  Symbol* field_signature   (int index) const { return field(index)->signature(constants()); }

  // Number of Java declared fields
  int java_fields_count() const           { return (int)_java_fields_count; }

  Array<u2>* fields() const            { return _fields; }
  void set_fields(Array<u2>* f, u2 java_fields_count) {
    guarantee(_fields == NULL || f == NULL, "Just checking");
    _fields = f;
    _java_fields_count = java_fields_count;
  }

  // inner classes
  Array<u2>* inner_classes() const       { return _inner_classes; }
  void set_inner_classes(Array<u2>* f)   { _inner_classes = f; }

  enum InnerClassAttributeOffset {
    // From http://mirror.eng/products/jdk/1.1/docs/guide/innerclasses/spec/innerclasses.doc10.html#18814
    inner_class_inner_class_info_offset = 0,
    inner_class_outer_class_info_offset = 1,
    inner_class_inner_name_offset = 2,
    inner_class_access_flags_offset = 3,
    inner_class_next_offset = 4
  };

  enum EnclosingMethodAttributeOffset {
    enclosing_method_class_index_offset = 0,
    enclosing_method_method_index_offset = 1,
    enclosing_method_attribute_size = 2
  };

  // method override check
  bool is_override(methodHandle super_method, Handle targetclassloader, Symbol* targetclassname, TRAPS);

  // package
  bool is_same_class_package(Klass* class2);
  bool is_same_class_package(oop classloader2, Symbol* classname2);
  static bool is_same_class_package(oop class_loader1, Symbol* class_name1, oop class_loader2, Symbol* class_name2);

  // find an enclosing class (defined where original code was, in jvm.cpp!)
  Klass* compute_enclosing_class(bool* inner_is_member, TRAPS) {
    instanceKlassHandle self(THREAD, this);
    return compute_enclosing_class_impl(self, inner_is_member, THREAD);
  }
  static Klass* compute_enclosing_class_impl(instanceKlassHandle self,
                                               bool* inner_is_member, TRAPS);

  // tell if two classes have the same enclosing class (at package level)
  bool is_same_package_member(Klass* class2, TRAPS) {
    instanceKlassHandle self(THREAD, this);
    return is_same_package_member_impl(self, class2, THREAD);
  }
  static bool is_same_package_member_impl(instanceKlassHandle self,
                                          Klass* class2, TRAPS);

  // initialization state
  bool is_loaded() const                   { return _init_state >= loaded; }
  bool is_linked() const                   { return _init_state >= linked; }
  bool is_initialized() const              { return _init_state == fully_initialized; }
  bool is_not_initialized() const          { return _init_state <  being_initialized; }
  bool is_being_initialized() const        { return _init_state == being_initialized; }
  bool is_in_error_state() const           { return _init_state == initialization_error; }
  bool is_reentrant_initialization(Thread *thread)  { return thread == _init_thread; }
  ClassState  init_state()                 { return (ClassState)_init_state; }
  bool is_rewritten() const                { return (_misc_flags & _misc_rewritten) != 0; }

  // defineClass specified verification
  bool should_verify_class() const         {
    return (_misc_flags & _misc_should_verify_class) != 0;
  }
  void set_should_verify_class(bool value) {
    if (value) {
      _misc_flags |= _misc_should_verify_class;
    } else {
      _misc_flags &= ~_misc_should_verify_class;
    }
  }

  // marking
  bool is_marked_dependent() const         { return _is_marked_dependent; }
  void set_is_marked_dependent(bool value) { _is_marked_dependent = value; }

  bool has_unloaded_dependent() const         { return _has_unloaded_dependent; }
  void set_has_unloaded_dependent(bool value) { _has_unloaded_dependent = value; }

  // initialization (virtuals from Klass)
  bool should_be_initialized() const;  // means that initialize should be called
  void initialize(TRAPS);
  void link_class(TRAPS);
  bool link_class_or_fail(TRAPS); // returns false on failure
  void unlink_class();
  void rewrite_class(TRAPS);
  void link_methods(TRAPS);
  Method* class_initializer();

  // set the class to initialized if no static initializer is present
  void eager_initialize(Thread *thread);

  // reference type
  ReferenceType reference_type() const     { return (ReferenceType)_reference_type; }
  void set_reference_type(ReferenceType t) {
    assert(t == (u1)t, "overflow");
    _reference_type = (u1)t;
  }

  static ByteSize reference_type_offset() { return in_ByteSize(offset_of(InstanceKlass, _reference_type)); }

  // find local field, returns true if found
  bool find_local_field(Symbol* name, Symbol* sig, fieldDescriptor* fd) const;
  // find field in direct superinterfaces, returns the interface in which the field is defined
  Klass* find_interface_field(Symbol* name, Symbol* sig, fieldDescriptor* fd) const;
  // find field according to JVM spec 5.4.3.2, returns the klass in which the field is defined
  Klass* find_field(Symbol* name, Symbol* sig, fieldDescriptor* fd) const;
  // find instance or static fields according to JVM spec 5.4.3.2, returns the klass in which the field is defined
  Klass* find_field(Symbol* name, Symbol* sig, bool is_static, fieldDescriptor* fd) const;

  // find a non-static or static field given its offset within the class.
  bool contains_field_offset(int offset) {
    return instanceOopDesc::contains_field_offset(offset, nonstatic_field_size());
  }

  bool find_local_field_from_offset(int offset, bool is_static, fieldDescriptor* fd) const;
  bool find_field_from_offset(int offset, bool is_static, fieldDescriptor* fd) const;

  // find a local method (returns NULL if not found)
  Method* find_method(Symbol* name, Symbol* signature) const;
  static Method* find_method(Array<Method*>* methods, Symbol* name, Symbol* signature);

  // find a local method, but skip static methods
  Method* find_instance_method(Symbol* name, Symbol* signature);
  static Method* find_instance_method(Array<Method*>* methods, Symbol* name, Symbol* signature);

  // true if method matches signature and conforms to skipping_X conditions.
  static bool method_matches(Method* m, Symbol* signature, bool skipping_overpass, bool skipping_static);

  // find a local method index in default_methods (returns -1 if not found)
  static int find_method_index(Array<Method*>* methods, Symbol* name, Symbol* signature, bool skipping_overpass, bool skipping_static);

  // lookup operation (returns NULL if not found)
  Method* uncached_lookup_method(Symbol* name, Symbol* signature, MethodLookupMode mode) const;

  // lookup a method in all the interfaces that this class implements
  // (returns NULL if not found)
  Method* lookup_method_in_all_interfaces(Symbol* name, Symbol* signature, MethodLookupMode mode) const;

  // lookup a method in local defaults then in all interfaces
  // (returns NULL if not found)
  Method* lookup_method_in_ordered_interfaces(Symbol* name, Symbol* signature) const;

  // Find method indices by name.  If a method with the specified name is
  // found the index to the first method is returned, and 'end' is filled in
  // with the index of first non-name-matching method.  If no method is found
  // -1 is returned.
  int find_method_by_name(Symbol* name, int* end);
  static int find_method_by_name(Array<Method*>* methods, Symbol* name, int* end);

  // constant pool
  ConstantPool* constants() const        { return _constants; }
  void set_constants(ConstantPool* c)    { _constants = c; }

  // protection domain
  oop protection_domain() const;

  // signers
  objArrayOop signers() const;

  // host class
  Klass* host_klass() const              {
    Klass** hk = (Klass**)adr_host_klass();
    if (hk == NULL) {
      return NULL;
    } else {
      assert(*hk != NULL, "host klass should always be set if the address is not null");
      return *hk;
    }
  }
  void set_host_klass(Klass* host)            {
    assert(is_anonymous(), "not anonymous");
    Klass** addr = (Klass**)adr_host_klass();
    assert(addr != NULL, "no reversed space");
    if (addr != NULL) {
      *addr = host;
    }
  }
  bool is_anonymous() const                {
    return (_misc_flags & _misc_is_anonymous) != 0;
  }
  void set_is_anonymous(bool value)        {
    if (value) {
      _misc_flags |= _misc_is_anonymous;
    } else {
      _misc_flags &= ~_misc_is_anonymous;
    }
  }

  // Oop that keeps the metadata for this class from being unloaded
  // in places where the metadata is stored in other places, like nmethods
  oop klass_holder() const {
    return is_anonymous() ? java_mirror() : class_loader();
  }

  bool is_contended() const                {
    return (_misc_flags & _misc_is_contended) != 0;
  }
  void set_is_contended(bool value)        {
    if (value) {
      _misc_flags |= _misc_is_contended;
    } else {
      _misc_flags &= ~_misc_is_contended;
    }
  }

  // source file name
  Symbol* source_file_name() const               {
    return (_source_file_name_index == 0) ?
      (Symbol*)NULL : _constants->symbol_at(_source_file_name_index);
  }
  u2 source_file_name_index() const              {
    return _source_file_name_index;
  }
  void set_source_file_name_index(u2 sourcefile_index) {
    _source_file_name_index = sourcefile_index;
  }

  // minor and major version numbers of class file
  u2 minor_version() const                 { return _minor_version; }
  void set_minor_version(u2 minor_version) { _minor_version = minor_version; }
  u2 major_version() const                 { return _major_version; }
  void set_major_version(u2 major_version) { _major_version = major_version; }

  // source debug extension
  char* source_debug_extension() const     { return _source_debug_extension; }
  void set_source_debug_extension(char* array, int length);

  // symbol unloading support (refcount already added)
  Symbol* array_name()                     { return _array_name; }
  void set_array_name(Symbol* name)        { assert(_array_name == NULL  || name == NULL, "name already created"); _array_name = name; }

  // nonstatic oop-map blocks
  static int nonstatic_oop_map_size(unsigned int oop_map_count) {
    return oop_map_count * OopMapBlock::size_in_words();
  }
  unsigned int nonstatic_oop_map_count() const {
    return _nonstatic_oop_map_size / OopMapBlock::size_in_words();
  }
  int nonstatic_oop_map_size() const { return _nonstatic_oop_map_size; }
  void set_nonstatic_oop_map_size(int words) {
    _nonstatic_oop_map_size = words;
  }

  // RedefineClasses() support for previous versions:
  void add_previous_version(instanceKlassHandle ikh, BitMap *emcp_methods,
         int emcp_method_count);
  // If the _previous_versions array is non-NULL, then this klass
  // has been redefined at least once even if we aren't currently
  // tracking a previous version.
  bool has_been_redefined() const { return _previous_versions != NULL; }
  bool has_previous_version() const;
  void init_previous_versions() {
    _previous_versions = NULL;
  }
  GrowableArray<PreviousVersionNode *>* previous_versions() const {
    return _previous_versions;
  }

  static void purge_previous_versions(InstanceKlass* ik);

  // JVMTI: Support for caching a class file before it is modified by an agent that can do retransformation
  void set_cached_class_file(JvmtiCachedClassFileData *data) {
    _cached_class_file = data;
  }
  JvmtiCachedClassFileData * get_cached_class_file() { return _cached_class_file; }
  jint get_cached_class_file_len();
  unsigned char * get_cached_class_file_bytes();

  // JVMTI: Support for caching of field indices, types, and offsets
  void set_jvmti_cached_class_field_map(JvmtiCachedClassFieldMap* descriptor) {
    _jvmti_cached_class_field_map = descriptor;
  }
  JvmtiCachedClassFieldMap* jvmti_cached_class_field_map() const {
    return _jvmti_cached_class_field_map;
  }

  bool has_default_methods() const {
    return (_misc_flags & _misc_has_default_methods) != 0;
  }
  void set_has_default_methods(bool b) {
    if (b) {
      _misc_flags |= _misc_has_default_methods;
    } else {
      _misc_flags &= ~_misc_has_default_methods;
    }
  }

  bool declares_default_methods() const {
    return (_misc_flags & _misc_declares_default_methods) != 0;
  }
  void set_declares_default_methods(bool b) {
    if (b) {
      _misc_flags |= _misc_declares_default_methods;
    } else {
      _misc_flags &= ~_misc_declares_default_methods;
    }
  }

  // for adding methods, ConstMethod::UNSET_IDNUM means no more ids available
  inline u2 next_method_idnum();
  void set_initial_method_idnum(u2 value)             { _idnum_allocated_count = value; }

  // generics support
  Symbol* generic_signature() const                   {
    return (_generic_signature_index == 0) ?
      (Symbol*)NULL : _constants->symbol_at(_generic_signature_index);
  }
  u2 generic_signature_index() const                  {
    return _generic_signature_index;
  }
  void set_generic_signature_index(u2 sig_index)      {
    _generic_signature_index = sig_index;
  }

  u2 enclosing_method_data(int offset);
  u2 enclosing_method_class_index() {
    return enclosing_method_data(enclosing_method_class_index_offset);
  }
  u2 enclosing_method_method_index() {
    return enclosing_method_data(enclosing_method_method_index_offset);
  }
  void set_enclosing_method_indices(u2 class_index,
                                    u2 method_index);

  // jmethodID support
  static jmethodID get_jmethod_id(instanceKlassHandle ik_h,
                     methodHandle method_h);
  static jmethodID get_jmethod_id_fetch_or_update(instanceKlassHandle ik_h,
                     size_t idnum, jmethodID new_id, jmethodID* new_jmeths,
                     jmethodID* to_dealloc_id_p,
                     jmethodID** to_dealloc_jmeths_p);
  static void get_jmethod_id_length_value(jmethodID* cache, size_t idnum,
                size_t *length_p, jmethodID* id_p);
  jmethodID jmethod_id_or_null(Method* method);

  // annotations support
  Annotations* annotations() const          { return _annotations; }
  void set_annotations(Annotations* anno)   { _annotations = anno; }

  AnnotationArray* class_annotations() const {
    return (_annotations != NULL) ? _annotations->class_annotations() : NULL;
  }
  Array<AnnotationArray*>* fields_annotations() const {
    return (_annotations != NULL) ? _annotations->fields_annotations() : NULL;
  }
  AnnotationArray* class_type_annotations() const {
    return (_annotations != NULL) ? _annotations->class_type_annotations() : NULL;
  }
  Array<AnnotationArray*>* fields_type_annotations() const {
    return (_annotations != NULL) ? _annotations->fields_type_annotations() : NULL;
  }
  // allocation
  instanceOop allocate_instance(TRAPS);

  // additional member function to return a handle
  instanceHandle allocate_instance_handle(TRAPS)      { return instanceHandle(THREAD, allocate_instance(THREAD)); }

  objArrayOop allocate_objArray(int n, int length, TRAPS);
  // Helper function
  static instanceOop register_finalizer(instanceOop i, TRAPS);

  // Check whether reflection/jni/jvm code is allowed to instantiate this class;
  // if not, throw either an Error or an Exception.
  virtual void check_valid_for_instantiation(bool throwError, TRAPS);

  // initialization
  void call_class_initializer(TRAPS);
  void set_initialization_state_and_notify(ClassState state, TRAPS);

  // OopMapCache support
  OopMapCache* oop_map_cache()               { return _oop_map_cache; }
  void set_oop_map_cache(OopMapCache *cache) { _oop_map_cache = cache; }
  void mask_for(methodHandle method, int bci, InterpreterOopMap* entry);

  // JNI identifier support (for static fields - for jni performance)
  JNIid* jni_ids()                               { return _jni_ids; }
  void set_jni_ids(JNIid* ids)                   { _jni_ids = ids; }
  JNIid* jni_id_for(int offset);

  // maintenance of deoptimization dependencies
  int mark_dependent_nmethods(DepChange& changes);
  void add_dependent_nmethod(nmethod* nm);
  void remove_dependent_nmethod(nmethod* nm);

  // On-stack replacement support
  nmethod* osr_nmethods_head() const         { return _osr_nmethods_head; };
  void set_osr_nmethods_head(nmethod* h)     { _osr_nmethods_head = h; };
  void add_osr_nmethod(nmethod* n);
  void remove_osr_nmethod(nmethod* n);
  int mark_osr_nmethods(const Method* m);
  nmethod* lookup_osr_nmethod(const Method* m, int bci, int level, bool match_level) const;

  // Breakpoint support (see methods on Method* for details)
  BreakpointInfo* breakpoints() const       { return _breakpoints; };
  void set_breakpoints(BreakpointInfo* bps) { _breakpoints = bps; };

  // support for stub routines
  static ByteSize init_state_offset()  { return in_ByteSize(offset_of(InstanceKlass, _init_state)); }
  TRACE_DEFINE_OFFSET;
  static ByteSize init_thread_offset() { return in_ByteSize(offset_of(InstanceKlass, _init_thread)); }

  // subclass/subinterface checks
  bool implements_interface(Klass* k) const;
  bool is_same_or_direct_interface(Klass* k) const;

  // Access to the implementor of an interface.
  Klass* implementor() const
  {
    Klass** k = adr_implementor();
    if (k == NULL) {
      return NULL;
    } else {
      return *k;
    }
  }

  void set_implementor(Klass* k) {
    assert(is_interface(), "not interface");
    Klass** addr = adr_implementor();
    assert(addr != NULL, "null addr");
    if (addr != NULL) {
      *addr = k;
    }
  }

  int  nof_implementors() const       {
    Klass* k = implementor();
    if (k == NULL) {
      return 0;
    } else if (k != this) {
      return 1;
    } else {
      return 2;
    }
  }

  void add_implementor(Klass* k);  // k is a new class that implements this interface
  void init_implementor();           // initialize

  // link this class into the implementors list of every interface it implements
  void process_interfaces(Thread *thread);

  // virtual operations from Klass
  bool is_leaf_class() const               { return _subklass == NULL; }
  GrowableArray<Klass*>* compute_secondary_supers(int num_extra_slots);
  bool compute_is_subtype_of(Klass* k);
  bool can_be_primary_super_slow() const;
  int oop_size(oop obj)  const             { return size_helper(); }
  bool oop_is_instance_slow() const        { return true; }

  // Iterators
  void do_local_static_fields(FieldClosure* cl);
  void do_nonstatic_fields(FieldClosure* cl); // including inherited fields
  void do_local_static_fields(void f(fieldDescriptor*, Handle, TRAPS), Handle, TRAPS);

  void methods_do(void f(Method* method));
  void array_klasses_do(void f(Klass* k));
  void array_klasses_do(void f(Klass* k, TRAPS), TRAPS);
  bool super_types_do(SuperTypeClosure* blk);

  // Casting from Klass*
  static InstanceKlass* cast(Klass* k) {
    assert(k->is_klass(), "must be");
    assert(k->oop_is_instance(), "cast to InstanceKlass");
    return (InstanceKlass*) k;
  }

  InstanceKlass* java_super() const {
    return (super() == NULL) ? NULL : cast(super());
  }

  // Sizing (in words)
  /**
   * 调用header_size()函数计算InstanceKlass本身占用的内存空间
   */
  static int header_size()            {
    // HeapWordSize在64位系统下的值为8，也就是一个字的大小，同时也是一个非压缩指针占用的内存空间
    // 调用align_object_offset()函数进行内存对齐，方便对内存进行高效操作。
    return align_object_offset(sizeof(InstanceKlass)/HeapWordSize);
  }
  /**
   *  size()函数的返回值就是此次创建Klass实例所需要开辟的内存空间。
   */
  static int size(int vtable_length, int itable_length,
                  int nonstatic_oop_map_size,
                  bool is_interface, bool is_anonymous) {
           // InstanceKlass类本身占用的内存空间
    return align_object_size(header_size() +
           // Vtable占用的内存空间
           align_object_offset(vtable_length) +
           // Itable占用的内存
           align_object_offset(itable_length) +
           // OopMapBlock占用的内存空间
           ((is_interface || is_anonymous) ?
             align_object_offset(nonstatic_oop_map_size) :
             nonstatic_oop_map_size) +
           // 针对接口存储的信息
           (is_interface ? (int)sizeof(Klass*)/HeapWordSize : 0) +
           // 针对匿名类存储的信息
           (is_anonymous ? (int)sizeof(Klass*)/HeapWordSize : 0));
  }
  int size() const                    { return size(vtable_length(),
                                               itable_length(),
                                               nonstatic_oop_map_size(),
                                               is_interface(),
                                               is_anonymous());
  }
#if INCLUDE_SERVICES
  virtual void collect_statistics(KlassSizeStats *sz) const;
#endif

  static int vtable_start_offset()    { return header_size(); }
  static int vtable_length_offset()   { return offset_of(InstanceKlass, _vtable_len) / HeapWordSize; }

  intptr_t* start_of_vtable() const        { return ((intptr_t*)this) + vtable_start_offset(); }
  intptr_t* start_of_itable() const        { return start_of_vtable() + align_object_offset(vtable_length()); }
  int  itable_offset_in_words() const { return start_of_itable() - (intptr_t*)this; }

  intptr_t* end_of_itable() const          { return start_of_itable() + itable_length(); }

  address static_field_addr(int offset);

  OopMapBlock* start_of_nonstatic_oop_maps() const {
    return (OopMapBlock*)(start_of_itable() + align_object_offset(itable_length()));
  }

  Klass** end_of_nonstatic_oop_maps() const {
    return (Klass**)(start_of_nonstatic_oop_maps() +
                     nonstatic_oop_map_count());
  }

  Klass** adr_implementor() const {
    if (is_interface()) {
      return (Klass**)end_of_nonstatic_oop_maps();
    } else {
      return NULL;
    }
  };

  Klass** adr_host_klass() const {
    if (is_anonymous()) {
      Klass** adr_impl = adr_implementor();
      if (adr_impl != NULL) {
        return adr_impl + 1;
      } else {
        return end_of_nonstatic_oop_maps();
      }
    } else {
      return NULL;
    }
  }

  // Use this to return the size of an instance in heap words:
  int size_helper() const {
    // 调用layout_helper()函数获取Klass类中定义的_layout_helper属性的值
    // 然后调用layout_helper_to_size_helper()函数获取对象所需的内存空间。
    return layout_helper_to_size_helper(layout_helper());
  }

  // This bit is initialized in classFileParser.cpp.
  // It is false under any of the following conditions:
  //  - the class is abstract (including any interface)
  //  - the class has a finalizer (if !RegisterFinalizersAtInit)
  //  - the class size is larger than FastAllocateSizeLimit
  //  - the class is java/lang/Class, which cannot be allocated directly
  bool can_be_fastpath_allocated() const {
    return !layout_helper_needs_slow_path(layout_helper());
  }

  // Java vtable/itable
  klassVtable* vtable() const;        // return new klassVtable wrapper
  inline Method* method_at_vtable(int index);
  klassItable* itable() const;        // return new klassItable wrapper
  Method* method_at_itable(Klass* holder, int index, TRAPS);

#if INCLUDE_JVMTI
  void adjust_default_methods(Method** old_methods, Method** new_methods,
                              int methods_length, bool* trace_name_printed);
#endif // INCLUDE_JVMTI

  // Garbage collection
  void oop_follow_contents(oop obj);
  int  oop_adjust_pointers(oop obj);

  void clean_implementors_list(BoolObjectClosure* is_alive);
  void clean_method_data(BoolObjectClosure* is_alive);
  void clean_dependent_nmethods();

  // Explicit metaspace deallocation of fields
  // For RedefineClasses and class file parsing errors, we need to deallocate
  // instanceKlasses and the metadata they point to.
  void deallocate_contents(ClassLoaderData* loader_data);
  static void deallocate_methods(ClassLoaderData* loader_data,
                                 Array<Method*>* methods);
  void static deallocate_interfaces(ClassLoaderData* loader_data,
                                    Klass* super_klass,
                                    Array<Klass*>* local_interfaces,
                                    Array<Klass*>* transitive_interfaces);

  // The constant pool is on stack if any of the methods are executing or
  // referenced by handles.
  bool on_stack() const { return _constants->on_stack(); }

  // callbacks for actions during class unloading
  static void notify_unload_class(InstanceKlass* ik);
  static void release_C_heap_structures(InstanceKlass* ik);

  // Parallel Scavenge and Parallel Old
  PARALLEL_GC_DECLS

  // Naming
  const char* signature_name() const;

  // Iterators
  int oop_oop_iterate(oop obj, ExtendedOopClosure* blk) {
    return oop_oop_iterate_v(obj, blk);
  }

  int oop_oop_iterate_m(oop obj, ExtendedOopClosure* blk, MemRegion mr) {
    return oop_oop_iterate_v_m(obj, blk, mr);
  }

#define InstanceKlass_OOP_OOP_ITERATE_DECL(OopClosureType, nv_suffix)      \
  int  oop_oop_iterate##nv_suffix(oop obj, OopClosureType* blk);           \
  int  oop_oop_iterate##nv_suffix##_m(oop obj, OopClosureType* blk,        \
                                      MemRegion mr);

  ALL_OOP_OOP_ITERATE_CLOSURES_1(InstanceKlass_OOP_OOP_ITERATE_DECL)
  ALL_OOP_OOP_ITERATE_CLOSURES_2(InstanceKlass_OOP_OOP_ITERATE_DECL)

#if INCLUDE_ALL_GCS
#define InstanceKlass_OOP_OOP_ITERATE_BACKWARDS_DECL(OopClosureType, nv_suffix) \
  int  oop_oop_iterate_backwards##nv_suffix(oop obj, OopClosureType* blk);

  ALL_OOP_OOP_ITERATE_CLOSURES_1(InstanceKlass_OOP_OOP_ITERATE_BACKWARDS_DECL)
  ALL_OOP_OOP_ITERATE_CLOSURES_2(InstanceKlass_OOP_OOP_ITERATE_BACKWARDS_DECL)
#endif // INCLUDE_ALL_GCS

  u2 idnum_allocated_count() const      { return _idnum_allocated_count; }

public:
  void set_in_error_state() {
    assert(DumpSharedSpaces, "only call this when dumping archive");
    _init_state = initialization_error;
  }
  bool check_sharing_error_state();

private:
  // initialization state
#ifdef ASSERT
  void set_init_state(ClassState state);
#else
  void set_init_state(ClassState state) { _init_state = (u1)state; }
#endif
  void set_rewritten()                  { _misc_flags |= _misc_rewritten; }
  void set_init_thread(Thread *thread)  { _init_thread = thread; }

  // The RedefineClasses() API can cause new method idnums to be needed
  // which will cause the caches to grow. Safety requires different
  // cache management logic if the caches can grow instead of just
  // going from NULL to non-NULL.
  bool idnum_can_increment() const      { return has_been_redefined(); }
  jmethodID* methods_jmethod_ids_acquire() const
         { return (jmethodID*)OrderAccess::load_ptr_acquire(&_methods_jmethod_ids); }
  void release_set_methods_jmethod_ids(jmethodID* jmeths)
         { OrderAccess::release_store_ptr(&_methods_jmethod_ids, jmeths); }

  // Lock during initialization
public:
  // Lock for (1) initialization; (2) access to the ConstantPool of this class.
  // Must be one per class and it has to be a VM internal object so java code
  // cannot lock it (like the mirror).
  // It has to be an object not a Mutex because it's held through java calls.
  oop init_lock() const;
private:
  void fence_and_clear_init_lock();

  // Static methods that are used to implement member methods where an exposed this pointer
  // is needed due to possible GCs
  static bool link_class_impl                           (instanceKlassHandle this_oop, bool throw_verifyerror, TRAPS);
  static bool verify_code                               (instanceKlassHandle this_oop, bool throw_verifyerror, TRAPS);
  static void initialize_impl                           (instanceKlassHandle this_oop, TRAPS);
  static void initialize_super_interfaces               (instanceKlassHandle this_oop, TRAPS);
  static void eager_initialize_impl                     (instanceKlassHandle this_oop);
  static void set_initialization_state_and_notify_impl  (instanceKlassHandle this_oop, ClassState state, TRAPS);
  static void call_class_initializer_impl               (instanceKlassHandle this_oop, TRAPS);
  static Klass* array_klass_impl                      (instanceKlassHandle this_oop, bool or_null, int n, TRAPS);
  static void do_local_static_fields_impl               (instanceKlassHandle this_oop, void f(fieldDescriptor* fd, Handle, TRAPS), Handle, TRAPS);
  /* jni_id_for_impl for jfieldID only */
  static JNIid* jni_id_for_impl                         (instanceKlassHandle this_oop, int offset);

  // Returns the array class for the n'th dimension
  Klass* array_klass_impl(bool or_null, int n, TRAPS);

  // Returns the array class with this class as element type
  Klass* array_klass_impl(bool or_null, TRAPS);

  // find a local method (returns NULL if not found)
  Method* find_method_impl(Symbol* name, Symbol* signature, bool skipping_overpass) const;
  static Method* find_method_impl(Array<Method*>* methods, Symbol* name, Symbol* signature, bool skipping_overpass, bool skipping_static);

  // Free CHeap allocated fields.
  void release_C_heap_structures();
public:
  // CDS support - remove and restore oops from metadata. Oops are not shared.
  virtual void remove_unshareable_info();
  virtual void restore_unshareable_info(ClassLoaderData* loader_data, Handle protection_domain, TRAPS);

  // jvm support
  jint compute_modifier_flags(TRAPS) const;

  // JSR-292 support
  MemberNameTable* member_names() { return _member_names; }
  void set_member_names(MemberNameTable* member_names) { _member_names = member_names; }
  bool add_member_name(Handle member_name);

public:
  // JVMTI support
  jint jvmti_class_status() const;

 public:
  // Printing
#ifndef PRODUCT
  void print_on(outputStream* st) const;
#endif
  void print_value_on(outputStream* st) const;

  void oop_print_value_on(oop obj, outputStream* st);

#ifndef PRODUCT
  void oop_print_on      (oop obj, outputStream* st);

  void print_dependent_nmethods(bool verbose = false);
  bool is_dependent_nmethod(nmethod* nm);
#endif

  const char* internal_name() const;

  // Verification
  void verify_on(outputStream* st);

  void oop_verify_on(oop obj, outputStream* st);
};

inline Method* InstanceKlass::method_at_vtable(int index)  {
#ifndef PRODUCT
  assert(index >= 0, "valid vtable index");
  if (DebugVtables) {
    verify_vtable_index(index);
  }
#endif
  vtableEntry* ve = (vtableEntry*)start_of_vtable();
  return ve[index].method();
}

// for adding methods
// UNSET_IDNUM return means no more ids available
inline u2 InstanceKlass::next_method_idnum() {
  if (_idnum_allocated_count == ConstMethod::MAX_IDNUM) {
    return ConstMethod::UNSET_IDNUM; // no more ids available
  } else {
    return _idnum_allocated_count++;
  }
}


/* JNIid class for jfieldIDs only */
class JNIid: public CHeapObj<mtClass> {
  friend class VMStructs;
 private:
  Klass*             _holder;
  JNIid*             _next;
  int                _offset;
#ifdef ASSERT
  bool               _is_static_field_id;
#endif

 public:
  // Accessors
  Klass* holder() const           { return _holder; }
  int offset() const              { return _offset; }
  JNIid* next()                   { return _next; }
  // Constructor
  JNIid(Klass* holder, int offset, JNIid* next);
  // Identifier lookup
  JNIid* find(int offset);

  bool find_local_field(fieldDescriptor* fd) {
    return InstanceKlass::cast(holder())->find_local_field_from_offset(offset(), true, fd);
  }

  static void deallocate(JNIid* id);
  // Debugging
#ifdef ASSERT
  bool is_static_field_id() const { return _is_static_field_id; }
  void set_is_static_field_id()   { _is_static_field_id = true; }
#endif
  void verify(Klass* holder);
};


// If breakpoints are more numerous than just JVMTI breakpoints,
// consider compressing this data structure.
// It is currently a simple linked list defined in method.hpp.

class BreakpointInfo;


// A collection point for interesting information about the previous
// version(s) of an InstanceKlass.  A GrowableArray of PreviousVersionNodes
// is attached to the InstanceKlass as needed. See PreviousVersionWalker below.
class PreviousVersionNode : public CHeapObj<mtClass> {
 private:
  ConstantPool*    _prev_constant_pool;

  // If the previous version of the InstanceKlass doesn't have any
  // EMCP methods, then _prev_EMCP_methods will be NULL. If all the
  // EMCP methods have been collected, then _prev_EMCP_methods can
  // have a length of zero.
  GrowableArray<Method*>* _prev_EMCP_methods;

public:
  PreviousVersionNode(ConstantPool* prev_constant_pool,
                      GrowableArray<Method*>* prev_EMCP_methods);
  ~PreviousVersionNode();
  ConstantPool* prev_constant_pool() const {
    return _prev_constant_pool;
  }
  GrowableArray<Method*>* prev_EMCP_methods() const {
    return _prev_EMCP_methods;
  }
};


// Helper object for walking previous versions.
class PreviousVersionWalker : public StackObj {
 private:
  Thread*                               _thread;
  GrowableArray<PreviousVersionNode *>* _previous_versions;
  int                                   _current_index;

  // A pointer to the current node object so we can handle the deletes.
  PreviousVersionNode*                  _current_p;

  // The constant pool handle keeps all the methods in this class from being
  // deallocated from the metaspace during class unloading.
  constantPoolHandle                    _current_constant_pool_handle;

 public:
  PreviousVersionWalker(Thread* thread, InstanceKlass *ik);

  // Return the interesting information for the next previous version
  // of the klass. Returns NULL if there are no more previous versions.
  PreviousVersionNode* next_previous_version();
};


//
// nmethodBucket is used to record dependent nmethods for
// deoptimization.  nmethod dependencies are actually <klass, method>
// pairs but we really only care about the klass part for purposes of
// finding nmethods which might need to be deoptimized.  Instead of
// recording the method, a count of how many times a particular nmethod
// was recorded is kept.  This ensures that any recording errors are
// noticed since an nmethod should be removed as many times are it's
// added.
//
class nmethodBucket: public CHeapObj<mtClass> {
  friend class VMStructs;
 private:
  nmethod*       _nmethod;
  int            _count;
  nmethodBucket* _next;

 public:
  nmethodBucket(nmethod* nmethod, nmethodBucket* next) {
    _nmethod = nmethod;
    _next = next;
    _count = 1;
  }
  int count()                             { return _count; }
  int increment()                         { _count += 1; return _count; }
  int decrement();
  nmethodBucket* next()                   { return _next; }
  void set_next(nmethodBucket* b)         { _next = b; }
  nmethod* get_nmethod()                  { return _nmethod; }
};

// An iterator that's used to access the inner classes indices in the
// InstanceKlass::_inner_classes array.
class InnerClassesIterator : public StackObj {
 private:
  Array<jushort>* _inner_classes;
  int _length;
  int _idx;
 public:

  InnerClassesIterator(instanceKlassHandle k) {
    _inner_classes = k->inner_classes();
    if (k->inner_classes() != NULL) {
      _length = _inner_classes->length();
      // The inner class array's length should be the multiple of
      // inner_class_next_offset if it only contains the InnerClasses
      // attribute data, or it should be
      // n*inner_class_next_offset+enclosing_method_attribute_size
      // if it also contains the EnclosingMethod data.
      assert((_length % InstanceKlass::inner_class_next_offset == 0 ||
              _length % InstanceKlass::inner_class_next_offset == InstanceKlass::enclosing_method_attribute_size),
             "just checking");
      // Remove the enclosing_method portion if exists.
      if (_length % InstanceKlass::inner_class_next_offset == InstanceKlass::enclosing_method_attribute_size) {
        _length -= InstanceKlass::enclosing_method_attribute_size;
      }
    } else {
      _length = 0;
    }
    _idx = 0;
  }

  int length() const {
    return _length;
  }

  void next() {
    _idx += InstanceKlass::inner_class_next_offset;
  }

  bool done() const {
    return (_idx >= _length);
  }

  u2 inner_class_info_index() const {
    return _inner_classes->at(
               _idx + InstanceKlass::inner_class_inner_class_info_offset);
  }

  void set_inner_class_info_index(u2 index) {
    _inner_classes->at_put(
               _idx + InstanceKlass::inner_class_inner_class_info_offset, index);
  }

  u2 outer_class_info_index() const {
    return _inner_classes->at(
               _idx + InstanceKlass::inner_class_outer_class_info_offset);
  }

  void set_outer_class_info_index(u2 index) {
    _inner_classes->at_put(
               _idx + InstanceKlass::inner_class_outer_class_info_offset, index);
  }

  u2 inner_name_index() const {
    return _inner_classes->at(
               _idx + InstanceKlass::inner_class_inner_name_offset);
  }

  void set_inner_name_index(u2 index) {
    _inner_classes->at_put(
               _idx + InstanceKlass::inner_class_inner_name_offset, index);
  }

  u2 inner_access_flags() const {
    return _inner_classes->at(
               _idx + InstanceKlass::inner_class_access_flags_offset);
  }
};

#endif // SHARE_VM_OOPS_INSTANCEKLASS_HPP
