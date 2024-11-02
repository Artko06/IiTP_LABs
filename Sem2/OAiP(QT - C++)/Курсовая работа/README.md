# Разработка программы на языке С++, с использованием фреймворка QT, реализующая игру морской бой

## Функциональная схема программы

![Algorithm scheme](schemes/1.png)

## Блок-схема алгоритма, реализующая авторассановку кораблей

Применяется в `src/BattleShip/addShip.cpp` в функции `AddShips::autoPlacing`.

![Algorithm scheme](schemes/2.png)

## Блок-схема алгоритма, реализующая проверку на корректное расположение кораблей

Применяется в `src/BattleShip/correctposship.cpp` в функции `CorrectPosShip::correctPosShip`.

![Algorithm scheme](schemes/3.png)

## Блок-схема алгоритма, реализующая алгоритм стрельбы для бота

Применяется в `src/BattleShip/bot.cpp` в функциях `Bot::randomShot() и Bot::hitShip()`.

![Algorithm scheme](schemes/4.png)

## Блок-схема алгоритма, реализующая проверку на целостность корабля

Применяется в `src/BattleShip/battlefield.cpp` в функции `Battlefield::checkToKill`.

![Algorithm scheme](schemes/5.png)

## Блок-схема алгоритма, реализующая статистику побед и поражений в игре

Применяется в `src/BattleShip/statswindow.cpp` в функции `StatsWindow::AddStats`.

![Algorithm scheme](schemes/6.png)
