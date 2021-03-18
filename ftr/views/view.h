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


#ifndef __ftr__views__view__
#define __ftr__views__view__

#include "../util/object.h"
#include "../value.h"

namespace ftr {

	class FX_EXPORT View: public Reference {
		FX_HIDDEN_ALL_COPY(View);
		public:

		virtual void layout_forward();
		virtual void layout_reverse();
		virtual void draw();

		private:
		View *_parent;
		View *_first, *_last;
		View *_prev, *_next;
		View *_next_pre_mark; /* 下一个标记预处理视图 */
		Action *_action;
		uint32_t _level; /* 在视图树中所处的层级 */
		Vec2   _translate, _scale, _skew; // 平移向量, 缩放向量, 倾斜向量
		float  _rotateZ;    /* z轴旋转角度值 */
		float  _opacity;    /* 可影响子视图的透明度值 */
		Mat   _final_matrix;  /* 父视图矩阵乘以基础矩阵等于最终变换矩阵 */
		float _final_opacity; /* 最终的不透明值 */
	};

}

#endif