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

#include "linux-ime-helper-1.h"
#include "ftr/util/cb.h"
#include "ftr/keyboard.h"
#include "ftr/display-port.h"
#include <X11/keysym.h>
#include <locale.h>

FX_NS(ftr)

extern void __dispatch_x11_async(cCb& cb); // sync to x11 main message loop

/**
 * @class LINUXIMEHelper::Inl
 */
class LINUXIMEHelper::Inl {
 public:

	static Inl* create(AppInl* app, Display* dpy, Window win, int inputStyle) {

		char *locale = setlocale(LC_CTYPE, "");
		if (locale == NULL) {
			FX_ERR("Can't set locale");
			return nullptr;
		}
		DLOG("locale: %s", locale);

		if (!XSupportsLocale()) {
			FX_ERR("X does not support locale");
			return nullptr;
		}

		char *modifiers = XSetLocaleModifiers("");
		if (modifiers == NULL) {
			FX_ERR("Can't set locale modifiers");
			return nullptr;
		}
		DLOG("modifiers: %s", modifiers);

		return new Inl(app, dpy, win, inputStyle);
	}

	Inl(AppInl* app, Display* dpy, Window win, int inputStyle)
		: _app(app)
		, _display(dpy)
		, _window(win)
		, _im(NULL), _ic(NULL)
		, _has_open(false)
		, _input_style(inputStyle)
		, _fontset(NULL)
	{
		_spot_location = {1,1};

		// load fontset
		char **missing_list = NULL;
		int missing_count = 0;
		char *default_string = NULL;
		_fontset = XCreateFontSet(_display, "*,*", 
			&missing_list, &missing_count, &default_string);
		if (_fontset == NULL) {
			FX_ERR("Can't create font set: %s", "*,*");
		}
	}

	~Inl() {
		if (_fontset) {
			XFreeFontSet(_display, _fontset);
		}
		close();
	}

	void open() {
		DLOG("IME open");
		if (!_has_open) {
			_has_open = true;
			registerInstantiateCallback();
		}
	}

	void close() {
		DLOG("IME close");
		_has_open = false;
		destroyIC();
		closeIM();
	}

	void clear() {
		DLOG("IME clear");
		if (_has_open && _ic) {
			if (!_preedit_string.is_empty()) {
				_preedit_string = String();
				_app->dispatch()->dispatch_ime_unmark(String());
			}
			XUnsetICFocus(_ic);
			Xutf8ResetIC(_ic);
			XSetICFocus(_ic);
		}
	}

	void set_keyboard_can_backspace(bool can_backspace, bool can_delete) {
		// TODO
	}

	void set_keyboard_type(KeyboardType type) {
		// TODO
	}

	void set_keyboard_return_type(KeyboardReturnType type) {
		// TODO
	}

	void set_spot_location(Vec2 location) {
		DLOG("set_spot_location, x=%f,y=%f", location[0], location[1]);
		if (location[0] != 0 || location[1] != 0) {
			Vec2 scale = _app->display_port()->scale_value();
			_spot_location = {
				int16(location.x() * scale.x()), int16(location.y() * scale.y())
			};
			updateSpotLocation();
		}
	}

	void key_press(XKeyPressedEvent *event)
	{
		if (_im == NULL)
			return;

		char buf[256] = { '\0', };
		KeySym keysym = 0;
		Status status = XLookupNone;

		if (_ic == NULL) {
			XLookupString(event, buf, sizeof(buf), &keysym, NULL);
			status = XLookupChars;
		} else {
			Xutf8LookupString(_ic, event, buf, 256, &keysym, &status);
		}

		DLOG("onKeyPress %lu\n", keysym);

		if (status == XLookupChars || 
			status == XLookupKeySym || status == XLookupBoth) {

			if (keysym == XK_Return) {
				onKeyReturn();
			} else if (keysym == XK_Delete) {
				onKeyDelete();
			} else if (keysym == XK_BackSpace) {
				onKeyBackspace();
			} else if (keysym == XK_Escape) {
				// esc
			} else if (keysym == XK_Left) {
				onKeyControl(KEYCODE_LEFT);
			} else if (keysym == XK_Right) {
				onKeyControl(KEYCODE_RIGHT);
			} else if (keysym == XK_Up) {
				onKeyControl(KEYCODE_UP);
			} else if (keysym == XK_Down) {
				onKeyControl(KEYCODE_DOWN);
			} else if (keysym == XK_Home) {
				onKeyControl(KEYCODE_MOVE_HOME);
			} else if (keysym == XK_End) {
				onKeyControl(KEYCODE_MOVE_END);
			} else {
				if ((status == XLookupChars || status == XLookupBoth) &&
					((event->state & ControlMask) != ControlMask) &&
					((event->state & Mod1Mask) != Mod1Mask)) 
				{
					insert(buf);
				}
			}
		}
	}

