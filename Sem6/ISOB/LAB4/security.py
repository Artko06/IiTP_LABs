import time
from collections import defaultdict
import config


class SecurityManager:
    def __init__(self):
        self.history = defaultdict(list)
        self.bans = {}

    def is_allowed(self, ip):
        now = time.time()

        if ip in self.bans:
            if now < self.bans[ip]:
                remaining = int(self.bans[ip] - now)
                print(f"[SECURITY] IP {ip} заблокирован. Осталось: {remaining} сек.")
                return False
            else:
                print(f"[SECURITY] Срок бана для {ip} истек. Доступ разрешен.")
                del self.bans[ip]

        self.history[ip] = [t for t in self.history[ip] if now - t < config.RATE_LIMIT_WINDOW]

        if len(self.history[ip]) >= config.MAX_REQUESTS_PER_IP:
            self.bans[ip] = now + config.BAN_DURATION
            print(f"[SECURITY] IP {ip} превысил лимит и забанен на {config.BAN_DURATION} сек.")
            return False

        self.history[ip].append(now)
        return True

    def validate_message_size(self, data):
        return len(data) <= config.MAX_MESSAGE_SIZE


security_mgr = SecurityManager()