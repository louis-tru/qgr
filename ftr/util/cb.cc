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

#include "ftr/util/cb.h"
#include "ftr/util/str.h"
#include "ftr/util/loop/loop.h"

namespace ftr {

	class DefaultStaticCallback: public CallbackCore<Object, Error> {
	 public:
		virtual bool retain() { return 1; }
		virtual void release() {}
		virtual void call(Cbd& event) const {}
	};

	static DefaultStaticCallback* default_callback_ = nullptr;
	static Mutex mutex;

	static inline DefaultStaticCallback* default_callback() {
		if ( !default_callback ) {
			ScopeLock scope(mutex);
			default_callback_ = NewRetain<DefaultStaticCallback>();
		}
		return default_callback_;
	}

	template<>
	Callback<Object>::Callback(int type): Handle<CallbackCore<Object, Error>>(default_callback()) {
	}

	class WrapCallback: public CallbackCore<Object, Error> {
	 public:
		inline WrapCallback(Cb cb, Error* err, Object* data)
		: _cb(cb), _err(err), _data(data) {
		}
		virtual ~WrapCallback() {
			Release(_err);
			Release(_data);
		}
		virtual void call(Cbd& evt) const {
			evt.error = _err;
			evt.data = _data;
			_cb->call(evt);
		}
	 private:
		Cb _cb;
		Error* _err;
		Object* _data;
	};

	void _async_callback_and_dealloc(Cb cb, Error* e, Object* d, PostMessage* loop) {
		loop->post_message( Cb(new WrapCallback(cb, e, d)) );
	}

}