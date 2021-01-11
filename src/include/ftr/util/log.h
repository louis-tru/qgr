/* ***** BEGIN LICENSE BLOCK *****
 * Distributed under the BSD license:
 *
 * Copyright (c) 2015, xuewen.chu
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of xuewen.chu nor the
 *       names of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL xuewen.chu BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * ***** END LICENSE BLOCK ***** */

#ifndef __ftr__util__log__
#define __ftr__util__log__

#include <ftr/util/macros.h>
#include <ftr/util/object.h>

namespace ftr {

	class Error;

	namespace console {
		FX_EXPORT void log(char);
		FX_EXPORT void log(unsigned char);
		FX_EXPORT void log(int16_t);
		FX_EXPORT void log(uint32_t);
		FX_EXPORT void log(int);
		FX_EXPORT void log(uint32_t);
		FX_EXPORT void log(float);
		FX_EXPORT void log(double);
		FX_EXPORT void log(int64_t);
		FX_EXPORT void log(uint64_t);
		FX_EXPORT void log(bool);
		FX_EXPORT void log(const char*, ...);
		FX_EXPORT void log(const String&);
		FX_EXPORT void log_ucs2(const String16&);
		FX_EXPORT void warn(const char*, ...);
		FX_EXPORT void warn(const String&);
		FX_EXPORT void error(const char*, ...);
		FX_EXPORT void error(const String&);
		FX_EXPORT void error(const Error&);
		FX_EXPORT void tag(const char*, const char*, ...);
		FX_EXPORT void print(const char*, ...);
		FX_EXPORT void print(const String&);
		FX_EXPORT void print_err(const char*, ...);
		FX_EXPORT void print_err(const String&);
		FX_EXPORT void clear();
		#if FX_ARCH_32BIT
		FX_EXPORT void log(long);
		FX_EXPORT void log(unsigned long);
		#endif
	}

	/**
	* @class Console # util log
	*/
	class FX_EXPORT Console {
		public:
		// typedef NonObjectTraits Traits;
		virtual ~Console() = default;
		virtual void log(const String& str);
		virtual void warn(const String& str);
		virtual void error(const String& str);
		virtual void print(const String& str);
		virtual void print_err(const String& str);
		virtual void clear();
		void set_as_default();
	};

}
#endif
