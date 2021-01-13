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

#ifndef __ftr__pre_render__
#define __ftr__pre_render__

#include "ftr/util/util.h"
#include "ftr/util/array.h"
#include "ftr/util/list.h"

/**
 * @ns ftr
 */

FX_NS(ftr)

class View;

/**
 * @class PreRender 预渲染
 */
class FX_EXPORT PreRender: public Object {
	FX_HIDDEN_ALL_COPY(PreRender);
 public:
	
	PreRender();
	
	virtual ~PreRender();
	
	class FX_EXPORT Task {
	 public:
		typedef List<Task*>::Iterator ID;
		inline Task(): _timeout(0) { }
		virtual ~Task();
		virtual bool run_task(int64 sys_time) = 0;
		void register_task();
		void unregister_task();
		inline bool is_register_task() const { return !_task_id.is_null(); }
		inline ID get_task_id() const { return _task_id; }
		inline void set_task_id(ID id) { _task_id = id; }
		inline int64 get_task_timeout() const { return _timeout; }
		inline void set_task_timeout(int64 timeout_us) { _timeout = timeout_us; }
	 private:
		ID    _task_id;
		int64 _timeout;
	};
	
	/**
	 * @func solve 解决预先渲染问题,如果需要更新视图返回true
	 */
	bool solve(int64 now_time);
	
	/**
	 * @func mark_pre
	 */
	void mark_pre(View* view);
	
 private:
	
	bool         _mark_pre;    // 是否有layout标记
	Array<View*> _marks;       // 被标记的视图
	List<Task*>  _tasks;
	static PreRender* _pre_render;
	
	friend PreRender* pre_render();
	
	FX_DEFINE_INLINE_CLASS(Inl)
};

/**
 * @func pre_render
 */
inline PreRender* pre_render() {
	return PreRender::_pre_render;
}

FX_END
#endif

