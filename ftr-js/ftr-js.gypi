{
	'targets': [
	{
		'target_name': 'ftr-v8',
		'type': 'none',
		'dependencies': [
			'depe/v8-link/v8-link.gyp:v8-link',
			'depe/v8-link/v8-link.gyp:v8_libplatform-link',
		],
	},
	{
		'target_name': 'ftr-node',
		'type': '<(output_type)',
		'dependencies': [
			'ftr',
			'ftr-js',
			'depe/node/node.gyp:node',
		],
		"sources": [
			'../tools/useless.c', # fix mac platform build error
		],
	},
	{
		'target_name': 'ftr-js',
		'type': '<(output_type)',
		'include_dirs': [
			'..',
			'../out',
			'../depe/node/deps/uv/include',
			'../depe/node/deps/openssl/openssl/include',
		],
		'dependencies': [
			'ftr',
			'ftr-v8',
		],
		'direct_dependent_settings': {
			'include_dirs': [ '..' ],
			'mac_bundle_resources': [],
		},
		'defines': [ 'NODE_WANT_INTERNALS=1' ],
		'sources': [
			'js-1.h',
			'js-cls.cc',
			'js.cc',
			'js.h',
			'ftr.h',
			'ftr.cc',
			'str.h',
			'str.cc',
			'v8.cc',
			'value.h',
			'wrap.h',
			'wrap.cc',
			'node.cc',
			# binding ftr/util
			'binding/cb-1.h',
			'binding/cb.cc',
			'binding/fs-1.h',
			'binding/fs.cc',
			'binding/fs-reader.cc',
			'binding/fs-path.cc',
			'binding/http.cc',
			'binding/util.cc',
			'binding/storage.cc',
			'binding/json-1.h',
			'binding/json.cc',
			'binding/event-1.h',
			'binding/event.cc',
			'binding/sys.cc',
			'binding/event.cc',
			'binding/timer.cc',
			'binding/console.cc',
			'binding/buffer.cc',
			# binding ftr
			'binding/value.cc',
			'binding/binding.cc',
			'binding/action.cc',
			'binding/action-frame.cc',
			'binding/app.cc',
			'binding/audio-player.cc',
			'binding/video.cc',
			'binding/media.cc',
			'binding/div.cc',
			'binding/display-port.cc',
			'binding/indep.cc',
			'binding/image.cc',
			'binding/layout.cc',
			'binding/box.cc',
			'binding/view.cc',
			'binding/root.cc',
			'binding/span.cc',
			'binding/sprite.cc',
			'binding/hybrid.cc',
			'binding/text-font.cc',
			'binding/text-node.cc',
			'binding/label.cc',
			'binding/limit.cc',
			'binding/panel.cc',
			'binding/button.cc',
			'binding/scroll.cc',
			'binding/css.cc',
			'binding/font.cc',
			'binding/text.cc',
			'binding/input.cc',
			'binding/background.cc',
		],
		'conditions': [
			['v8_enable_inspector==1', { 'defines': [ 'HAVE_INSPECTOR=1' ] }],
			['node_use_openssl=="true"', { 'defines': [ 'HAVE_OPENSSL=1' ] }],
			['node_use_dtrace=="true"', { 'defines': [ 'HAVE_DTRACE=1' ] }],
		],
		# actions
		'variables': {
			'libs_ftr_files_out': [
				'libs/ftr/out/ftr/_action.js',
				'libs/ftr/out/ftr/_common.js',
				'libs/ftr/out/ftr/_event.js',
				'libs/ftr/out/ftr/_ext.js',
				'libs/ftr/out/ftr/_path.js',
				'libs/ftr/out/ftr/_pkgutil.js',
				'libs/ftr/out/ftr/_util.js',
				'libs/ftr/out/ftr/_view.js',
				'libs/ftr/out/ftr/action.js',
				'libs/ftr/out/ftr/app.js',
				'libs/ftr/out/ftr/buffer.js',
				'libs/ftr/out/ftr/checkbox.js',
				'libs/ftr/out/ftr/css.js',
				'libs/ftr/out/ftr/ctr.js',
				'libs/ftr/out/ftr/dialog.js',
				'libs/ftr/out/ftr/display_port.js',
				'libs/ftr/out/ftr/event.js',
				'libs/ftr/out/ftr/font.js',
				'libs/ftr/out/ftr/fs.js',
				'libs/ftr/out/ftr/http.js',
				'libs/ftr/out/ftr/media.js',
				'libs/ftr/out/ftr/nav.js',
				'libs/ftr/out/ftr/overlay.js',
				'libs/ftr/out/ftr/package.json',
				'libs/ftr/out/ftr/path.js',
				'libs/ftr/out/ftr/pkg.js',
				'libs/ftr/out/ftr/index.js',
				'libs/ftr/out/ftr/reader.js',
				'libs/ftr/out/ftr/stepper.js',
				'libs/ftr/out/ftr/storage.js',
				'libs/ftr/out/ftr/sys.js',
				'libs/ftr/out/ftr/timer.js',
				'libs/ftr/out/ftr/util.js',
				'libs/ftr/out/ftr/value.js',
				'libs/ftr/out/ftr/errno.js',
			],
		},
		'actions': [
			{
				'action_name': 'build_ftr_',
				'inputs': [
					'../libs/ftr/tsconfig.json',
					'../libs/ftr/_action.ts',
					'../libs/ftr/_common.ts',
					'../libs/ftr/_event.ts',
					'../libs/ftr/_ext.ts',
					'../libs/ftr/_path.ts',
					'../libs/ftr/_pkgutil.ts',
					'../libs/ftr/_util.ts',
					'../libs/ftr/_view.ts',
					'../libs/ftr/action.ts',
					'../libs/ftr/app.ts',
					'../libs/ftr/buffer.ts',
					'../libs/ftr/checkbox.tsx',
					'../libs/ftr/css.ts',
					'../libs/ftr/ctr.ts',
					'../libs/ftr/dialog.tsx',
					'../libs/ftr/display_port.ts',
					'../libs/ftr/event.ts',
					'../libs/ftr/font.ts',
					'../libs/ftr/fs.ts',
					'../libs/ftr/http.ts',
					'../libs/ftr/media.ts',
					'../libs/ftr/nav.tsx',
					'../libs/ftr/overlay.tsx',
					'../libs/ftr/package.json',
					'../libs/ftr/path.ts',
					'../libs/ftr/pkg.ts',
					'../libs/ftr/index.ts',
					'../libs/ftr/reader.ts',
					'../libs/ftr/stepper.tsx',
					'../libs/ftr/storage.ts',
					'../libs/ftr/sys.ts',
					'../libs/ftr/timer.ts',
					'../libs/ftr/util.ts',
					'../libs/ftr/value.ts',
					'../libs/ftr/errno.ts',
				],
				'outputs': [
					'../out/_event.js',
					'../out/_value.js',
					'../out/_pkg.js',
					'../out/_pkgutil.js',
					'<@(libs_ftr_files_out)',
				],
				'action': [ 'sh', '-c', 'cd libs/ftr; npm run build' ],
			},
			{
				'action_name': 'gen_inl_js_natives',
				'inputs': [
					'../tools/gen-js-natives.js',
					'../out/_event.js',
					'../out/_value.js',
				],
				'outputs': [
					'../out/native-inl-js.h',
					'../out/native-inl-js.cc',
				],
				'action': [
					'<(node)',
					'<@(_inputs)',
					'',
					'INL',
					'wrap',
					'<@(_outputs)',
				],
				'process_outputs_as_sources': 1,
			},
			{
				'action_name': 'gen_lib_js_natives',
				'inputs': [
					'../tools/gen-js-natives.js',
					'../out/_pkg.js',
					'../out/_pkgutil.js',
				],
				'outputs': [
					'../out/native-lib-js.h',
					'../out/native-lib-js.cc',
				],
				'action': [
					'<(node)',
					'<@(_inputs)',
					'',
					'LIB',
					'wrap',
					'<@(_outputs)',
				],
				'process_outputs_as_sources': 1,
			},
			{
				'action_name': 'gen_ext_js_natives_',
				'inputs': [
					'../tools/gen-js-natives.js',
					'<@(libs_ftr_files_out)',
				],
				'outputs': [
					'../out/native-ext-js.h',
					'../out/native-ext-js.cc',
				],
				'action': [
					'<(node)',
					'<@(_inputs)',
					'ftr',
					'EXT',
					'',
					'<@(_outputs)',
				],
				'process_outputs_as_sources': 1,
			},
		],
	},
	],

	'conditions': [
		['os!="ios"', {
			'targets+': [
			{
				'target_name': 'ftr_exec',
				'product_name': 'ftr',
				'type': 'executable',
				'dependencies': [
					'ftr',
					'ftr-js',
					'ftr-media',
					'ftr-node',
				],
				'sources': [
					'main.cc',
				],
				'ldflags': [ '<@(other_ldflags)' ],
			}],
		}]
	],
}
