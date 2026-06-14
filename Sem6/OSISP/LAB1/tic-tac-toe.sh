#!/bin/bash

BOARD=(" " " " " " " " " " " " " " " " " ")
PLAYER="X"
WINNER=""
MOVE_COUNT=0
GAME_OVER=0
SCREENSHOT_DIR="./screenshots"

mkdir -p "$SCREENSHOT_DIR"


clear_screen() {
    printf "\033[2J\033[H"
}


display_board() {
    clear_screen
    echo "==================================="
    echo "      КРЕСТИКИ-НОЛИКИ"
    echo "==================================="
    echo ""
    echo "      Игрок 1: X"
    echo "      Игрок 2: O"
    echo ""
    echo "        1 | 2 | 3 "
    echo "       ---+---+---"
    echo "        4 | 5 | 6 "
    echo "       ---+---+---"
    echo "        7 | 8 | 9 "
    echo ""
    echo "==================================="
    echo ""
    echo "       ${BOARD[0]} | ${BOARD[1]} | ${BOARD[2]} "
    echo "      ---+---+---"
    echo "       ${BOARD[3]} | ${BOARD[4]} | ${BOARD[5]} "
    echo "      ---+---+---"
    echo "       ${BOARD[6]} | ${BOARD[7]} | ${BOARD[8]} "
    echo ""
    
    if [[ $GAME_OVER -eq 0 ]]; then
        echo "Ход игрока: $PLAYER"
        echo "Введите номер клетки (1-9) или 's' для скриншота:"
    fi
}


check_winner() {
    # Горизонтальные линии
    for i in 0 3 6; do
        if [[ ${BOARD[$i]} != " " && "${BOARD[$i]}" == "${BOARD[$((i+1))]}" && "${BOARD[$i]}" == "${BOARD[$((i+2))]}" ]]; then
            WINNER="${BOARD[$i]}"
            return 0
        fi
    done
    
    # Вертикальные линии
    for i in 0 1 2; do
        if [[ ${BOARD[$i]} != " " && "${BOARD[$i]}" == "${BOARD[$((i+3))]}" && "${BOARD[$i]}" == "${BOARD[$((i+6))]}" ]]; then
            WINNER="${BOARD[$i]}"
            return 0
        fi
    done
    
    # Диагонали
    if [[ ${BOARD[0]} != " " && "${BOARD[0]}" == "${BOARD[4]}" && "${BOARD[0]}" == "${BOARD[8]}" ]]; then
        WINNER="${BOARD[0]}"
        return 0
    fi
    
    if [[ ${BOARD[2]} != " " && "${BOARD[2]}" == "${BOARD[4]}" && "${BOARD[2]}" == "${BOARD[6]}" ]]; then
        WINNER="${BOARD[2]}"
        return 0
    fi
    
    # Проверка на ничью
    if [[ $MOVE_COUNT -eq 9 ]]; then
        WINNER="DRAW"
        return 0
    fi
    
    return 1
}

make_move() {
    local position=$1
    
    # Проверяем, что позиция в пределах 1-9
    if ! [[ $position =~ ^[1-9]$ ]]; then
        echo "Ошибка: введите число от 1 до 9"
        return 1
    fi
    
    # Конвертируем в индекс массива (0-8)
    local index=$((position-1))
    
    # Проверяем, что клетка свободна
    if [[ ${BOARD[$index]} != " " ]]; then
        echo "Клетка уже занята! Выберите другую."
        return 1
    fi
    
    # Делаем ход
    BOARD[index]=$PLAYER
    MOVE_COUNT=$((MOVE_COUNT + 1))
    
    # Проверяем на победу
    if check_winner; then
        GAME_OVER=1
        return 0
    fi
    
    # Меняем игрока
    if [[ $PLAYER == "X" ]]; then
        PLAYER="O"
    else
        PLAYER="X"
    fi
    
    return 0
}


save_screenshot() {
    local timestamp
    timestamp=$(date +"%Y%m%d_%H%M%S")
    local screenshot_file="$SCREENSHOT_DIR/game_${timestamp}.txt"
    
    cat > "$screenshot_file" << EOF
===================================
      КРЕСТИКИ-НОЛИКИ
===================================
Дата: $(date "+%d.%m.%Y %H:%M:%S")
Ход: $MOVE_COUNT
Игрок: $PLAYER

       ${BOARD[0]} | ${BOARD[1]} | ${BOARD[2]} 
      ---+---+---
       ${BOARD[3]} | ${BOARD[4]} | ${BOARD[5]} 
      ---+---+---
       ${BOARD[6]} | ${BOARD[7]} | ${BOARD[8]} 

$(if [[ $GAME_OVER -eq 1 ]]; then
    if [[ $WINNER == "DRAW" ]]; then
        echo "Результат: НИЧЬЯ!"
    else
        echo "Результат: Победил игрок $WINNER!"
    fi
fi)
EOF
    
    echo "Скриншот сохранен: $screenshot_file"
    read -r -p "Нажмите Enter для продолжения..."
}


display_result() {
    clear_screen
    echo "==================================="
    echo "      ИГРА ЗАВЕРШЕНА"
    echo "==================================="
    echo ""
    echo "       ${BOARD[0]} | ${BOARD[1]} | ${BOARD[2]} "
    echo "      ---+---+---"
    echo "       ${BOARD[3]} | ${BOARD[4]} | ${BOARD[5]} "
    echo "      ---+---+---"
    echo "       ${BOARD[6]} | ${BOARD[7]} | ${BOARD[8]} "
    echo ""
    
    if [[ $WINNER == "DRAW" ]]; then
        echo "   НИЧЬЯ! Победила дружба!"
    else
        echo "   ПОБЕДИЛ ИГРОК $WINNER!"
    fi
    
    echo ""
    echo "Количество ходов: $MOVE_COUNT"
    echo ""
    echo "==================================="
    echo "Нажмите:"
    echo "  n - Новая игра"
    echo "  s - Сохранить скриншот"
    echo "  q - Выйти из игры"
}


new_game() {
    BOARD=(" " " " " " " " " " " " " " " " " ")
    PLAYER="X"
    WINNER=""
    MOVE_COUNT=0
    GAME_OVER=0
}


main() {
    local input
    
    while true; do
        if [[ $GAME_OVER -eq 0 ]]; then
            display_board
            
            # Читаем ввод пользователя
            read -r -p "> " input
            
            case $input in
                [1-9])
                    if make_move "$input"; then
                        if [[ $GAME_OVER -eq 1 ]]; then
                            display_result
                        fi
                    else
                        echo "Некорректный ход!"
                        read -r -p "Нажмите Enter для продолжения..."
                    fi
                    ;;
                s|S)
                    save_screenshot
                    ;;
                q|Q)
                    echo "Выход из игры..."
                    exit 0
                    ;;
                *)
                    echo "Некорректный ввод. Используйте 1-9, s (скриншот) или q (выход)"
                    read -r -p "Нажмите Enter для продолжения..."
                    ;;
            esac
        else
            # Игра завершена, ждем действия пользователя
            read -r -p "> " input
            
            case $input in
                n|N)
                    new_game
                    ;;
                s|S)
                    save_screenshot
                    ;;
                q|Q)
                    echo "Выход из игры..."
                    exit 0
                    ;;
                *)
                    echo "Некорректный ввод. Используйте n, s или q"
                    ;;
            esac
        fi
    done
}

trap 'echo -e "\n\nВыход из игры..."; exit 0' INT

main

# bash tic-tac-toe.sh