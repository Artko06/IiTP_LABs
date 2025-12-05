PRAGMA foreign_keys = ON;

CREATE TABLE club
(
    id INTEGER PRIMARY KEY AUTOINCREMENT,

    name TEXT UNIQUE NOT NULL,
    founded_date DATE NOT NULL,
    coach TEXT
);

CREATE TABLE sponsor(
    id INTEGER PRIMARY KEY AUTOINCREMENT,

    name TEXT UNIQUE NOT NULL
);

CREATE TABLE club_sponsorship (
    id INTEGER PRIMARY KEY AUTOINCREMENT,

    club_id INTEGER NOT NULL,
    sponsor_id INTEGER NOT NULL,

    start_date DATE NOT NULL,

    FOREIGN KEY (club_id) REFERENCES club(id) ON DELETE CASCADE,
    FOREIGN KEY (sponsor_id) REFERENCES sponsor(id) ON DELETE CASCADE
);

CREATE TABLE stadium (
    id INTEGER PRIMARY KEY AUTOINCREMENT,

    name TEXT NOT NULL UNIQUE,
    city TEXT,
    capacity INTEGER CHECK (capacity > 0)
);

CREATE TABLE player (
    id INTEGER PRIMARY KEY AUTOINCREMENT,

    club_id INTEGER NOT NULL,

    first_name TEXT NOT NULL,
    last_name TEXT NOT NULL,
    position TEXT NOT NULL ,
    number INTEGER CHECK (number BETWEEN 1 AND 99),
    birth_date DATE NOT NULL,

    FOREIGN KEY (club_id) REFERENCES club(id) ON DELETE SET NULL
);

CREATE TABLE match (
    id INTEGER PRIMARY KEY AUTOINCREMENT,

    home_club_id INTEGER NOT NULL,
    away_club_id INTEGER NOT NULL,
    stadium_id INTEGER NOT NULL,

    match_date DATETIME NOT NULL,
    status TEXT DEFAULT 'expected' CHECK (status IN ('expected', 'playing', 'completed')),
    home_score INTEGER DEFAULT 0 CHECK (home_score >= 0),
    away_score INTEGER DEFAULT 0 CHECK (away_score >= 0),
    referee TEXT,

    FOREIGN KEY (home_club_id) REFERENCES club(id) ON DELETE CASCADE,
    FOREIGN KEY (away_club_id) REFERENCES club(id) ON DELETE CASCADE,
    FOREIGN KEY (stadium_id) REFERENCES stadium(id) ON DELETE CASCADE,

    CHECK (home_club_id != away_club_id)
);


