from pydantic import BaseModel

class Data(BaseModel):
    values: list
    location: str