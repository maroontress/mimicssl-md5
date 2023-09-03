# MimicSSL-MD5

This is an [MD5][wikipedia::md5] implementation in C23, and its API is
compatible with [OpenSSL 1.1][openssl::md5_init]. See the
[RFC 1321][ietf::rfc1321] for the MD5 specifications.

Note that the current implementation works only on little-endian platforms.

## Example

An example usage would be as follows:

```c
⋮
MD5_CTX ctx;
uint8_t md[16];
char buffer[1024];

MD5_Init(&ctx);
for (;;) {
    size_t size = fread(buffer, 1, sizeof(buffer), file);
    if (size == 0) {
        break;
    }
    MD5_Update(&ctx, buffer, size);
}
MD5_Final(md, &ctx);
⋮
```

## Build

This repository uses [lighter][maroontress::lighter] for testing as a submodule
of Git. So clone it as follows:

```plaintext
git clone --recursive URL
```

Then build the library on macOS or Linux as follows:

```textplain
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
ctest --test-dir build
cmake --install build --prefix=/path/to/dir
```

Or on Windows as follows:

```textplain
cmake -S . -B build
cmake --build build --config Release
ctest --test-dir build -C Release
cmake --install build --config Release --prefix=/path/to/dir
```

## Build for Android

Set environment variables `ANDROID_HOME` and `ANDROID_NDK` appropriately. For
example:

```sh
export ANDROID_HOME=/usr/local/lib/android/sdk
export ANDROID_NDK=$ANDROID_HOME/ndk/25.2.9519653
```

Note that the value of `ANDROID_HOME` will vary depending on your environment,
but a typical configuration would be as follows:

- Windows: `C:\\Users\\USERNAME\\AppData\\Local\\Android\\sdk`
- Linux: `/home/USERNAME/Android/Sdk`
- macOS: `/Users/USERNAME/Library/Android/sdk`

Then build the library as follows:

```sh
abi=ABI
build_dir=BUILD_DIR
sh android.sh $build_dir configure $abi -G Ninja \
    -DCMAKE_MAKE_PROGRAM="/path/to/ninja" \
    -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_INSTALL_PREFIX:PATH="/path/to/dir"
cmake --build $build_dir/$abi -v
cmake --install $build_dir/$abi
```

`ABI` should be replaced by `arm64-v8a`, `armeabi-v7a`, `x86`, or `x86_64`.
`BUILD_DIR` should be replaced by the build directory (e.g., `build-android`).
Note that `ninja` is required on Windows.

Before running `testsuite`, start Android Emulator as follows:

```sh
$ANDROID_HOME/emulator/emulator -avd AVD_NAME -no-snapshot
```

`AVD_NAME` should be replaced by the AVD name. Note that You can get the list of
AVD names as follows:

```sh
$ANDROID_HOME/emulator/emulator -list-avds
```

Then run `testusite` as follows:

```sh
sh android.sh $build_dir test $abi
```

## Build for iOS

Build the library on macOS as follows:

```sh
build_dir=BUILD_DIR
sh ios.sh $build_dir configure
sh ios.sh $build_dir build --config Release
sh ios.sh $build_dir install /path/to/dir --config Release
```

`BUILD_DIR` should be replaced by the build directory (e.g., `build-ios`).

You can run `testsuite` with the iPhone simulator as follows:

```sh
sh ios.sh $build_dir test ARCH -C Release
```

`ARCH` should be replaced by `arm64` or `x86_64`. Note that `jq` is required to
run the iPhone simulator.

[wikipedia::md5]: https://en.wikipedia.org/wiki/MD5
[ietf::rfc1321]: https://www.ietf.org/rfc/rfc1321.txt
[openssl::md5_init]: https://www.openssl.org/docs/man1.1.1/man3/MD5_Init.html
[maroontress::lighter]: https://github.com/maroontress/lighter
