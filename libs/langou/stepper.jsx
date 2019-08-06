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

import { 
	CSS, Hybrid, Button, ViewController, atomPixel 
} from 'langou/langou';

CSS({
	
	'.x_stepper': {
		width: 45 * 2 + 3,
		textFamily: 'icon',
		textLineHeight: 29,
		textSize: 20,
		textColor: '#727272',
	},
	
	'.x_stepper .minus, .x_stepper .plus': {
		width: 45,
		height: 28,
		border: `${atomPixel} #727272`,
		borderColor: '#727272',
		backgroundColor: '#fff',
	},
	
	'.x_stepper .minus': {
		borderRadiusLeftTop: 6,
		borderRadiusLeftBottom: 6,
	},
	
	'.x_stepper .plus': {
		borderLeftWidth: 0,
		borderRadiusRightTop: 6,
		borderRadiusRightBottom: 6,
	},
	
	'.x_stepper .minus:down, .x_stepper .plus:down': {
		backgroundColor: '#eee',
	},
})

/**
 * @class Stepper
 */
export class Stepper extends ViewController {
	m_value = 0;
	
	min = 0;
	max = Infinity;
	step = 1;
	
	event onChange;
	
	m_minus_click_handle() {
		this.value = this.m_value - this.step;
	}
	
	m_plus_click_handle() {
		this.value = this.m_value + this.step;
	}
	
	render(vx) {
		return (
			<Hybrid class="x_stepper">
				<Button class="minus" 
					onClick="m_minus_click_handle" defaultHighlighted=0>\ued5e</Button>
				<Button class="plus" 
					onClick="m_plus_click_handle" defaultHighlighted=0>\ued5d</Button>
			</Hybrid>  
		);
	}
	
	get value() {
		return this.m_value;
	}
	
	set value(value) {
		
		if ( typeof value == 'number') {
			
			value = Math.min(this.max, Math.max(this.min, value));
			
			if ( value != this.m_value ) {
				this.m_value = value;
				this.triggerChange();
			}
		}
	}
}
