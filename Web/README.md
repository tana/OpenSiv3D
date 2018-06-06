# OpenSiv3DをWebに移植する
## 注意
現状のコードは*正常に動作しません*。
エンジンの初期化は可能ですが、背景色設定以外の描画機能は使えません(何も描画されない)。

## 現在の移植方法
- [Emscripten](http://kripken.github.io/emscripten-site/)を使ってWebAssemblyに変換しています。
- ベースはLinux版OpenSiv3Dです。今はLinux版のソースコードをそのまま変更していますが、今後条件コンパイル等を利用して整理する予定です。
- 移植が難しいもの(CPU関係、OSに依存する部分など)は、コメントアウトやプリプロセッサによる無効化、コンパイル対象から外すなどの方法で対応しています。
- AngelScriptも移植できない部分があるようなので([参考1](http://www.angelcode.com/angelscript/sdk/docs/manual/doc_register_func.html)、[参考2](https://www.gamedev.net/forums/topic/643190-compiling-angelscript-runtime-with-emscripten/))、無効化しています。
- 一部の入出力機能等はWebブラウザのAPIに移植する予定ですが、現在は無効化されているものも多くあります。
- グラフィックスにはWebGL2を使っていますが、OpenGLの新しい機能には対応していないものも多いので、グラフィックス関係は大きな変更を加えています。
- Emscriptenではメインループをコールバックにする必要があるため([参考](https://kripken.github.io/emscripten-site/docs/porting/emscripten-runtime-environment.html#browser-main-loop))、Siv3Dを使うアプリケーションのコードにも変更が必要です。

## ビルド方法
Emscriptenがインストールされており、`emcc`や`emmake`等のコマンドが使えるようなっている必要があります。
### 依存ライブラリの準備
依存ライブラリは`Web/deps`ディレクトリ内に`lib`、`include`、`bin`、`share`のようなディレクトリ構造を作って配置します。
- Web/
    - deps/
        - bin/
        - lib/
        - include/
        - share/
        - hogehoge-x.y.z/ (各ライブラリのソースを展開したディレクトリ)
        - ...
    - ...

基本的には、`./configure`、`make`等のコマンドにEmscriptenの`emconfigure`、`emmake`のようなコマンド([参考](http://kripken.github.io/emscripten-site/docs/compiling/Building-Projects.html#building-projects))をつけてソースコード(tar.gz等)からビルドします。

(各ライブラリのビルドはすべて、ライブラリのソースアーカイブを展開したディレクトリに入ってから行う想定で書いています)
#### [libjpeg-turbo](https://libjpeg-turbo.org/)
アセンブラを使わないようにSIMDを無効化してビルドします。
```
emconfigure ./configure --prefix=`pwd`/.. --without-simd
emmake make -j4
emmake make install
```
#### [giflib](https://sourceforge.net/projects/giflib/)
```
emconfigure ./configure --prefix=`pwd`/..
emmake make -j4
emmake make install
```
#### [FreeType](https://www.freetype.org/)
FreeTypeはビルド時に内部でFreeType自身を使うようなので、通常ビルドした後にEmscriptenでビルドします([参考](https://kripken.github.io/emscripten-site/docs/compiling/Building-Projects.html#build-system-issues))
```
./configure
make -j4
emconfigure ./configure --prefix=`pwd`/..
emmake make -j4
emmake make install
```
#### [zlib](https://zlib.net/)
```
emconfigure ./configure --prefix=`pwd`/..
emmake make -j4
emmake make install
```
#### [libpng](http://www.libpng.org/pub/png/libpng.html)
zlibに依存しています。
```
export ZLIBLIB=`pwd`/../lib
export ZLIBINC=`pwd`/../include
export CPPFLAGS="-I$ZLIBINC"
export LDFLAGS="-L$ZLIBLIB"
emconfigure ./configure --prefix=`pwd`/..
emmake make -j4
emmake make install
```
#### [Boost](https://www.boost.org/)
`./configure`、`make`、`make install`の方式ではないようなので、[Boostドキュメントのビルド方法](https://www.boost.org/doc/libs/1_66_0/more/getting_started/unix-variants.html)を参考にしてビルドします。
Emscripten用のビルド設定がすでに入っているようなので
([参考](https://stackoverflow.com/questions/15724357/using-boost-with-emscripten/47751199#47751199))、それを利用します。
現在はBoost 1.65.1を想定しています。
```
./bootstrap.sh
./b2 --prefix=`pwd`/.. --with-system --with-filesystem toolset=emscripten
./b2 --prefix=`pwd`/.. --with-system --with-filesystem toolset=emscripten install
```
#### [OpenCV](https://opencv.org/)
OpenCVは[ドキュメント](https://docs.opencv.org/3.4.1/d7/d9f/tutorial_linux_install.html)を参考にして、CMakeで設定とビルドを行います。
ただし、ソースディレクトリとは別のディレクトリでビルドする必要があるので、`build`ディレクトリを作成して、その中でビルドします。
```
mkdir build
cd build/
emcmake cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=`pwd`/../.. -DWITH_TBB=OFF -DWITH_PTHREADS_PF=OFF -DWITH_ITT=OFF -DWITH_TIFF=OFF -DWITH_JPEG=OFF -DWITH_JASPER=OFF -DWITH_PNG=OFF -DWITH_OPENEXR=OFF -DWITH_WEBP=OFF -DBUILD_opencv_apps=OFF -DBUILD_PERF_TESTS=OFF -DBUILD_TESTS=OFF ..
emmake make -j4
emmake make install
```
#### [HarfBuzz](https://www.freedesktop.org/wiki/Software/HarfBuzz/)
FreeTypeと組み合わせる必要があるようです。
```
emconfigure env CPPFLAGS="-I`pwd`/../include -I`pwd`/../include/freetype2 -DHB_NO_MT" LDFLAGS="-L`pwd`/../lib" PKG_CONFIG_PATH="`pwd`/../lib/pkgconfig" ./configure --prefix=`pwd`/.. --without-icu --with-freetype
emmake make -j4
emmake make install
```

### OpenSiv3D自体のビルド
`Web`ディレクトリの下に`Debug`ディレクトリを作ってから、その中に入って作業します。
```
emcmake cmake -DCMAKE_BUILD_TYPE=Debug ..
emmake make -j4
```

### テストプログラムのビルドと実行
Linux版と同じように`Web/Test`ディレクトリに入り、`resources`と`example`のリンクを作成してから、
次のようにビルドします。
```
emcmake cmake .
emmake make -j4
```

実行する歳は、`emrun`コマンドを使うとローカルWebサーバが立ち上がってからブラウザが起動します
([参考](https://kripken.github.io/emscripten-site/docs/compiling/Running-html-files-with-emrun.html))。
```
emrun Siv3D_Test.html
```