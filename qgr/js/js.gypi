{
	'variables': {
		'use_v8%': 0,
	},
	'target_defaults': {
		'conditions': [
			['use_v8==0 and os=="ios"', {
				'defines': [ 'USE_JSC=1' ],
			},{
				'defines': [ 'USE_JSC=0' ],
			}]
		]
	},
	'targets': [
	{
		'target_name': 'qgr-v8',
		'type': 'none',
		'conditions': [
			['library_output_type=="shared_library"',{
				'type': '<(library_output_type)',
			}]
		],
		'dependencies': [
			'depe/v8-link/v8-link.gyp:v8-link',
			'depe/v8-link/v8-link.gyp:v8_libplatform-link',
		],
	},
	{
		'target_name': 'qgr-node',
		'type': 'none',
		'conditions': [
			['library_output_type=="shared_library"',{
				'type': '<(library_output_type)',
			}],
		],
		'dependencies': [
			'qgr-js',
			'qgr-v8',
			'depe/node/node.gyp:node',
		],
	},
	{
		'target_name': 'qgr-js',
		'type': '<(library_output_type)',
		'include_dirs': [
			'../..',
			'../../out',
			'../../depe/node/deps/uv/include',
			'../../depe/node/deps/openssl/openssl/include',
		],
		'dependencies': [
			'qgr',
			'qgr-media',
			'qgr-v8',
		],
		'direct_dependent_settings': {
			'include_dirs': [ '../..' ],
			'mac_bundle_resources': [],
		},
		'defines': [ 'NODE_WANT_INTERNALS=1' ],
		'sources': [
			'js-1.h',
			'js-cls.cc',
			'js.cc',
			'js.h',
			'qgr.h',
			'qgr.cc',
			'str.h',
			'str.cc',
			'v8.cc',
			'value.h',
			'wrap.h',
			'wrap.cc',
			'node.cc',
			# binding qgr-utils
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
			# binding qgr
			'binding/value.cc',
			'binding/qgr-binding.cc',
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
		'actions': [
			{
				'action_name': 'gen_inl_js_natives',
				'variables': {
					'files': [
						'binding/ext.js',
						'binding/event.js',
						'binding/value.js',
						'../../depe/node/lib/module.js',
					],
				},
				'inputs': [
					'../../tools/gen-js-natives.js',
					'<@(files)',
				],
				'outputs': [
					'../../out/native-inl-js.h',
					'../../out/native-inl-js.cc',
				],
				'action': [
					'<(node)',
					'<@(_inputs)',
					'',
					'-',
					'INL',
					'wrap',
					'<@(_outputs)',
				],
				'process_outputs_as_sources': 1,
			},
			{
				'action_name': 'gen_lib_js_natives',
				'variables': {
					'files': [
						'binding/_keys.js',
						'binding/_pkg.js',
						'binding/_pkgutil.js',
					],
				},
				'inputs': [
					'../../tools/gen-js-natives.js',
					'<@(files)',
				],
				'outputs': [
					'../../out/native-lib-js.h',
					'../../out/native-lib-js.cc',
				],
				'action': [
					'<(node)',
					'<@(_inputs)',
					'',
					'-',
					'LIB',
					'wrap',
					'<@(_outputs)',
				],
				'process_outputs_as_sources': 1,
			},
			{
				'action_name': 'gen_ext_js_natives_',
				'variables': {
					'files': [
						'../../libs/qgr/action.js',
						'../../libs/qgr/app.js',
						'../../libs/qgr/buffer.js',
						'../../libs/qgr/checkbox.jsx',
						'../../libs/qgr/cli.js',
						'../../libs/qgr/css.js',
						'../../libs/qgr/ctr.js',
						'../../libs/qgr/dialog.jsx',
						'../../libs/qgr/display_port.js',
						'../../libs/qgr/event.js',
						'../../libs/qgr/font.js',
						'../../libs/qgr/fs.js',
						'../../libs/qgr/http.js',
						'../../libs/qgr/keys.js',
						'../../libs/qgr/list.jsx',
						'../../libs/qgr/media.js',
						'../../libs/qgr/nav.jsx',
						'../../libs/qgr/overlay.jsx',
						'../../libs/qgr/package.json',
						'../../libs/qgr/path.js',
						'../../libs/qgr/pkg.js',
						'../../libs/qgr/qgr.js',
						'../../libs/qgr/reader.js',
						'../../libs/qgr/stepper.jsx',
						'../../libs/qgr/storage.js',
						'../../libs/qgr/sys.js',
						'../../libs/qgr/timer.js',
						'../../libs/qgr/util.js',
						'../../libs/qgr/value.js',
					],
				},
				'inputs': [
					'../../tools/gen-js-natives.js',
					'<@(files)',
				],
				'outputs': [
					'../../out/native-ext-js.h',
					'../../out/native-ext-js.cc',
				],
				'action': [
					'<(node)',
					'<@(_inputs)',
					'qgr',
					'qgr.js',
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
				'target_name': 'qgr_exec',
				'product_name': 'qgr',
				'type': 'executable',
				'dependencies': [
					'qgr',
					'qgr-js',
					'qgr-node',
					'qgr-media',
				],
				'sources': [
					'main.cc',
				],
				'ldflags': [ '<@(other_ldflags)' ],
			}],
		}]
	],
}
