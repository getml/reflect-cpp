#!/bin/bash

# Скрипт для анализа include файлов с помощью clang-tidy
# Использование: ./analyze_includes.sh [файл.hpp]

set -e

if [ $# -eq 0 ]; then
    echo "Использование: $0 <файл.hpp>"
    echo "Пример: $0 include/rfl/always_false.hpp"
    exit 1
fi

FILE="$1"
BASENAME=$(basename "$FILE" .hpp)

echo "Анализ файла: $FILE"
echo "=========================="

# Создаем временный файл в той же директории, что и анализируемый файл
FILE_DIR=$(dirname "$FILE")
TEMP_CPP="$FILE_DIR/test_${BASENAME}.cpp"

# Определяем относительный путь к файлу от корня проекта
REL_PATH=$(realpath --relative-to="$(pwd)" "$FILE")

cat > "$TEMP_CPP" << EOF
// Временный файл для анализа include файлов
#include <rfl/always_false.hpp>

// Простая функция main для компиляции
int main() {
    return 0;
}
EOF

echo "Создан временный файл: $TEMP_CPP"

# Запускаем clang-tidy анализ с правильными путями
echo "Запуск clang-tidy..."
cd "$(pwd)" && clang-tidy "$TEMP_CPP" --quiet -extra-arg=-Wno-pch-messages 2>&1 | grep -E "(warning|error)" || echo "Нет предупреждений"

# Удаляем временный файл
rm -f "$TEMP_CPP"
echo "Анализ завершен для $FILE"
echo ""