	void focus_in()
	{
		if (_ic == NULL)
			return;
		XSetICFocus(_ic);
	}

	void focus_out()
	{
		if (_ic == NULL)
			return;

		char* str = Xutf8ResetIC(_ic);

		if (str != NULL) {
			setPreeditString(str, 0, 0);
		}
		setPreeditString(NULL, 0, 0);

		XUnsetICFocus(_ic);
	}

 private:

	// for XIM
	void registerInstantiateCallback() 
	{
		XRegisterIMInstantiateCallback(_display, NULL, NULL, NULL,
			IMInstantiateCallback, (XPointer)this);
	}

	// IM instantiate callback
	static void IMInstantiateCallback(Display *display,
						 XPointer client_data, XPointer data)
	{
		if (client_data == NULL)
			return;
		DLOG("XIM is available now");
		auto self = reinterpret_cast<Inl*>(client_data);
		self->openIM();
	}

	// IM destroy callbacks
	static void IMDestroyCallback(XIM im, XPointer client_data, XPointer data)
	{
		DLOG("xim is destroyed");

		if (client_data == NULL)
			return;

		auto self = reinterpret_cast<Inl*>(client_data);

		self->_im = NULL;
		self->_ic = NULL;

		if (self->_has_open)
			self->registerInstantiateCallback();
	}

	// on the spot callbacks
	static void preeditStartCallback(XIM xim, XPointer user_data, XPointer data)
	{
		DLOG("preedit start");
	}

	static void preeditDoneCallback(XIM xim, XPointer user_data, XPointer data)
	{
		DLOG("preedit done");

		if (user_data == NULL)
			return;

		auto self = reinterpret_cast<Inl*>(user_data);
		self->setPreeditString(NULL, 0, 0);
	}

	static void preeditDrawCallback(XIM xim, XPointer user_data, XPointer data)
	{
		if (user_data == NULL || data == NULL)
			return;

		auto self = reinterpret_cast<Inl*>(user_data);

		XIMPreeditDrawCallbackStruct *draw_data = 
			reinterpret_cast<XIMPreeditDrawCallbackStruct*>(data);

		self->setPreeditCaret(draw_data->caret);

		if (draw_data->text == NULL) {
			self->setPreeditString(NULL,
				draw_data->chg_first, draw_data->chg_length);
		} else {
			if (draw_data->text->encoding_is_wchar) {
				String str = wchar_t_to_string(draw_data->text->string.wide_char);
				self->setPreeditString(*str,
						draw_data->chg_first, draw_data->chg_length);
			} else {
				self->setPreeditString(draw_data->text->string.multi_byte,
					draw_data->chg_first, draw_data->chg_length);
			}
		}
	}

	static void preeditCaretCallback(XIM xim, XPointer user_data, XPointer data)
	{
		if (user_data == NULL || data == NULL)
		return;

		auto self = reinterpret_cast<Inl*>(user_data);
		XIMPreeditCaretCallbackStruct *caret_data = 
			reinterpret_cast<XIMPreeditCaretCallbackStruct*>(data);

		switch (caret_data->direction) {
			case XIMForwardChar:
				self->onKeyControl(KEYCODE_RIGHT);
				break;
			case XIMBackwardChar:
				self->onKeyControl(KEYCODE_LEFT);
				break;
			case XIMDontChange:
				break;
			default:
				DLOG("preedit caret: %d", caret_data->direction);
				break;
		}
	}

	static void statusStartCallback(XIM xim, XPointer user_data, XPointer data)
	{
		DLOG("status start");
	}

	static void statusDoneCallback(XIM xim, XPointer user_data, XPointer data)
	{
		DLOG("status done");
	}

	static void statusDrawCallback(XIM xim, XPointer user_data, XPointer data)
	{
		DLOG("status draw");
	}

	// string conversion callback
	static void stringConversionCallback(XIM xim, XPointer client_data, XPointer data)
	{
		DLOG("string conversion");
	}

