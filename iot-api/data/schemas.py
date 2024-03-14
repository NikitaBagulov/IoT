from pydantic import BaseModel
from typing import List

class Data(BaseModel):
    values: List[float]
    location: str