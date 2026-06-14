set -e
mkdir -p build web/data
cp data/*.json web/data/ 2>/dev/null || true
emcmake cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build -j4
cp build/mysteryos.js build/mysteryos.wasm web/
cp build/mysteryos.data web/ 2>/dev/null || true
cp build/mysteryos.html web/index.html
echo "Done. Run: python3 -m http.server 8080 --directory web"