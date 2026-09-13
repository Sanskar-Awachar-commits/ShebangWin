# Changelog

## [1.2.0] - 2026-09-13

### Added
* **Subdirectory execution**: Automatically changes the working directory to the target file's directory during command execution, allowing relative compilation and output in subfolders.
* **UTF-8 BOM support**: Added automatic stripping of UTF-8 Byte Order Marks (`\xEF\xBB\xBF`) for files saved with BOM on Windows.
* **Shebang header validation**: Validates that the first line starts with the expected comment prefix and avoids executing arbitrary non-comment code.
* **Exit code propagation**: Now forwards the exit code from `system()` to the shell for better CLI script and CI/CD integration.
* **Expanded language support**: Added support for Rust (`.rs`), Go (`.go`), Zig (`.zig`), Swift (`.swift`), Kotlin (`.kt`, `.kts`), Scala (`.scala`), Dart (`.dart`), C# (`.cs`), JavaScript/TypeScript (`.js`, `.mjs`, `.cjs`, `.ts`, `.jsx`, `.tsx`), PHP (`.php`), Command (`.cmd`), Bash/Zsh (`.bash`, `.zsh`), R (`.r`), Perl (`.pl`), YAML/TOML (`.yaml`, `.yml`, `.toml`), and Ada (`.ada`).

### Fixed
* **Windows relative path resolution**: Fixed path resolution in `check_file()` when passing explicit relative paths like `.\file.cpp` or `src/file.cpp`.
* **System command quote handling**: Removed redundant outer quotes that broke nested quotation in Windows `cmd.exe /c`.
* **CLI argument bounds checking**: Added proper early exit checks in `main()` to prevent NULL pointer dereferences when no arguments or excess arguments are passed.

---

## [1.1.0] - 2026-04-27

### Added
* **CHANGELOG.md**
* **CLI Flag System** (`-h`, `--help`, `-v`, `--version`)

### Fixed
* **The space in path bug**: Fixed an issue where scripts in folders with spaces would fail.