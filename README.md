# Сборка проекта — csvparser

## Требования

- CMake ≥ 3.16
- Компилятор с поддержкой C11 

---

## Сборка

### Linux / macOS — GCC или Clang

```sh
cmake -S . -B _build
cmake --build _build
```

### Windows — MSVC (cl.exe)

```bat
cmake -S . -B _build -G "Visual Studio 17 2022"
cmake --build _build --config Release
```

### Windows — clang-cl (LLVM + MSVC frontend)

```bat
cmake -S . -B _build -G "Ninja" -DCMAKE_C_COMPILER=clang-cl
cmake --build _build
```

### Windows — Clang (GNU frontend / MinGW-style)

```bat
cmake -S . -B _build -G "Ninja" -DCMAKE_C_COMPILER=clang
cmake --build _build
```

### Cygwin — GCC

В терминале Cygwin:

```sh
cmake -S . -B _build -G "Unix Makefiles"
cmake --build _build
```

---

## Очистка (удаление директории сборки)

```sh
cmake --build _build --target clear
```

Удаляет директорию `_build` целиком.  
На Windows (не Cygwin) используется `rmdir /s /q`, на остальных платформах — `rm -rf`.

---

## Запуск модульных тестов

```sh
cmake --build _build --target csvparser_tests
```

Цель компилирует все файлы из `tests/*.c` вместе с исходниками библиотеки и сразу запускает полученный бинарник `run_tests`.  
Если директория `tests/` пуста, CMake пропускает цель и выводит об этом сообщение на этапе конфигурации.