# PrometheusEndlineTester
Endline Test Client for Prometheus Boards

# building

install dependencies
~~~
git clone https://github.com/Microsoft/vcpkg.git
cd vcpkg
./bootstrap-vcpkg.sh
./vcpkg integrate install
./vcpkg install curl:x64-windows
./vcpkg install boost:x64-windows
~~~

build with
~~~
-DCMAKE_TOOLCHAIN_FILE=C:/dev/src/vcpkg/scripts/buildsystems/vcpkg.cmake
~~~


# credits

## code
[QR-Code-generator](https://github.com/nayuki/QR-Code-generator) by [nayuki](https://github.com/nayuki)
[libcurl](https://curl.haxx.se/libcurl/) by [The curl project](https://curl.haxx.se/)
[mbed-os](https://github.com/ARMmbed/mbed-os.git) by [ARMmbed](https://github.com/ARMmbed)

## icons and logos

Conformité Européenne Marking by [European Union](https://ec.europa.eu/growth/single-market/ce-marking_en)
WEEE symbol by European Union [European Union](https://commons.wikimedia.org/wiki/File:WEEE_symbol_vectors.svg)

svg of Unicode Character "⧖" by [www.fileformat.info](http://www.fileformat.info/info/unicode/char/29d6/white_hourglass.svg)*
svg of Unicode Character "⏱" by [www.fileformat.info](http://www.fileformat.info/info/unicode/char/23f1/stopwatch.svg)*
svg of Unicode Character "⯈" by [www.fileformat.info](http://www.fileformat.info/info/unicode/char/25b8/black_rightpointing_small_triangle.svg)*
svg of Unicode Character "✓" by [www.fileformat.info](http://www.fileformat.info/info/unicode/char/2713/check_mark.svg)*
svg of Unicode Character "✗" by [www.fileformat.info](https://www.fileformat.info/info/unicode/char/2717/ballot_x.svg)*

*All graphic representations of Unicode characters are in the public domain.
