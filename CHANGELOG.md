[1.1.0] - 2026-04-27

# Added
* **CHANGELOG.md**
* **CLI Flag System**

# Fixed
* **The space in path bug**: Fixed a critical issue where scripts in folders with spaces would fail. Commands are now safely wrapped in double quotes before being passed to system().