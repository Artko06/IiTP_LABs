import redis
import time

class RateLimiter:
    def __init__(self, redis_host='localhost', redis_port=6379):
        self.redis = redis.Redis(host=redis_host, port=redis_port, decode_responses=True)

    def check_simple(self, user_id: str, limit: int = 50, window: int = 300) -> bool:
        """Не более limit запросов за window секунд"""
        key = f"rate:{user_id}:{int(time.time() / window)}"

        pipe = self.redis.pipeline()
        pipe.incr(key)
        pipe.expire(key, window)
        count, _ = pipe.execute()

        return count <= limit

    def check_sliding(self, user_id: str, limit: int = 50, window: int = 300) -> bool:
        """Скользящее окно - более точное ограничение"""
        key = f"rate_slide:{user_id}"
        now = time.time()

        lua = """
        local key = KEYS[1]
        local now = tonumber(ARGV[1])
        local window = tonumber(ARGV[2])
        local limit = tonumber(ARGV[3])
        
        redis.call('ZREMRANGEBYSCORE', key, 0, now - window)
        local count = redis.call('ZCARD', key)
        
        if count < limit then
            redis.call('ZADD', key, now, now)
            redis.call('EXPIRE', key, window)
            return 1
        end
        return 0
        """

        return bool(self.redis.eval(lua, 1, key, now, window, limit))

def demo_rate_limiting():
    limiter = RateLimiter()
    user = "user123"

    print("\n1. Простой счетчик c TTL (10 запросов и ttl = 30 сек):")
    for i in range(15):
        allowed = limiter.check_simple(user, limit=10, window=30)
        print(f"Запрос {i+1}: {'✓ Разрешено' if allowed else '✗ Отклонено'}")
        time.sleep(0.1)

    print("\n2. Скользящее окно (5 запросов и ttl = 30 сек):")
    for i in range(10):
        allowed = limiter.check_sliding(user, limit=5, window=30)
        print(f"Запрос {i+1}: {'✓ Разрешено' if allowed else '✗ Отклонено'}")
        time.sleep(0.1)

if __name__ == "__main__":
    demo_rate_limiting()