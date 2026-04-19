import json
from dataclasses import dataclass, asdict
from enum import Enum
from typing import Any, Dict
import uuid
from datetime import datetime

class TaskType(Enum):
    SEND_EMAIL = "send_email"
    PROCESS_ORDER = "process_order"
    WRITE_LOG = "write_log"

@dataclass
class Task:
    id: str
    type: TaskType
    payload: Dict[str, Any]
    created_at: str
    retries: int = 0

    def to_json(self) -> str:
        """Преобразование задачи в JSON строку"""
        task_dict = asdict(self)
        task_dict['type'] = self.type.value
        return json.dumps(task_dict)

    @classmethod
    def from_json(cls, json_str: str):
        """Создание задачи из JSON строки"""
        data = json.loads(json_str)
        data['type'] = TaskType(data['type'])
        return cls(**data)

    @classmethod
    def create_task(cls, task_type: TaskType, payload: Dict[str, Any]):
        """Создание новой задачи"""
        return cls(
            id=str(uuid.uuid4()),
            type=task_type,
            payload=payload,
            created_at=datetime.now().isoformat()
        )