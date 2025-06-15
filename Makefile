.PHONY: build
.DEFAULT_GOAL := help

help:
	@echo "Type: make [rule]. Available options are:"
	@echo ""
	@echo "- help"
	@echo "- format"
	@echo "- clean"
	@echo ""
	@echo "- build"
	@echo "- build-complete"
	@echo "- run"
	@echo "- package"
	@echo ""

format:
	find -E src/ -regex '.*\.(cpp|hpp|cc|cxx|c|h)' -exec clang-format -style=file -i {} \;

windows-format:
	powershell -Command "Get-ChildItem -Path src -Recurse -Include *.cpp,*.hpp,*.cc,*.cxx,*.c,*.h | ForEach-Object { clang-format -style=file -i $$_.FullName }"

clean:
	rm -rf build
	find . -name ".DS_Store" -delete

build:
	rm -rf build
	cmake -B build .
	cmake --build build

build-complete:
	rm -rf build
	cmake -B build -DGENERATE_ICONS=ON .
	cmake --build build

package: build
	cd build && cpack

run:
	./build/MyApp
