from fastapi import FastAPI
from data.schemas import Data

app = FastAPI()


data=Data()

@app.get('/get_data', response_model=Data)
def get_data():
    return data

@app.post('add_data', response_model=Data)
def add_value_in_data(value: float, loc: str):
    data.values.append(value)
    data.location = loc
    return data

