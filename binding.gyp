{
  # NOTE: 'module_name' and 'module_path' come from the 'binary' property in package.json
  # node-pre-gyp handles passing them down to node-gyp when you build from source
  "targets": [
    {
      "target_name": "<(module_name)",
      "sources": [ "src/LogicManagerWrap.cpp",
                    "src/Disk/DiskWrap.cpp",
                    "src/Login/LoginWrap.cpp",
	"src/LogicCallBackManager.cpp" ],
      "include_dirs": [
        "<!(node -e \"require('nan')\")",
'src\Disk',
					'src\Login',
					'src',
		"E:\Lenovo2.0\WFBS-2015\src",
		"E:\Lenovo2.0\WFBS-2015\src\WFBSManager-2015ActiveX",
		"E:\Lenovo2.0\WFBS-2015\src\WFBSManager-2015ActiveX\ManagerLib\Logic",
		"E:\Lenovo2.0\WFBS-2015\src\WFBSManager-2015ActiveX\ManagerLib\SessionManager",
		"E:\Lenovo2.0\WFBS-2015\src\WFBSManager-2015ActiveX\ManagerLib\Socket",
		"E:\Lenovo2.0\WFBS-2015\src\WFBSManager-2015ActiveX\ManagerLib\Utils",
		"E:\Lenovo2.0\WFBS-2015\src\WFBSManager-2015ActiveX\System"
      ],
      "defines": [
        "_BSCNT_",
        "_WINSOCK_DEPRECATED_NO_WARNINGS",
		"RSACRYPT_EXPORTS"
      ],
	  "libraries": [
		"E:\Lenovo2.0\WFBS-2015\src\Lib\cryptlib64.lib",
		"E:\Lenovo2.0\WFBS-2015\src\Lib\RSACryptElectron.lib",
		"E:\Lenovo2.0\WFBS-2015\src\Lib\zlibwapi.lib",
		"E:\Lenovo2.0\WFBS-2015\src\WFBSManager-2015ActiveX\Lib64\Release\ManagerLib.lib",
		"E:\Lenovo2.0\WFBS-2015\src\WFBSManager-2015ActiveX\Lib64\Release\System.lib"
	  ],
      'product_dir': '<(module_path)',
      "xcode_settings": {
        "MACOSX_DEPLOYMENT_TARGET":"10.9"
      }
    }
  ]
}
