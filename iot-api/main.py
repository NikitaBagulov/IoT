from fastapi import FastAPI
from data.schemas import Data

app = FastAPI()


@app.get('/get_data')
def get_data():
    pass

@app.post('add_data', response_model=Data)
def add_data(data: Data):
    pass
