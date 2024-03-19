from fastapi import FastAPI
from measurements.router import router as measurement_router
from database import Base, engine


app = FastAPI()

Base.metadata.create_all(bind=engine)
app.include_router(measurement_router)

@app.get("/")
def greet():
    return {"Message": "Hello!!!"}
