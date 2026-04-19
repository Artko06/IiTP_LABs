SELECT s.sid, s.SERIAL#, s.username, t.start_time
FROM v$session s
JOIN v$transaction t ON s.saddr = t.ses_addr;

-- ALTER SYSTEM KILL SESSION 'SID,SERIAL' IMMEDIATE;
ALTER SYSTEM KILL SESSION '388,14976' IMMEDIATE;
