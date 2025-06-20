/*
 * Copyright (c) 2001, 2013, Oracle and/or its affiliates. All rights reserved.
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

#ifndef SHARE_VM_MEMORY_DEFNEWGENERATION_HPP
#define SHARE_VM_MEMORY_DEFNEWGENERATION_HPP

#include "gc_implementation/shared/ageTable.hpp"
#include "gc_implementation/shared/cSpaceCounters.hpp"
#include "gc_implementation/shared/generationCounters.hpp"
#include "gc_implementation/shared/copyFailedInfo.hpp"
#include "memory/generation.inline.hpp"
#include "utilities/stack.hpp"

class EdenSpace;
class ContiguousSpace;
class ScanClosure;
class STWGCTimer;

// DefNewGeneration is a young generation containing eden, from- and
// to-space.

class DefNewGeneration: public Generation {
  friend class VMStructs;

protected:
  // 当前代的下一个内存代，对于年轻代来说，下一个内存代就是老年代
  Generation* _next_gen;
  // 最大的晋升年龄
  uint        _tenuring_threshold;   // Tenuring threshold for next collection.
  ageTable    _age_table;
  // Size of object to pretenure in words; command line provides bytes
  // 当分配对象的内存大于以下值时，会被认为是大对象，直接在老年代中分配
  // 可通过-XX:PretenureSizeThreshold选项指定此值
  size_t      _pretenure_size_threshold_words;

  ageTable*   age_table() { return &_age_table; }

  // Initialize state to optimistically assume no promotion failure will
  // happen.
  void   init_assuming_no_promotion_failure();
  // True iff a promotion has failed in the current collection.
  bool   _promotion_failed;
  bool   promotion_failed() { return _promotion_failed; }
  PromotionFailedInfo _promotion_failed_info;

  // Handling promotion failure.  A young generation collection
  // can fail if a live object cannot be copied out of its
  // location in eden or from-space during the collection.  If
  // a collection fails, the young generation is left in a
  // consistent state such that it can be collected by a
  // full collection.
  //   Before the collection
  //     Objects are in eden or from-space
  //     All roots into the young generation point into eden or from-space.
  //
  //   After a failed collection
  //     Objects may be in eden, from-space, or to-space
  //     An object A in eden or from-space may have a copy B
  //       in to-space.  If B exists, all roots that once pointed
  //       to A must now point to B.
  //     All objects in the young generation are unmarked.
  //     Eden, from-space, and to-space will all be collected by
  //       the full collection.
  void handle_promotion_failure(oop);

  // In the absence of promotion failure, we wouldn't look at "from-space"
  // objects after a young-gen collection.  When promotion fails, however,
  // the subsequent full collection will look at from-space objects:
  // therefore we must remove their forwarding pointers.
  void remove_forwarding_pointers();

  // Preserve the mark of "obj", if necessary, in preparation for its mark
  // word being overwritten with a self-forwarding-pointer.
  void   preserve_mark_if_necessary(oop obj, markOop m);
  void   preserve_mark(oop obj, markOop m);    // work routine used by the above

  // Together, these keep <object with a preserved mark, mark value> pairs.
  // They should always contain the same number of elements.
  Stack<oop, mtGC>     _objs_with_preserved_marks;
  Stack<markOop, mtGC> _preserved_marks_of_objs;

  // Promotion failure handling
  ExtendedOopClosure *_promo_failure_scan_stack_closure;
  void set_promo_failure_scan_stack_closure(ExtendedOopClosure *scan_stack_closure) {
    _promo_failure_scan_stack_closure = scan_stack_closure;
  }

  Stack<oop, mtGC> _promo_failure_scan_stack;
  void drain_promo_failure_scan_stack(void);
  bool _promo_failure_drain_in_progress;

  // Performance Counters
  GenerationCounters*  _gen_counters;
  CSpaceCounters*      _eden_counters;
  CSpaceCounters*      _from_counters;
  CSpaceCounters*      _to_counters;

  // sizing information
  size_t               _max_eden_size;
  size_t               _max_survivor_size;

  // Allocation support
  bool _should_allocate_from_space;
  bool should_allocate_from_space() const {
    return _should_allocate_from_space;
  }
  void clear_should_allocate_from_space() {
    _should_allocate_from_space = false;
  }
  void set_should_allocate_from_space() {
    _should_allocate_from_space = true;
  }

  // Tenuring
  void adjust_desired_tenuring_threshold();

  // Spaces
  EdenSpace*       _eden_space;
  ContiguousSpace* _from_space;
  ContiguousSpace* _to_space;

  STWGCTimer* _gc_timer;

  enum SomeProtectedConstants {
    // Generations are GenGrain-aligned and have size that are multiples of
    // GenGrain.
    MinFreeScratchWords = 100
  };

  // Return the size of a survivor space if this generation were of size
  // gen_size.
  // 函数计算Survivor的大小
  size_t compute_survivor_size(size_t gen_size, size_t alignment) const {
    // 在JVM参数中有一个比较重要的参数
    // SurvivorRatio，用于定义新生代中Eden空间和Survivor空间的比例，默认为8
    size_t n = gen_size / (SurvivorRatio + 2);
    return n > alignment ? align_size_down(n, alignment) : alignment;
  }

 public:  // was "protected" but caused compile error on win32
  class IsAliveClosure: public BoolObjectClosure {
    Generation* _g;
  public:
    IsAliveClosure(Generation* g);
    bool do_object_b(oop p);
  };

  class KeepAliveClosure: public OopClosure {
  protected:
    ScanWeakRefClosure* _cl;
    CardTableRS* _rs;
    template <class T> void do_oop_work(T* p);
  public:
    KeepAliveClosure(ScanWeakRefClosure* cl);
    virtual void do_oop(oop* p);
    virtual void do_oop(narrowOop* p);
  };

  class FastKeepAliveClosure: public KeepAliveClosure {
  protected:
    HeapWord* _boundary;
    template <class T> void do_oop_work(T* p);
  public:
    FastKeepAliveClosure(DefNewGeneration* g, ScanWeakRefClosure* cl);
    virtual void do_oop(oop* p);
    virtual void do_oop(narrowOop* p);
  };

  class EvacuateFollowersClosure: public VoidClosure {
    GenCollectedHeap* _gch;
    int _level;
    ScanClosure* _scan_cur_or_nonheap;
    ScanClosure* _scan_older;
  public:
    EvacuateFollowersClosure(GenCollectedHeap* gch, int level,
                             ScanClosure* cur, ScanClosure* older);
    void do_void();
  };

  class FastEvacuateFollowersClosure: public VoidClosure {
    GenCollectedHeap* _gch;
    int _level;
    DefNewGeneration* _gen;
    FastScanClosure* _scan_cur_or_nonheap;
    FastScanClosure* _scan_older;
  public:
    FastEvacuateFollowersClosure(GenCollectedHeap* gch, int level,
                                 DefNewGeneration* gen,
                                 FastScanClosure* cur,
                                 FastScanClosure* older);
    void do_void();
  };

 public:
  DefNewGeneration(ReservedSpace rs, size_t initial_byte_size, int level,
                   const char* policy="Copy");

  virtual void ref_processor_init();

  virtual Generation::Name kind() { return Generation::DefNew; }

  // Accessing spaces
  EdenSpace*       eden() const           { return _eden_space; }
  ContiguousSpace* from() const           { return _from_space;  }
  ContiguousSpace* to()   const           { return _to_space;    }

  virtual CompactibleSpace* first_compaction_space() const;

  // Space enquiries
  size_t capacity() const;
  size_t used() const;
  size_t free() const;
  size_t max_capacity() const;
  size_t capacity_before_gc() const;
  size_t unsafe_max_alloc_nogc() const;
  size_t contiguous_available() const;

  size_t max_eden_size() const              { return _max_eden_size; }
  size_t max_survivor_size() const          { return _max_survivor_size; }

  bool supports_inline_contig_alloc() const { return true; }
  HeapWord** top_addr() const;
  HeapWord** end_addr() const;

  // Thread-local allocation buffers
  bool supports_tlab_allocation() const { return true; }
  size_t tlab_capacity() const;
  size_t tlab_used() const;
  size_t unsafe_max_tlab_alloc() const;

  // Grow the generation by the specified number of bytes.
  // The size of bytes is assumed to be properly aligned.
  // Return true if the expansion was successful.
  bool expand(size_t bytes);

  // DefNewGeneration cannot currently expand except at
  // a GC.
  virtual bool is_maximal_no_gc() const { return true; }

  // Iteration
  void object_iterate(ObjectClosure* blk);

  void younger_refs_iterate(OopsInGenClosure* cl);

  void space_iterate(SpaceClosure* blk, bool usedOnly = false);

  // Allocation support
  virtual bool should_allocate(size_t word_size, bool is_tlab) {
    assert(UseTLAB || !is_tlab, "Should not allocate tlab");
    // 在64位系统下，BitsPerSize_t的值为64，LogHeapWordSize的值为3
    size_t overflow_limit    = (size_t)1 << (BitsPerSize_t - LogHeapWordSize);
    // 判断条件1
    // 申请的内存空间未溢出
    const bool non_zero      = word_size > 0;
    // 判断条件2
    // 申请的内存大小不为0
    const bool overflows     = word_size >= overflow_limit;
    // 判断条件3
    // 申请的内存未超过本内存代的限制阈值
    // 检查申请的内存是否太大，check_too_big为true时，对象直接在old区分配内存
    const bool check_too_big = _pretenure_size_threshold_words > 0;
    const bool not_too_big   = word_size < _pretenure_size_threshold_words;
    const bool size_ok       = is_tlab || !check_too_big || not_too_big;
    // 3个判断条件必须都满足
    bool result = !overflows &&
                  non_zero   &&
                  size_ok;

    return result;
  }

  HeapWord* allocate(size_t word_size, bool is_tlab);
  HeapWord* allocate_from_space(size_t word_size);

  HeapWord* par_allocate(size_t word_size, bool is_tlab);

  // Prologue & Epilogue
  virtual void gc_prologue(bool full);
  virtual void gc_epilogue(bool full);

  // Save the tops for eden, from, and to
  virtual void record_spaces_top();

  // Doesn't require additional work during GC prologue and epilogue
  virtual bool performs_in_place_marking() const { return false; }

  // Accessing marks
  void save_marks();
  void reset_saved_marks();
  bool no_allocs_since_save_marks();

  // Need to declare the full complement of closures, whether we'll
  // override them or not, or get message from the compiler:
  //   oop_since_save_marks_iterate_nv hides virtual function...
#define DefNew_SINCE_SAVE_MARKS_DECL(OopClosureType, nv_suffix) \
  void oop_since_save_marks_iterate##nv_suffix(OopClosureType* cl);

  ALL_SINCE_SAVE_MARKS_CLOSURES(DefNew_SINCE_SAVE_MARKS_DECL)

#undef DefNew_SINCE_SAVE_MARKS_DECL

  // For non-youngest collection, the DefNewGeneration can contribute
  // "to-space".
  virtual void contribute_scratch(ScratchBlock*& list, Generation* requestor,
                          size_t max_alloc_words);

  // Reset for contribution of "to-space".
  virtual void reset_scratch();

  // GC support
  virtual void compute_new_size();

  // Returns true if the collection is likely to be safely
  // completed. Even if this method returns true, a collection
  // may not be guaranteed to succeed, and the system should be
  // able to safely unwind and recover from that failure, albeit
  // at some additional cost. Override superclass's implementation.
  virtual bool collection_attempt_is_safe();

  virtual void collect(bool   full,
                       bool   clear_all_soft_refs,
                       size_t size,
                       bool   is_tlab);
  HeapWord* expand_and_allocate(size_t size,
                                bool is_tlab,
                                bool parallel = false);

  oop copy_to_survivor_space(oop old);
  uint tenuring_threshold() { return _tenuring_threshold; }

  // Performance Counter support
  void update_counters();

  // Printing
  virtual const char* name() const;
  virtual const char* short_name() const { return "DefNew"; }

  bool must_be_youngest() const { return true; }
  bool must_be_oldest() const { return false; }

  // PrintHeapAtGC support.
  void print_on(outputStream* st) const;

  void verify();

  bool promo_failure_scan_is_complete() const {
    return _promo_failure_scan_stack.is_empty();
  }

 protected:
  // If clear_space is true, clear the survivor spaces.  Eden is
  // cleared if the minimum size of eden is 0.  If mangle_space
  // is true, also mangle the space in debug mode.
  void compute_space_boundaries(uintx minimum_eden_size,
                                bool clear_space,
                                bool mangle_space);
  // Scavenge support
  void swap_spaces();
};

#endif // SHARE_VM_MEMORY_DEFNEWGENERATION_HPP
