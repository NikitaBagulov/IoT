from fastapi import APIRouter, Depends
from measurements.schemas import TemperatureValue
from sqlalchemy.orm import Session
from .crud import add_measurement, get_measurements_by_location
from database import SessionLocal

router = APIRouter()

def get_db():
    db = SessionLocal()
    try:
        yield db
    finally:
        db.close()

@router.post("/submit_temperature")
def submit_temperature(measurement: TemperatureValue):
    added_measurement = add_measurement(measurement)
    return {"status": "updated", "measurement": added_measurement}

@router.get("/temperature")
def get_temperature(location: str, db: Session = Depends(get_db)):
    measurements = get_measurements_by_location(db, location)
    return {"measurements": measurements}