	// for XIM interaction
	void openIM()
	{
		ASSERT(!_im);

		_im = XOpenIM(_display, NULL, NULL, NULL);
		if (_im  == NULL) {
			DLOG("Can't open XIM");
			return;
		}

		DLOG("XIM is opened");
		XUnregisterIMInstantiateCallback(_display, NULL, NULL, NULL,
						 IMInstantiateCallback, (XPointer)this);

		// register destroy callback
		XIMCallback destroy;
		destroy.callback = IMDestroyCallback;
		destroy.client_data = (XPointer)this;
		XSetIMValues(_im, XNDestroyCallback, &destroy, NULL);

		bool useStringConversion = false;

		XIMValuesList* ic_values = NULL;
		XGetIMValues(_im,
			 XNQueryICValuesList, &ic_values,
			 NULL);
		
		if (ic_values != NULL) {
			for (int i = 0; i < ic_values->count_values; i++) {
				DLOG("%s", ic_values->supported_values[i]);
				if (strcmp(ic_values->supported_values[i],
							XNStringConversionCallback) == 0) {
					useStringConversion = true;
					break;
				}
			}
		}

		createIC(useStringConversion);
	}

	void closeIM(void)
	{
		if (_im == NULL)
			return;

		XCloseIM(_im);
		_im = NULL;

		DLOG("XIM is closed");
	}

	void createIC(bool useStringConversion)
	{
		if (_im == NULL)
			return;
		
		ASSERT(!_ic);

		if ((_input_style & XIMPreeditPosition) && _fontset) {
			XRectangle area = { 0,0,1,1 };
			XVaNestedList attr = XVaCreateNestedList(0,
							XNSpotLocation, &_spot_location,
							XNArea, &area,
							XNFontSet, _fontset,
							NULL);

			_ic = XCreateIC(_im,
							XNInputStyle, XIMPreeditPosition,
							XNClientWindow, _window,
							XNPreeditAttributes, attr,
							NULL);

			XFree(attr);
		} else {
			XIMCallback preedit_start, preedit_done, preedit_draw, preedit_caret;

			preedit_start.callback = preeditStartCallback;
			preedit_start.client_data = (XPointer)this;
			preedit_done.callback = preeditDoneCallback;
			preedit_done.client_data = (XPointer)this;
			preedit_draw.callback = preeditDrawCallback;
			preedit_draw.client_data = (XPointer)this;
			preedit_caret.callback = preeditCaretCallback;
			preedit_caret.client_data = (XPointer)this;

			XIMCallback status_start, status_done, status_draw;

			status_start.callback = statusStartCallback;
			status_start.client_data = (XPointer)this;
			status_done.callback = statusDoneCallback;
			status_done.client_data = (XPointer)this;
			status_draw.callback = statusDrawCallback;
			status_draw.client_data = (XPointer)this;

			XVaNestedList preedit_attr = XVaCreateNestedList(0,
							XNPreeditStartCallback, &preedit_start,
							XNPreeditDoneCallback,  &preedit_done,
							XNPreeditDrawCallback,  &preedit_draw,
							XNPreeditCaretCallback, &preedit_caret,
							NULL);

			XVaNestedList status_attr = XVaCreateNestedList(0,
							XNStatusStartCallback, &status_start,
							XNStatusDoneCallback,  &status_done,
							XNStatusDrawCallback,  &status_draw,
							NULL);

			_ic = XCreateIC(_im,
							XNInputStyle, XIMPreeditCallbacks,
							XNClientWindow, _window,
							XNPreeditAttributes, preedit_attr,
							XNStatusAttributes, status_attr,
							NULL);
			XFree(preedit_attr);
			XFree(status_attr);
		}

		if (_ic != NULL) {
			if (useStringConversion) {
				XIMCallback strconv;
				strconv.callback    = stringConversionCallback;
				strconv.client_data = (XPointer)this;
				XSetICValues(_ic, XNStringConversionCallback, &strconv, NULL);
			}
			DLOG("XIC is created");
		} else {
			DLOG("cannot create XIC");
		}
	}

	void destroyIC()
	{
		if (_ic == NULL)
			return;

		char* str = Xutf8ResetIC(_ic);

		if (str != NULL) {
			setPreeditString(str, 0, 0);
		}
		setPreeditString(NULL, 0, 0);

		XDestroyIC(_ic);
		_ic = NULL;
		DLOG("XIC is destroyed");
	}

