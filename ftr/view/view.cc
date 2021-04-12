/* ***** BEGIN LICENSE BLOCK *****
 * Distributed under the BSD license:
 *
 * Copyright © 2015-2016, xuewen.chu
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

#include "./view.h"

namespace ftr {

	View::View() {
		// TODO ...
	}

	View::~View() {
		// TODO ...
	}

	void View::set_action(Action* val) {
		// TODO ...
	}

	void View::set_translate(Vec2 val) {
		// TODO ...
	}

	void View::set_scale(Vec2 val) {
		// TODO ...
	}

	void View::set_skew(Vec2 val) {
		// TODO ...
	}

	void View::set_rotate(float val) {
		// TODO ...
	}

	void View::set_opacity(float val) {
		// TODO ...
	}

	void View::draw() {
		// TODO ...
	}

	void View::layout_forward() {
		// TODO ...
	}

	void View::layout_reverse() {
		// TODO ...
	}

	void View::layout_size_lock(bool lock, Vec2 size) {
		// TODO ...
	}

	Vec2 View::layout_content_size() {
		return _layout_size;
	}

	// 内部布局偏移补偿
	Vec2 View::layout_offset_inside() {
		return _layout_origin;
	}

	// 计算布局变换矩阵
	Mat View::layout_matrix() const {
		Vec2 offset = _layout_offset; // xy 布局偏移
		Vec2 in = _parent ? _parent->layout_offset_inside(): Vec2();
		offset.x( offset.x() + _layout_origin.x() + _translate.x() - in.x() );
		offset.y( offset.y() + _layout_origin.y() + _translate.y() - in.y() );
		return Mat(offset, _scale, -_rotate, _skew);
	}

	const Mat& View::transform_matrix() {
		if (1) { // update layout_matrix
			if (_parent) {
				_parent->transform_matrix().multiplication(layout_matrix(), _transform_matrix);
			} else {
				_transform_matrix = layout_matrix();
			}
		}
		return _transform_matrix;
	}

}