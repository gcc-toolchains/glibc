/* Copyright (C) 1997-2015 Free Software Foundation, Inc.
   This file is part of the GNU C Library.
   Contributed by Ralf Baechle <ralf@gnu.org>, 1998.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library.  If not, see
   <http://www.gnu.org/licenses/>.  */

#include <errno.h>
#ifndef NO_SGIDEFS_H
#include <sgidefs.h>
#endif
#include <unistd.h>
#include <endian.h>

#include <sysdep-cancel.h>
#include <sys/syscall.h>

#ifdef __NR_pwrite64            /* Newer kernels renamed but it's the same.  */
# ifdef __NR_pwrite
#  error "__NR_pwrite and __NR_pwrite64 both defined???"
# endif
# define __NR_pwrite __NR_pwrite64
#endif


ssize_t
__libc_pwrite64 (int fd, const void *buf, size_t count, off64_t offset)
{
  ssize_t result;

  if (SINGLE_THREAD_P)
    {
#if _MIPS_SIM == _ABIN32 || _MIPS_SIM == _ABI64
      result = INLINE_SYSCALL (pwrite, 4, fd, buf, count, offset);
#else
     result = INLINE_SYSCALL (pwrite, 6, fd, buf, count, 0,
			      __LONG_LONG_PAIR ((off_t) (offset >> 32),
			     (off_t) (offset & 0xffffffff)));
#endif

     return result;
    }

  int oldtype = LIBC_CANCEL_ASYNC ();

#if _MIPS_SIM == _ABIN32 || _MIPS_SIM == _ABI64
  result = INLINE_SYSCALL (pwrite, 4, fd, buf, count, offset);
#else
  result = INLINE_SYSCALL (pwrite, 6, fd, buf, count, 0,
			   __LONG_LONG_PAIR ((off_t) (offset >> 32),
					     (off_t) (offset & 0xffffffff)));
#endif

  LIBC_CANCEL_RESET (oldtype);

  return result;
}

weak_alias (__libc_pwrite64, __pwrite64)
libc_hidden_weak (__pwrite64)
weak_alias (__libc_pwrite64, pwrite64)