	static String wchar_t_to_string(const wchar_t *str)
	{
		if (sizeof(wchar_t) == 2) {
			Ucs2String ustr = (const uint16*)str;
			return ustr.to_string();
		} else {
			Ucs4String ustr = (const uint*)str;
			return ustr.to_string();
		}
	}

	void updateSpotLocation() {
		if (_ic) {
			XVaNestedList attr = XVaCreateNestedList(0,
				XNSpotLocation, &_spot_location, NULL);
			XSetICValues(_ic, XNPreeditAttributes, attr, NULL);
			XFree(attr);
		}
	}

	void insert(cchar* str)
	{
		DLOG("insert, %s", str);
		_app->dispatch()->dispatch_ime_insert(str);
	}

	void setPreeditString(cchar* str, int pos, int length)
	{
		DLOG("setPreeditString, %s, %d, %d", str, pos, length);
		if (str == NULL) {
			_app->dispatch()->dispatch_ime_unmark(String());
			_preedit_string = String();
		} else {
			_preedit_string = str;
			_app->dispatch()->dispatch_ime_marked(_preedit_string);
		}
	}

	void onKeyReturn()
	{
		_app->dispatch()->dispatch_ime_insert("\n");
	}

	void onKeyDelete()
	{
		_app->dispatch()->dispatch_ime_delete(1);
	}

	void onKeyBackspace()
	{
		_app->dispatch()->dispatch_ime_delete(-1);
	}

	void setPreeditCaret(int pos)
	{
		// TODO ...
	}

	void onKeyControl(KeyboardKeyName name) {
		_app->dispatch()->dispatch_ime_control(name);
	}

	AppInl* _app;
	Display *_display;
	Window _window;
	XIM _im;
	XIC _ic;
	String _preedit_string;
	bool _has_open;
	int _input_style;
	XPoint _spot_location;
	XFontSet _fontset;
};

LINUXIMEHelper::LINUXIMEHelper(AppInl* app, Display* dpy, Window win, int inputStyle)
: _inl(Inl::create(app, dpy, win, inputStyle)) {
}
LINUXIMEHelper::~LINUXIMEHelper() {
	delete _inl;
	_inl = nullptr;
}
void LINUXIMEHelper::open(KeyboardOptions options) {
	if (!_inl) return;
	__dispatch_x11_async(Cb([=](CbD& e){
		_inl->set_keyboard_type(options.type);
		_inl->set_keyboard_return_type(options.return_type);
		_inl->set_spot_location(options.spot_location);
		if (options.is_clear) {
			_inl->clear();
		}
		_inl->open();
	}));
}
void LINUXIMEHelper::close() {
	if (!_inl) return;
	__dispatch_x11_async(Cb([=](CbD& e){
		_inl->close();
	}));
}
void LINUXIMEHelper::clear() {
	if (!_inl) return;
	__dispatch_x11_async(Cb([=](CbD& e){
		_inl->clear();
	}));
}
void LINUXIMEHelper::set_keyboard_can_backspace(bool can_backspace, bool can_delete) {
	if (!_inl) return;
	__dispatch_x11_async(Cb([=](CbD& e){
		_inl->set_keyboard_can_backspace(can_backspace, can_delete);
	}));
}
void LINUXIMEHelper::set_keyboard_type(KeyboardType type) {
	if (!_inl) return;
	__dispatch_x11_async(Cb([=](CbD& e){
		_inl->set_keyboard_type(type);
	}));
}
void LINUXIMEHelper::set_keyboard_return_type(KeyboardReturnType type) {
	if (!_inl) return;
	__dispatch_x11_async(Cb([=](CbD& e){
		_inl->set_keyboard_return_type(type);
	}));
}
void LINUXIMEHelper::set_spot_location(Vec2 location) {
	if (!_inl) return;
	__dispatch_x11_async(Cb([=](CbD& e){
		_inl->set_spot_location(location);
	}));
}
void LINUXIMEHelper::key_press(XKeyPressedEvent *event) {
	if (!_inl) return;
	_inl->key_press(event);
}
void LINUXIMEHelper::focus_in() {
	if (!_inl) return;
	_inl->focus_in();
}
void LINUXIMEHelper::focus_out() {
	if (!_inl) return;
	_inl->focus_out();
}

FX_END
