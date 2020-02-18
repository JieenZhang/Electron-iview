@echo off
::python 必须使用2.7 系统为默认
node ./node_modules/node-pre-gyp/bin/node-pre-gyp --target=2.0.18 --runtime=electron --fallback-to-build --directory ./ --dist-url=https://npm.taobao.org/mirrors/atom-shell --msvs_version=2015 configure & node ./node_modules/node-pre-gyp/bin/node-pre-gyp --target=2.0.18 --runtime=electron --fallback-to-build --directory ./ --dist-url=https://npm.taobao.org/mirrors/atom-shell --msvs_version=2015 build

::node ./node_modules/node-pre-gyp/bin/node-pre-gyp --target=2.0.18 --runtime=electron --fallback-to-build --directory ./ --dist-url=https://atom.io/download/electron --msvs_version=2015 configure & node ./node_modules/node-pre-gyp/bin/node-pre-gyp --target=2.0.18 --runtime=electron --fallback-to-build --directory ./ --dist-url=https://atom.io/download/electron --msvs_version=2015 build


npm run dev