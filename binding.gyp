{
  "targets": [
    {
      "target_name": "<(module_name)",
      "sources": [
        "src/addon.cc",
      ],
      "include_dirs": [
        "<!(node -p \"require('node-addon-api').include_dir\")"
      ],
      "dependencies": [
        "<!(node -p \"require('node-addon-api').gyp\")"
      ],
      "cflags!": [
        "-fno-exceptions"
      ],
      "cflags_cc!": [
        "-fno-exceptions"
      ],
      'conditions': [
         ['OS == "win"', {
          'sources': [
            'src/register_win.cpp',
          ],
          "defines": [
            "_HAS_EXCEPTIONS=1"
          ],
          "msvs_settings": {
            "VCCLCompilerTool": {
             "ExceptionHandling": 1
            }
          }
        }],
        ['OS == "linux"', {
          'sources': [
            'src/register_linux.cpp',
          ],
        }],
        ['OS == "mac"', {
          'sources': [
            'src/register_mac.m',
          ],
          "cflags+": [
             "-fvisibility=hidden"
          ],
          "xcode_settings": {
             "GCC_ENABLE_CPP_EXCEPTIONS": "YES",
             "CLANG_CXX_LIBRARY": "libc++",
             "MACOSX_DEPLOYMENT_TARGET": "10.7",
             "GCC_SYMBOLS_PRIVATE_EXTERN": "YES"
          }
        }],
      ],
      "defines": [
        "NAPI_VERSION=<(napi_build_version)"
      ],
    },
    {
      "target_name": "action_after_build",
      "type": "none",
      "dependencies": [
        "<(module_name)"
      ],
      "copies": [
        {
          "files": [
            "<(PRODUCT_DIR)/<(module_name).node"
          ],
          "destination": "<(module_path)"
        }
      ]
    }
  ]
}
