import time
import redis
from task_schema import Task, TaskType


class TaskConsumer:
    def __init__(self, redis_host='localhost', redis_port=6379, queue_name='queue:tasks'):
        self.redis_client = redis.Redis(
            host=redis_host,
            port=redis_port,
            decode_responses=True
        )
        self.queue_name = queue_name
        self.running = False

    def process_task(self, task: Task) -> bool:
        """Обработка задачи в зависимости от её типа"""
        try:
            print(f"\nProcessing task {task.id} of type {task.type.value}")

            if task.type == TaskType.SEND_EMAIL:
                return self._process_email_task(task)
            elif task.type == TaskType.PROCESS_ORDER:
                return self._process_order_task(task)
            elif task.type == TaskType.WRITE_LOG:
                return self._process_log_task(task)
            else:
                print(f"Unknown task type: {task.type}")
                return False

        except Exception as e:
            print(f"Error processing task {task.id}: {e}")
            return False

    @staticmethod
    def _process_email_task(task: Task) -> bool:
        """Обработка задачи отправки email"""
        payload = task.payload
        print(f"Sending email to {payload['to']}")
        print(f"Subject: {payload['subject']}")
        print(f"Body: {payload['body'][:50]}...")

        time.sleep(1)

        print(f"Email sent successfully to {payload['to']}")
        return True

    @staticmethod
    def _process_order_task(task: Task) -> bool:
        """Обработка задачи заказа"""
        payload = task.payload
        print(f"Processing order {payload['order_id']}")
        print(f"Customer: {payload['customer_id']}")
        print(f"Amount: ${payload['amount']}")

        time.sleep(2)

        print(f"Order {payload['order_id']} processed successfully")
        return True

    @staticmethod
    def _process_log_task(task: Task) -> bool:
        """Обработка задачи записи лога"""
        payload = task.payload
        print(f"Writing log: [{payload['level']}] {payload['service']} - {payload['message']}")

        time.sleep(0.5)

        print(f"Log written successfully")
        return True

    def start_consuming(self, timeout: int = 5):
        """Запуск потребителя в бесконечном цикле"""
        self.running = True
        print(f"Consumer started. Listening on queue: {self.queue_name}")

        while self.running:
            try:
                result = self.redis_client.brpop(self.queue_name, timeout=timeout)

                if result:
                    _, task_json = result
                    task = Task.from_json(task_json)

                    success = self.process_task(task)

                    if not success and task.retries < 3:
                        task.retries += 1
                        retry_json = task.to_json()
                        self.redis_client.lpush(self.queue_name, retry_json)
                        print(f"Task {task.id} scheduled for retry ({task.retries}/3)")

            except redis.exceptions.ConnectionError as e:
                print(f"Redis connection error: {e}. Retrying in 5 seconds...")
                time.sleep(5)
            except KeyboardInterrupt:
                print("\nConsumer stopped by user")
                self.running = False
            except Exception as e:
                print(f"Unexpected error: {e}")
                time.sleep(1)

    def stop_consuming(self):
        """Остановка потребителя"""
        self.running = False

if __name__ == "__main__":
    consumer = TaskConsumer()
    consumer.start_consuming()
