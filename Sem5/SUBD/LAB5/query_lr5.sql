-- 1.1 Вывод всех данных обо всех футбольных клубах

SELECT *
FROM club;

-- 1.2 Вывод имён и фамилий игроков футбольного клуба "Славия-Мозырь", отсортированных по фамилии в алфавитном порядке

SELECT first_name, last_name
FROM player
WHERE club_id = (SELECT id FROM club WHERE name = 'Славия-Мозырь')
ORDER BY last_name;

-- 1.3 Вывод данных об игроках, фамилии которых начинаются на букву "М"

SELECT *
FROM player
WHERE last_name LIKE 'М%';

-- 2.1 Вывод всех матчей клубов, которые основаны в апреле

SELECT
    hc.name AS home_club,
    hc.founded_date AS home_founded_date,
    ac.name AS away_club,
    ac.founded_date AS away_founded_date,
    s.name AS stadium,
    m.match_date,
    m.status,
    m.referee
FROM match m
JOIN club hc ON m.home_club_id = hc.id
JOIN club ac ON m.away_club_id = ac.id
JOIN stadium s ON m.stadium_id = s.id
WHERE (hc.founded_date LIKE '____-04-__' OR ac.founded_date LIKE '____-04-__');

-- 2.2 Вывод всех игроков моложе 25 лет, не играющих за футбольный клуб "Славия-Мозырь"

SELECT
    p.first_name,
    p.last_name,
    p.position,
    p.number,
    p.birth_date,
    c.name AS club_name,
    (strftime('%Y', 'now') - strftime('%Y', p.birth_date)) -
    (strftime('%m-%d', 'now') < strftime('%m-%d', p.birth_date)) AS age
FROM player p
JOIN club c ON p.club_id = c.id
WHERE (strftime('%Y', 'now') - strftime('%Y', p.birth_date)) -
      (strftime('%m-%d', 'now') < strftime('%m-%d', p.birth_date)) < 25
  AND c.name != 'Славия-Мозырь';

-- 2.3 Вывод информации о матче с указанием названий клубов, стадиона и даты проведения

SELECT
    hc.name AS home_club,
    ac.name AS away_club,
    s.name AS stadium,
    m.match_date,
    m.status,
    m.referee,
    m.home_score,
    m.away_score
FROM match m
JOIN club hc ON m.home_club_id = hc.id
JOIN club ac ON m.away_club_id = ac.id
JOIN stadium s ON m.stadium_id = s.id;

-- 2.4 Вывод информации о матчах, которые проводятся не на стадионе "Юность"

SELECT
    hc.name AS home_club,
    ac.name AS away_club,
    s.name AS stadium,
    m.match_date,
    m.status,
    m.referee,
    m.home_score,
    m.away_score
FROM match m
JOIN club hc ON m.home_club_id = hc.id
JOIN club ac ON m.away_club_id = ac.id
JOIN stadium s ON m.stadium_id = s.id
WHERE s.name != 'Юность';

-- 2.5 Вывод отсортированного списка спонсоров и тренеров клуба

SELECT
    s.name,
    c.coach,
    c.name
FROM club_sponsorship cs
JOIN club c ON cs.club_id = c.id
JOIN sponsor s ON cs.sponsor_id = s.id
ORDER BY s.name;

-- 2.6 Вывод отсортированного списка всех тренеров и названий их клубов с указанием количества игроков в клубе

SELECT
       c.coach,
       c.name,
       count(p.id) AS count_players
FROM club c
LEFT JOIN player p ON p.club_id = c.id
WHERE c.coach IS NOT NULL
GROUP BY c.coach, c.name
ORDER BY c.coach;

-- 2.7 Вывод названия стадионов, на которых еще не проводились матчи

SELECT
    s.name AS stadium_name,
    s.city,
    s.capacity
FROM stadium s
LEFT JOIN match m ON s.id = m.stadium_id
WHERE m.id IS NULL
