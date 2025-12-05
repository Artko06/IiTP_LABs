-- 1.1 Определить количество завершенных матчей

SELECT COUNT(*) AS completed_matches_count
FROM match
WHERE status = 'completed';

-- 1.2 Вывести 3 самых молодых клуба

SELECT name, founded_date
FROM club
ORDER BY founded_date DESC
LIMIT 3;

-- 1.3 Получить количество клубов и спонсоров по которым заключены

SELECT
    COUNT(DISTINCT club_id) AS active_clubs_count,
    COUNT(DISTINCT sponsor_id) AS active_sponsors_count
FROM club_sponsorship;

-- 1.4 Вывести список стадионов отсортированный по количеству запланированных матчей

SELECT
    s.name,
    s.city,
    COUNT(m.id) AS matches_hosted
FROM stadium s
LEFT JOIN match m ON s.id = m.stadium_id
WHERE m.status = 'expected'
GROUP BY s.id
ORDER BY matches_hosted DESC;

-- 1.5 Вывести клубы у которых нет в базе данных ни одного игрока

SELECT
    c.name,
    0 AS count_players
FROM club c
LEFT JOIN player p ON c.id = p.club_id
WHERE p.id IS NULL;

-- 1.6 Вывести клубы у которых средний возраст игроков максимален.
-- Клубы, где в базе данных ни одного игрока не учитывать
-- Округлить до двух знаков после запятой

SELECT
    c.name,
    ROUND(
            AVG(
                    (strftime('%Y', 'now') - strftime('%Y', p.birth_date)) -
        (strftime('%m-%d', 'now') < strftime('%m-%d', p.birth_date)
            )
        ), 2) AS avg_age
FROM club c
JOIN player p ON c.id = p.club_id
GROUP BY c.id
HAVING COUNT(p.id) > 0
ORDER BY avg_age DESC;