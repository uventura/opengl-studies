echo "====================="
echo "      BUILDING      "
echo -e "=====================\n"

cmake -S . -B out/build

pushd out/build
make
popd
