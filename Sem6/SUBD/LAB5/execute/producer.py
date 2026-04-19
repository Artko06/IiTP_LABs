import time
import redis
from task_schema import Task, TaskType


class TaskProducer:
    def __init__(self, redis_host='localhost', redis_port=6379, queue_name='queue:tasks'):
        self.redis_client = redis.Redis(
            host=redis_host,
            port=redis_port,
            decode_responses=True
        )
        self.queue_name = queue_name

    def add_task(self, task: Task) -> bool:
        """Добавляет задачу в очередь"""
        try:
            task_json = task.to_json()
            result = self.redis_client.lpush(self.queue_name, task_json)
            print(f"Task {task.id} added to queue. Queue length: {result}")
            return result > 0
        except Exception as e:
            print(f"Error adding task to queue: {e}")
            return False

    @staticmethod
    def create_email_task(to_email: str, subject: str, body: str) -> Task:
        """Создает задачу отправки email"""
        payload = {
            "to": to_email,
            "subject": subject,
            "body": body,
            "status": "pending"
        }
        return Task.create_task(TaskType.SEND_EMAIL, payload)

    @staticmethod
    def create_order_task(order_id: int, customer_id: int, amount: float) -> Task:
        """Создает задачу обработки заказа"""
        payload = {
            "order_id": order_id,
            "customer_id": customer_id,
            "amount": amount,
            "status": "pending",
            "items": []
        }
        return Task.create_task(TaskType.PROCESS_ORDER, payload)

    @staticmethod
    def create_log_task(service_name: str, level: str, message: str) -> Task:
        """Создает задачу записи лога"""
        payload = {
            "service": service_name,
            "level": level,
            "message": message,
            "timestamp": time.time()
        }
        return Task.create_task(TaskType.WRITE_LOG, payload)

    def produce_example_tasks(self):
        """Генерация различных типов задач"""
        email_task = self.create_email_task(
            to_email="user@example.com",
            subject="Welcome!",
            body="Thank you for registering!"
        )
        self.add_task(email_task)

        order_task = self.create_order_task(
            order_id=1001,
            customer_id=12345,
            amount=99.99
        )
        self.add_task(order_task)

        log_task = self.create_log_task(
            service_name="auth-service",
            level="INFO",
            message="User login successful"
        )
        self.add_task(log_task)

if __name__ == "__main__":
    producer = TaskProducer()
    producer.produce_example_tasks()