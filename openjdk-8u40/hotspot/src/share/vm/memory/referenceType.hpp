/*
 * Copyright (c) 2012, 2013, Oracle and/or its affiliates. All rights reserved.
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

#ifndef SHARE_VM_MEMORY_REFRERENCETYPE_HPP
#define SHARE_VM_MEMORY_REFRERENCETYPE_HPP

#include "utilities/debug.hpp"

// ReferenceType is used to distinguish between java/lang/ref/Reference subclasses

enum ReferenceType {
  // 普通类，也就是非引用类型
  // 表示java/lang/ref/Reference子类，但是这个子类不是REF_SOFT、REF_WEAK、REF_FINAL、REF_PHANTOM中的任何一种
  REF_NONE,      // Regular class
  REF_OTHER,     // Subclass of java/lang/ref/Reference, but not subclass of one of the classes below
  // 表示java/lang/ref/SoftReference类及其子类
  REF_SOFT,      // Subclass of java/lang/ref/SoftReference
  // 表示 java/lang/ref/WeakReference类及其子类
  REF_WEAK,      // Subclass of java/lang/ref/WeakReference
  // 表示 java/lang/ref/FinalReference类及其子类
  REF_FINAL,     // Subclass of java/lang/ref/FinalReference
  // 表示 java/lang/ref/PhantomReference类及其子类
  REF_PHANTOM,   // Subclass of java/lang/ref/PhantomReference
  REF_CLEANER    // Subclass of sun/misc/Cleaner
};

#endif // SHARE_VM_MEMORY_REFRERENCETYPE_HPP
