INSERT INTO club (name, founded_date, coach)
VALUES
('БАТЭ', '1973-04-12', 'Артём Концевой'),
('Динамо-Минск', '1927-06-18', 'Вадим Скрипченко'),
('Неман Гродно', '1964-01-17', 'Игорь Ковалевич'),
('Торпедо-БелАЗ', '1961-03-21', 'Дмитриий Молош'),
('Славия-Мозырь', '1987-04-11', 'Иван Биончик');


INSERT INTO sponsor(name)
VALUES
('МТС'),
('Белаз'),
('Белгазпромбанк'),
('Горизонт'),
('Мозырский НПЗ');


INSERT INTO club_sponsorship (club_id, sponsor_id, start_date)
SELECT
    c.id,
    s.id,
    '2025-01-01'
FROM club c
CROSS JOIN sponsor s
WHERE (c.name = 'БАТЭ' AND s.name = 'МТС')
   OR (c.name = 'Динамо-Минск' AND s.name = 'Белгазпромбанк')
   OR (c.name = 'Неман Гродно' AND s.name = 'Горизонт')
   OR (c.name = 'Торпедо-БелАЗ' AND s.name = 'Белаз')
   OR (c.name = 'Славия-Мозырь' AND s.name = 'Мозырский НПЗ');


INSERT INTO stadium (name, city, capacity)
VALUES
('Борисов-Арена', 'Борисов', 13126),
('Динамо', 'Минск', 22000),
('Неман', 'Гродно', 8500),
('Торпедо', 'Жодино', 6500),
('Юность', 'Мозырь', 5133);


INSERT INTO player (club_id, first_name, last_name, position, number, birth_date)
SELECT c.id, 'Даниил', 'Мартинович', 'Вратарь', 81, '2006-02-19'
FROM club c WHERE c.name = 'БАТЭ'

UNION ALL

SELECT c.id, 'Артём', 'Рахманов', 'Защитник', 3, '1990-07-10'
FROM club c WHERE c.name = 'БАТЭ'

UNION ALL

SELECT c.id, 'Максим', 'Телеш', 'Полузащитник', 14, '2005-06-25'
FROM club c WHERE c.name = 'БАТЭ'

UNION ALL

SELECT c.id, 'Константин', 'Веретынский', 'Вратарь', 1, '2006-03-23'
FROM club c WHERE c.name = 'Славия-Мозырь'

UNION ALL

SELECT c.id, 'Владислав', 'Давыдов', 'Защитник', 3, '1999-06-18'
FROM club c WHERE c.name = 'Славия-Мозырь'

UNION ALL

SELECT c.id, 'Юлий', 'Кузнецов', 'Полузащитник', 5, '2003-08-02'
FROM club c WHERE c.name = 'Славия-Мозырь'

UNION ALL

SELECT c.id, 'Артём', 'Макавчик', 'Вратарь', 1, '2000-07-04'
FROM club c WHERE c.name = 'Динамо-Минск'

UNION ALL

SELECT c.id, 'Алексей', 'Гаврилович', 'Защитник', 4, '1990-01-05'
FROM club c WHERE c.name = 'Динамо-Минск'

UNION ALL

SELECT c.id, 'Максим', 'Мякиш', 'Полузащитник', 6, '2000-03-03'
FROM club c WHERE c.name = 'Динамо-Минск';


INSERT INTO match (home_club_id, away_club_id, stadium_id, match_date, status, home_score, away_score, referee)
SELECT
    home.id,
    away.id,
    s.id,
    '2025-11-01 19:00:00',
    'expected',
    0,
    0,
    'Виктор Шимусик'
FROM club home, club away, stadium s
WHERE home.name = 'БАТЭ' AND away.name = 'Динамо-Минск' AND s.name = 'Борисов-Арена'

UNION ALL

SELECT
    home.id,
    away.id,
    s.id,
    '2025-11-07 19:00:00',
    'expected',
    0,
    0,
    'Алексей Кульбаков'
FROM club home, club away, stadium s
WHERE home.name = 'Неман Гродно' AND away.name = 'Торпедо-БелАЗ' AND s.name = 'Неман'

UNION ALL

SELECT
    home.id,
    away.id,
    s.id,
    '2025-11-14 19:00:00',
    'expected',
    0,
    0,
    'Дмитрий Дмитриев'
FROM club home, club away, stadium s
WHERE home.name = 'Славия-Мозырь' AND away.name = 'БАТЭ' AND s.name = 'Юность'

UNION ALL

SELECT
    home.id,
    away.id,
    s.id,
    '2025-11-21 19:00:00',
    'expected',
    0,
    0,
    'Сергей Стойко'
FROM club home, club away, stadium s
WHERE home.name = 'БАТЭ' AND away.name = 'Неман Гродно' AND s.name = 'Борисов-Арена'

UNION ALL

SELECT
    home.id,
    away.id,
    s.id,
    '2025-11-28 19:00:00',
    'expected',
    0,
    0,
    'Евгений Наумов'
FROM club home, club away, stadium s
WHERE home.name = 'Динамо-Минск' AND away.name = 'Торпедо-БелАЗ' AND s.name = 'Динамо';