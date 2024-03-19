from .models import Measurement
from .schemas import TemperatureValue
from database import SessionLocal
from sqlalchemy.orm import Session
from typing import List


def add_measurement(measurement: TemperatureValue):
    db = SessionLocal()
    db_measurement = Measurement(**measurement.model_dump())
    db.add(db_measurement)
    db.commit()
    db.refresh(db_measurement)
    return db_measurement,

def get_measurements_by_location(db: Session, location: str):
    measurements = db.query(Measurement).filter(Measurement.location == location).all()
    if len(measurements) != 0:
        values = [measurements[i].value for i in range(len(measurements))]
        result = {"Values": values, "Location": location}
        return result
    else:
        {"location": "not found"}