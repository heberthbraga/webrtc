{
	'includes': [
    	'cmx.gypi',
  	],
  	'targets': [
  		{
  			'target_name': 'cmx_base',
        'type': 'static_library',
        'include_dirs': [ '.', '../..', ],
  			'sources': [
        		'<@(cmx_sources)',
      		],
  		},
  	],
}