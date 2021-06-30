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

#ifndef __flare__layout__flow__
#define __flare__layout__flow__

#include "./flex.h"

namespace flare {

	/**
	 * @class FlowLayout
	 */
	class FX_EXPORT FlowLayout: public FlexLayout {
		FX_Define_View(FlowLayout);
		public:

		// 多根交叉轴线的对齐方式。如果项目只有一根交叉轴，该属性不起作用
		enum WrapAlign: uint8_t {
			START = value::START, // 与交叉轴的起点对齐
			CENTER = value::CENTER, // 与交叉轴的中点对齐
			END = value::END, // 与交叉轴的终点对齐
			SPACE_BETWEEN = value::SPACE_BETWEEN, // 与交叉轴两端对齐,轴线之间的间隔平均分布
			SPACE_AROUND = value::SPACE_AROUND, // 每根轴线两侧的间隔都相等,所以轴线之间的间隔比轴线与边框的间隔大一倍
			SPACE_EVENLY = value::SPACE_EVENLY, // 每根轴线两侧的间隔都相等,这包括边框的间距
			STRETCH = value::STRETCH, // 轴线占满整个交叉轴，平均分配剩余的交叉轴空间
		};

		/**
		 * @constructors
		 */
		FlowLayout();

		// define props
		FX_Define_Prop(WrapAlign, wrap_align); // wrap_align 多根交叉轴的对齐方式,如果项目只有一根交叉轴,该属性不起作用
		FX_Define_Prop(bool, wrap_reverse); // wrap_reverse 多根交叉轴是否反向排列

		// --------------- o v e r w r i t e ---------------
		// @overwrite
		virtual bool layout_forward(uint32_t mark);
		virtual bool layout_reverse(uint32_t mark);

		// --------------- m e m b e r . f i e l d ---------------
		private:
	};

}

#endif